#pragma once

#ifndef FORMATTER_H_
#define FORMATTER_H_

#include "../data/model_resources/earnings_struct.h"

#include "vectorforge/vector_base.h"
#include "vectorforge/node.h"

#include <string>
#include <array>
#include <sstream>
#include <iomanip>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

// Ticker     Date            Similarity     Deviation     Went Up?     Percent Change
// #####      ####-##-##      ###.####%      ###.####%     YES          ###.####%

struct Formatter {
    static std::string FormatDecimal(double val, int decimals) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(decimals) << val;
        return oss.str();
    }

    static std::string FormatNode(const vectorforge::node::Node<EarningsStruct, double, 12, 16>& in, const std::array<double, 12>& query) {
        std::ostringstream oss;

        oss << std::left 
            << std::setw(11) << in.GetData().ticker
            << std::setw(16) << in.GetData().date
            << std::setw(15) << (FormatDecimal(in.CosineSimilarityTo(query) * 100, 4) + "%")
            << std::setw(14) << (FormatDecimal(in.DistanceToCoords(query), 4))
            << std::setw(13) << (in.GetData().label ? "YES" : "NO")
            << (in.GetData().label ? "+" : "")
            << (FormatDecimal(in.GetData().move_pct, 4) + "%");

        return oss.str();
    }
};

#endif