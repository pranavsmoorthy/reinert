#ifndef DATA_EARNINGS_STRUCT_H_
#define DATA_EARNINGS_STRUCT_H_

#include <array>
#include <string>

struct EarningsStruct {
    // The Dimensions of the vector
    // Normalized via StandardScaler. Raw values represent the exact day BEFORE earnings.
    //
    // --- Technical Price Action & Volatility ---
    // [0] = 14-day RSI (Momentum: is it overbought or oversold?)
    // [1] = MACD Histogram (Trend acceleration heading into earnings)
    // [2] = Distance from 50-day SMA (Short-term trend relative to price)
    // [3] = Distance from 200-day SMA (Macro trend relative to price)
    // [4] = Normalized ATR (Average True Range - how wildly the stock is swinging)
    // [5] = Bollinger Band Position (Where price sits relative to statistical extremes)
    //
    // --- Fundamentals & Track Record ---
    // [6] = Dividend Yield (Dimension 7: Often 0.0 for growth stocks)
    // [7] = Previous Quarter Earnings Surprise % (Company's track record of beating estimates)
    // [8] = Trailing P/E Ratio (Current valuation multiple)
    // [9] = Price-to-Sales Ratio (Top-line valuation, useful for unprofitable tech)
    //
    // --- Macroeconomics & Market Sentiment ---
    // [10] = S&P 500 5-Day Return (Broader market momentum leading into the report)
    // [11] = VIX Level (Overall fear/volatility in the macro market)


    std::string ticker; // The stock symbol
    std::string date;   // The date of the report
    double move_pct;    // Percent moved after report

    // The "Truth" label for the classifier
    // Represents the price action the morning AFTER the earnings report
    // True = Stock went UP 
    // False = Stock went DOWN 
    bool label;

    EarningsStruct() : ticker(""), date(""), move_pct(0.0), label(false) {}

    bool operator==(const EarningsStruct& other) const {
        return (ticker == other.ticker) && 
            (date == other.date) && 
            (move_pct == other.move_pct) && 
            (label == other.label);
    }
};

#endif