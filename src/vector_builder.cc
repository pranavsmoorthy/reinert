#include "utils/vector_builder.h"

#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

nlohmann::json VectorBuilder::ticker_chart_json_;
nlohmann::json VectorBuilder::sec_edgar_json_;

nlohmann::json VectorBuilder::spy_chart_json_;
nlohmann::json VectorBuilder::vix_chart_json_;
nlohmann::json VectorBuilder::tnx_chart_json_;

std::vector<double> VectorBuilder::ExtractCloses(const nlohmann::json& chart_json) {
    std::vector<double> closes;
    if (!chart_json.contains("chart") || chart_json["chart"]["result"].empty()) return closes;
    
    auto quote = chart_json["chart"]["result"][0]["indicators"]["quote"][0];
    for (const auto& price : quote["close"]) {
        if (!price.is_null()) {
            closes.push_back(price.get<double>());
        } else if (!closes.empty()) {
            closes.push_back(closes.back());
        }
    }
    return closes;
}

double VectorBuilder::GetLatestPrice(const nlohmann::json& chart_json) {
    auto closes = ExtractCloses(chart_json);
    return closes.empty() ? 0.0 : closes.back();
}

void VectorBuilder::SetJsonData(const std::string& ticker, const Context& ctx) {
    ticker_chart_json_ = Retriever::FetchYFinanceJson(ticker);

    std::string cik_number = ctx.cik_map.GetCIK(ticker);
    sec_edgar_json_ = Retriever::FetchSECEdgarJson(cik_number);
}

void VectorBuilder::SetSpyVixTnxData() {
    spy_chart_json_ = Retriever::FetchYFinanceJson("SPY");
    vix_chart_json_ = Retriever::FetchYFinanceJson("%5Evix");
    tnx_chart_json_ = Retriever::FetchYFinanceJson("%5Etnx");
}

// Dim 0
double VectorBuilder::ThirtyDayMomentum() {
    auto closes = ExtractCloses(ticker_chart_json_);
    if (closes.size() <= 30) return 0.0;
    
    double current = closes.back();
    double past = closes[closes.size() - 1 - 30];
    return (current - past) / past;
}

// Dim 1
double VectorBuilder::TwoWeekRSI() {
    auto closes = ExtractCloses(ticker_chart_json_);
    if (closes.size() <= 14) return 0.5;

    double avg_gain = 0.0;
    double avg_loss = 0.0;

    for (int i = 1; i <= 14; ++i) {
        double change = closes[i] - closes[i - 1];
        if (change > 0) avg_gain += change;
        else avg_loss -= change;
    }
    avg_gain /= 14.0;
    avg_loss /= 14.0;

    for (size_t i = 15; i < closes.size(); ++i) {
        double change = closes[i] - closes[i - 1];
        double gain = (change > 0) ? change : 0.0;
        double loss = (change < 0) ? -change : 0.0;

        avg_gain = ((avg_gain * 13.0) + gain) / 14.0;
        avg_loss = ((avg_loss * 13.0) + loss) / 14.0;
    }

    if (avg_loss == 0.0) return 1.0; 
    double rs = avg_gain / avg_loss;
    double rsi = 100.0 - (100.0 / (1.0 + rs));
    
    return rsi / 100.0; 
}

// Dim 2
double VectorBuilder::FiftyDaySMADistance() {
    auto closes = ExtractCloses(ticker_chart_json_);
    if (closes.size() < 50) return 0.0;

    double sum = 0.0;
    for (size_t i = closes.size() - 50; i < closes.size(); ++i) {
        sum += closes[i];
    }
    double sma = sum / 50.0;
    double current = closes.back();
    
    return (current - sma) / sma;
}

// Dim 3
double VectorBuilder::SectorETFMomentum() {
    auto closes = ExtractCloses(spy_chart_json_);
    if (closes.size() <= 30) return 0.0;
    
    double current = closes.back();
    double past = closes[closes.size() - 1 - 30];
    return (current - past) / past;
}

