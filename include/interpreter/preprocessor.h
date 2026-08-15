#pragma once

#include <array>
#include <algorithm>
#include <cmath>

/**
 * @class Preprocessor
 * @brief A utility class for normalizing and scaling raw financial data.
 * 
 * The Preprocessor prepares raw stock market and macro-economic data points 
 * by clamping them to expected bounds and scaling them into a normalized 
 * vector space (typically between 0.0 and 1.0 or -1.0 and 1.0) before they 
 * are inserted into or queried against the vector database.
 */
class Preprocessor {
private:
    /**
     * @brief Clamps and normalizes a raw value between specified minimum and maximum bounds.
     * 
     * @param raw The raw input value to scale.
     * @param min_bound The minimum expected threshold.
     * @param max_bound The maximum expected threshold.
     * @return double The normalized value scaled relative to the bounds.
     */
    static double Scale(double raw, double min_bound, double max_bound) {
        double clamped = std::clamp(raw, min_bound, max_bound);
        return (clamped - min_bound) / (max_bound - min_bound);
    }

public:
    /**
     * @brief Transforms a raw 12-dimensional feature vector into a normalized coordinate vector.
     * 
     * This function processes 12 specific financial features across four categories: 
     * Technicals, Track Record, Valuation, and Macro indicators. Market cap utilizes 
     * a Log10 transformation prior to scaling.
     * 
     * @param raw The 12-element array of raw financial metrics.
     * @return std::array<double, 12> The processed and scaled coordinate vector.
     */
    static std::array<double, 12> TransformCoordinates(const std::array<double, 12>& raw) {
        std::array<double, 12> p;

        // Category 1: Technicals
        p[0] = Scale(raw[0], -0.5, 0.5);      // 30-Day Momentum
        p[1] = std::clamp(raw[1], 0.0, 1.0);  // RSI (Already 0-1, just clamp to be safe)
        p[2] = Scale(raw[2], -0.4, 0.4);      // Distance from 50 SMA
        p[3] = Scale(raw[3], -0.2, 0.2);      // Sector ETF Momentum

        // Category 2: Track Record
        p[4] = Scale(raw[4], 0.0, 8.0);       // Consecutive Beats
        p[5] = Scale(raw[5], -1.0, 1.0);      // Avg EPS Surprise
        p[6] = Scale(raw[6], -0.3, 0.3);      // Previous Gap

        // Category 3: Valuation
        p[7] = Scale(raw[7], 0.0, 100.0);     // Trailing P/E
        p[8] = Scale(raw[8], -0.5, 1.0);      // YoY Rev Growth
        
        // Market Cap requires a Log10 transformation before scaling
        double log_cap = (raw[9] > 0) ? std::log10(raw[9]) : 9.0;
        p[9] = Scale(log_cap, 9.0, 12.47);    // Cap between $1B and ~$3T

        // Category 4: Macro
        p[10] = Scale(raw[10], 10.0, 85.0);   // VIX
        p[11] = Scale(raw[11], 0.0, 10.0);    // 10Y Yield

        return p;
    }
};