#pragma once

#ifndef INCLUDE_UTILS_VECTOR_BUILDER_H_
#define INCLUDE_UTILS_VECTOR_BUILDER_H_

#include <nlohmann/json.hpp>
#include <string>
#include <array>

#include "retriever.h"
#include "context.h"

class VectorBuilder {
    private: 
        static nlohmann::json ticker_chart_json_;
        static nlohmann::json sec_edgar_json_;

        static nlohmann::json spy_chart_json_;
        static nlohmann::json vix_chart_json_;
        static nlohmann::json tnx_chart_json_;

        static double ThirtyDayMomentum();
        static double TwoWeekRSI();
        static double FiftyDaySMADistance();
        static double SectorETFMomentum();
        // Consecutive EPS Beats
        // EPS Surprise
        static double PreviousQuarterGap();
        static double TrailingPERatio();
        static double YearOverYearRevenueGrowth();
        static double MarketCap();
        static double VIXLevel();
        static double TenYearYield();

        static std::vector<double> ExtractCloses(const nlohmann::json& chart_json);
        static double GetLatestPrice(const nlohmann::json& chart_json);

    public:
        static void SetJsonData(const std::string& ticker, const Context& ctx);
        static void SetSpyVixTnxData();

        static std::array<double, 12> MakeCoords(double consecutive_eps_beats, double eps_surprise);
};

#endif