// Dim 6
double VectorBuilder::PreviousQuarterGap() {
    try {
        // 1. Get latest filing date from SEC
        std::string filed_date_str = "";
        const auto& revenues = sec_edgar_json_["facts"]["us-gaap"]["Revenues"]["units"]["USD"];
        for (auto it = revenues.rbegin(); it != revenues.rend(); ++it) {
            if ((*it).value("form", "") == "10-Q" || (*it).value("form", "") == "10-K") {
                filed_date_str = (*it).value("filed", "");
                break;
            }
        }
        
        if (filed_date_str.empty()) return 0.0;

        // Convert YYYY-MM-DD to UNIX timestamp
        std::tm tm = {};
        std::istringstream ss(filed_date_str);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        std::time_t filed_unix = std::mktime(&tm);

        // 2. Find closest timestamp index in Yahoo Chart
        auto timestamps = ticker_chart_json_["chart"]["result"][0]["timestamp"];
        auto closes = ExtractCloses(ticker_chart_json_);
        
        int target_idx = -1;
        for (size_t i = 0; i < timestamps.size(); ++i) {
            if (timestamps[i].get<std::time_t>() >= filed_unix) {
                target_idx = i;
                break;
            }
        }

        // 3. Calculate Gap: (P[t+1] - P[t-1]) / P[t-1]
        if (target_idx > 0 && target_idx + 1 < static_cast<int>(closes.size())) {
            double price_before = closes[target_idx - 1];
            double price_after = closes[target_idx + 1];
            if (price_before > 0.0) {
                return (price_after - price_before) / price_before;
            }
        }
    } catch (...) {
        return 0.0;
    }
    return 0.0;
}

// Dim 7
double VectorBuilder::TrailingPERatio() {
    try {
        double current_price = GetLatestPrice(ticker_chart_json_);
        double ttm_eps = 0.0;
        int quarters_found = 0;

        const auto& eps_arr = sec_edgar_json_["facts"]["us-gaap"]["EarningsPerShareBasic"]["units"]["USD/shares"];
        
        for (auto it = eps_arr.rbegin(); it != eps_arr.rend() && quarters_found < 4; ++it) {
            std::string form = (*it).value("form", "");
            if (form == "10-Q" || form == "10-K") {
                ttm_eps += (*it).value("val", 0.0);
                quarters_found++;
            }
        }

        if (ttm_eps <= 0.0) return 0.0; 
        return current_price / ttm_eps;
    } catch (...) {
        return 0.0;
    }
}

// Dim 8
double VectorBuilder::YearOverYearRevenueGrowth() {
    try {
        const auto& rev_arr = sec_edgar_json_["facts"]["us-gaap"]["Revenues"]["units"]["USD"];
        
        double current_rev = 0.0;
        double prior_rev = 0.0;
        std::string current_frame = "";

        // Find most recent quarterly revenue
        for (auto it = rev_arr.rbegin(); it != rev_arr.rend(); ++it) {
            std::string frame = (*it).value("frame", "");
            if (frame.length() >= 6 && frame.find("Q") != std::string::npos) { // e.g. CY2025Q2
                current_rev = (*it).value("val", 0.0);
                current_frame = frame;
                break;
            }
        }

        if (current_frame.empty()) return 0.0;

        // Construct prior year frame (e.g., CY2025Q2 -> CY2024Q2)
        int year = std::stoi(current_frame.substr(2, 4));
        std::string prior_frame = "CY" + std::to_string(year - 1) + current_frame.substr(6);

        // Find prior year revenue
        for (auto it = rev_arr.rbegin(); it != rev_arr.rend(); ++it) {
            if ((*it).value("frame", "") == prior_frame) {
                prior_rev = (*it).value("val", 0.0);
                break;
            }
        }

        if (prior_rev == 0.0) return 0.0;
        return (current_rev - prior_rev) / std::abs(prior_rev);
    } catch (...) {
        return 0.0;
    }
}

// Dim 9
double VectorBuilder::MarketCap() {
    try {
        double current_price = GetLatestPrice(ticker_chart_json_);
        double shares_out = 0.0;

        const auto& shares_arr = sec_edgar_json_["facts"]["dei"]["EntityCommonStockSharesOutstanding"]["units"]["shares"];
        if (!shares_arr.empty()) {
            shares_out = shares_arr.back().value("val", 0.0);
        }

        return current_price * shares_out;
    } catch (...) {
        return 0.0;
    }
}

// Dim 10
double VectorBuilder::VIXLevel() {
    return GetLatestPrice(vix_chart_json_);
}

// Dim 11
double VectorBuilder::TenYearYield() {
    return GetLatestPrice(tnx_chart_json_);
}

std::array<double, 12> VectorBuilder::MakeCoords(double consecutive_eps_beats, double eps_surprise) {
    std::array<double, 12> raw_coords = {0};

    raw_coords[0]  = ThirtyDayMomentum();
    raw_coords[1]  = TwoWeekRSI();
    raw_coords[2]  = FiftyDaySMADistance();
    raw_coords[3]  = SectorETFMomentum();
    raw_coords[4]  = consecutive_eps_beats;     // Direct override
    raw_coords[5]  = eps_surprise;              // Direct override
    raw_coords[6]  = PreviousQuarterGap();
    raw_coords[7]  = TrailingPERatio();
    raw_coords[8]  = YearOverYearRevenueGrowth();
    raw_coords[9]  = MarketCap();
    raw_coords[10] = VIXLevel();
    raw_coords[11] = TenYearYield();

    return raw_coords;
}