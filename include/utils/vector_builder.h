#pragma once

#ifndef INCLUDE_UTILS_VECTOR_BUILDER_H_
#define INCLUDE_UTILS_VECTOR_BUILDER_H_

#include <nlohmann/json.hpp>
#include <string>
#include <array>

#include "retriever.h"
#include "context.h"

/**
 * @class VectorBuilder
 * @brief A utility class responsible for constructing the 12-dimensional feature vector.
 * This class aggregates raw financial data from Yahoo Finance and SEC EDGAR APIs,
 * calculates specific technical, fundamental, and macroeconomic indicators, and
 * compiles them into a coordinate array for vector database querying.
*/
class VectorBuilder {
    private: 
        /**
         * @brief Stored JSON response containing the target ticker's daily chart data.
        */
        static nlohmann::json ticker_chart_json_;
        /**
         * @brief Stored JSON response containing the target ticker's SEC company facts.
         */
        static nlohmann::json sec_edgar_json_;

        /**
         * @brief Stored JSON responses for macro market indicators (S&P 500, Volatility Index, 10-Year Treasury).
         */
        static nlohmann::json spy_chart_json_;
        static nlohmann::json vix_chart_json_;
        static nlohmann::json tnx_chart_json_;

        // Technical and Fundamental Calculation Helpers

        static double ThirtyDayMomentum();
        static double TwoWeekRSI();
        static double FiftyDaySMADistance();
        static double SectorETFMomentum();

        // Consecutive EPS Beats is passed manually
        // EPS Surprise is passed manually
        
        static double PreviousQuarterGap();
        static double TrailingPERatio();
        static double YearOverYearRevenueGrowth();
        static double MarketCap();
        static double VIXLevel();
        static double TenYearYield();

        /**
         * @brief Extracts a continuous series of closing prices from a provided chart JSON.
         * 
         * @param chart_json The JSON object containing the financial chart data.
         * @return std::vector<double> A sequence of daily closing prices.
         */
        static std::vector<double> ExtractCloses(const nlohmann::json& chart_json);

        /**
         * @brief Retrieves the most recent closing price from the provided chart JSON.
         * 
         * @param chart_json The JSON object containing the financial chart data.
         * @return double The latest available closing price.
         */
        static double GetLatestPrice(const nlohmann::json& chart_json);

    public:
        /**
        * @brief Fetches and initializes the ticker-specific JSON data using the application's Context.
        *
        * @param ticker The stock ticker symbol to build a vector for.
        * @param ctx The global memory and state context of the application.
        */
        static void SetJsonData(const std::string& ticker, const Context& ctx);

        /**
         * @brief Fetches and initializes the macroeconomic market data required for vector baseline comparisons.
         */
        static void SetSpyVixTnxData();

        /**
         * @brief Calculates and compiles all internal data points into a finalized 12-dimensional coordinate array.
         * 
         * @param consecutive_eps_beats The user-provided consecutive EPS beats track record.
         * @param eps_surprise The user-provided historical average earnings surprise.
         * @return std::array<double, 12> The unscaled raw coordinate vector ready for preprocessing.
         */
        static std::array<double, 12> MakeCoords(double consecutive_eps_beats, double eps_surprise);
};

#endif