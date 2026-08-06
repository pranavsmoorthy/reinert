import yfinance as yf
import pandas as pd
import pandas_ta_classic as ta
import numpy as np
from sklearn.preprocessing import MinMaxScaler
from datetime import datetime, timedelta
import time
import json
import os

# ==========================================
# CONFIGURATION
# ==========================================
# File containing our list of stock tickers
TICKERS_FILE = "tickers.json"

SECTOR_ETF = "SPY" 
OUTPUT_FILE = "../vectorforge_12d_dataset.csv"

# ==========================================
# HELPER FUNCTIONS
# ==========================================
def load_tickers(filepath):
    """Loads tickers from a JSON file, or fetches from the SEC if missing."""
    import requests
    import os

    if not os.path.exists(filepath):
        print(f"⚠️ Could not find {filepath}. Downloading from SEC...")
        # The SEC API requires a custom User-Agent string to prevent abuse
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) VectorForgeDatasetBuilder/1.0'
        }
        try:
            response = requests.get('https://www.sec.gov/files/company_tickers.json', headers=headers)
            response.raise_for_status()
            sec_data = response.json()
            
            # The SEC JSON looks like: {"0": {"cik_str": 320193, "ticker": "AAPL", "title": "Apple Inc."}}
            tickers_list = []
            for key, company_info in sec_data.items():
                tickers_list.append(company_info['ticker'])
                
            # Take the top 500 for our dataset (you can remove this slice to run on all ~10,000!)
            tickers_list = tickers_list[:500] 

            # Save it locally in the simple array format for next time
            with open(filepath, 'w') as f:
                json.dump(tickers_list, f, indent=4)
            print(f"✅ Downloaded {len(tickers_list)} tickers and saved to {filepath}.")
            return tickers_list
            
        except Exception as e:
            print(f"❌ Failed to download from SEC: {e}")
            print("Creating a default fallback list...")
            default_tickers = [
                "AAPL", "MSFT", "GOOGL", "AMZN", "META", 
                "TSLA", "NVDA", "AMD", "NFLX", "CRM",
                "JPM", "V", "MA", "DIS", "PYPL"
            ]
            with open(filepath, 'w') as f:
                json.dump(default_tickers, f, indent=4)
            return default_tickers
        
    with open(filepath, 'r') as f:
        data = json.load(f)
        # Handle the case where the user manually downloaded the SEC JSON
        if isinstance(data, dict) and "0" in data and "ticker" in data["0"]:
            print(f"🔄 Detected raw SEC JSON format in {filepath}. Extracting tickers...")
            return [info['ticker'] for key, info in data.items()]
        
        # Assume it's already a clean array
        return data

def get_historical_prices(ticker, period="5y"):
    """Fetches daily historical prices and cleans the index."""
    df = yf.download(ticker, period=period, progress=False)
    if isinstance(df.columns, pd.MultiIndex):
        df.columns = df.columns.droplevel(1)
    if not df.empty:
        df.index = pd.to_datetime(df.index).tz_localize(None).normalize()
    return df

def get_macro_data():
    """Fetches the VIX, 10-Year Treasury Yield, and Sector ETF."""
    print("Fetching Macro Environment data (^VIX, ^TNX, SPY)...")
    vix = get_historical_prices("^VIX", "5y")
    tnx = get_historical_prices("^TNX", "5y")
    spy = get_historical_prices(SECTOR_ETF, "5y")
    if not spy.empty:
        spy['Mom_30'] = spy['Close'].pct_change(periods=30)
    return vix, tnx, spy

# ==========================================
# MAIN PIPELINE
# ==========================================
def build_dataset():
    vix_df, tnx_df, spy_df = get_macro_data()
    all_vectors = []

    today = pd.Timestamp.now().normalize()
    tickers_list = load_tickers(TICKERS_FILE)

    print(f"Loaded {len(tickers_list)} tickers from {TICKERS_FILE}.")

    for ticker_sym in tickers_list:
        print(f"\n📊 Processing {ticker_sym}...")
        try:
            ticker = yf.Ticker(ticker_sym)
            
            # --- QUALITY FILTERS ---
            info = ticker.info
            
            # Filter 1: Market Cap > $1 Billion
            market_cap = info.get('marketCap', 0)
            if market_cap < 10_000_000:
                print(f"  -> Skipping: Market Cap too low (${market_cap:,.0f})")
                continue
                
            # Filter 2: Average Daily Volume > 1 Million Shares
            avg_volume = info.get('averageVolume', 0)
            if avg_volume < 750_000:
                 print(f"  -> Skipping: Volume too low ({avg_volume:,.0f} shares)")
                 continue
                 
            # Filter 3: Current Price > $5.00 (Avoid penny stocks)
            current_price = info.get('currentPrice', info.get('regularMarketPrice', 0))
            if current_price < 5.0:
                 print(f"  -> Skipping: Price too low (${current_price:.2f})")
                 continue
                 
            # 1. Get Earnings History directly from the company profile!
            earnings_df = ticker.get_earnings_dates(limit=20) 
            if earnings_df is None or earnings_df.empty:
                print(f"  -> Skipping: No earnings data found.")
                continue
                
            # Clean up the earnings dates
            earnings_df.index = pd.to_datetime(earnings_df.index).tz_localize(None).normalize()
            # Sort chronologically (oldest to newest)
            earnings_df = earnings_df.sort_index()
            # Filter out future earnings dates (we can't predict the future yet!)
            past_earnings = earnings_df[earnings_df.index < today]

            # 2. Get Price History
            price_df = get_historical_prices(ticker_sym, "5y")
            if price_df.empty:
                print(f"  -> Skipping: No price data found.")
                continue
            
            price_df['SMA_50'] = ta.sma(price_df['Close'], length=50)
            price_df['RSI_14'] = ta.rsi(price_df['Close'], length=14)
            price_df['Mom_30'] = price_df['Close'].pct_change(periods=30)

            # 3. Iterate through each historical earnings date for this stock
            for i in range(len(past_earnings)):
                target_date = past_earnings.index[i]
                
                # We need at least 4 previous quarters of history BEFORE this date to calculate track records
                if i < 4:
                    continue

                past_days = price_df[price_df.index < target_date]
                future_days = price_df[price_df.index > target_date]
                
                if past_days.empty or future_days.empty or len(past_days) < 50:
                    continue 
                
                t_minus_1 = past_days.index[-1]
                t_plus_1 = future_days.index[0]
                
                current_stats = price_df.loc[t_minus_1]

                # --- The "Truth" ---
                price_before = current_stats['Close']
                price_after = price_df.loc[t_plus_1]['Close']
                move_pct = (price_after - price_before) / price_before
                went_up = int(move_pct > 0)

                # --- CATEGORY 1: Technicals ---
                dim_0 = current_stats['Mom_30']
                dim_1 = current_stats['RSI_14'] / 100.0 
                dim_2 = (current_stats['Close'] - current_stats['SMA_50']) / current_stats['SMA_50']
                
                try:
                    dim_3 = spy_df.loc[t_minus_1]['Mom_30']
                except:
                    dim_3 = 0.0

                # --- CATEGORY 2: Earnings Track Record ---
                # Get the 4 reports leading up to this one
                past_4_quarters = past_earnings.iloc[i-4:i]
                
                consecutive_beats = 0
                for surprise in reversed(past_4_quarters['Surprise(%)']):
                    if pd.notna(surprise) and surprise > 0:
                        consecutive_beats += 1
                    else:
                        break
                dim_4 = consecutive_beats
                
                dim_5 = past_4_quarters['Surprise(%)'].mean()
                if pd.isna(dim_5): dim_5 = 0.0
                
                prev_report_date = past_earnings.index[i-1]
                try:
                    prev_t_minus_1 = price_df[price_df.index < prev_report_date].index[-1]
                    prev_t_plus_1 = price_df[price_df.index > prev_report_date].index[0]
                    dim_6 = (price_df.loc[prev_t_plus_1]['Close'] - price_df.loc[prev_t_minus_1]['Close']) / price_df.loc[prev_t_minus_1]['Close']
                except:
                    dim_6 = 0.0

                # --- CATEGORY 3: Valuation ---
                ttm_eps = past_4_quarters['Reported EPS'].sum()
                dim_7 = current_stats['Close'] / ttm_eps if pd.notna(ttm_eps) and ttm_eps > 0 else 0.0
                
                eps_1yr_ago = past_earnings.iloc[i-4]['Reported EPS']
                curr_eps = past_earnings.iloc[i-1]['Reported EPS']
                dim_8 = (curr_eps - eps_1yr_ago) / abs(eps_1yr_ago) if pd.notna(eps_1yr_ago) and eps_1yr_ago != 0 else 0.0
                
                try:
                    shares_out = ticker.info.get('sharesOutstanding', 1)
                except:
                    shares_out = 1
                dim_9 = current_stats['Close'] * shares_out

                # --- CATEGORY 4: Macro ---
                try:
                    dim_10 = vix_df.loc[t_minus_1]['Close']
                    dim_11 = tnx_df.loc[t_minus_1]['Close']
                except:
                    dim_10 = 20.0 
                    dim_11 = 4.0  

                # Compile Vector
                vector_data = {
                    "Ticker": ticker_sym,
                    "Earnings_Date": target_date.strftime("%Y-%m-%d"),
                    "Move_Pct": round(move_pct * 100, 2),
                    "Went_Up": went_up,
                    "Dim0": dim_0, "Dim1": dim_1, "Dim2": dim_2, "Dim3": dim_3,
                    "Dim4": dim_4, "Dim5": dim_5, "Dim6": dim_6, "Dim7": dim_7,
                    "Dim8": dim_8, "Dim9": dim_9, "Dim10": dim_10, "Dim11": dim_11
                }
                
                # Check for NaNs before appending to ensure mathematical safety for the Vector DB
                if not pd.isna(list(vector_data.values())[4:]).any():
                    all_vectors.append(vector_data)
                    print(f"  -> Extracted Vector for {target_date.strftime('%Y-%m-%d')}")
                
        except Exception as e:
            print(f"  -> Error processing {ticker_sym}: {e}")
            
        time.sleep(1.0) # Yahoo API rate limit protection

    # ==========================================
    # DATA NORMALIZATION
    # ==========================================
    print(f"\nNormalizing {len(all_vectors)} rows of data for VectorForge...")
    df = pd.DataFrame(all_vectors)
    
    if df.empty:
        print("Error: No data was gathered.")
        return

    dim_cols = [f"Dim{i}" for i in range(12)]
    
    # Force all 12 dimensions to live on a 0.0 - 1.0 scale
    scaler = MinMaxScaler()
    df[dim_cols] = scaler.fit_transform(df[dim_cols])

    df.to_csv(OUTPUT_FILE, index=False)
    print(f"Success! Saved {len(df)} historical earnings vectors to {OUTPUT_FILE}")

if __name__ == "__main__":
    build_dataset()