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

// Alias for the specific VectorForge node type utilized in the application's graph
using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>; 

/**
 * @struct Formatter
 * @brief A utility structure for generating aligned, tabular terminal output for vector search results.
 * 
 * Provides static helper methods to convert mathematical and vector database types 
 * into human-readable strings, ensuring precise column alignment for console displays.
 */
struct Formatter { 
    
    /**
     * @brief Formats a double-precision floating-point number to a specific number of decimal places.
     * 
     * @param val The numerical value to be formatted.
     * @param decimals The desired number of trailing decimal places.
     * @return std::string The formatted numerical string.
     */
    static std::string FormatDecimal(double val, int decimals) { 
        std::ostringstream oss; 
        oss << std::fixed << std::setprecision(decimals) << val; 
        return oss.str(); 
    }

    /**
     * @brief Formats a VectorForge graph node's data into a padded, tabular string row.
     * 
     * This method aligns the ticker, date, calculated cosine similarity, Euclidean distance, 
     * and percentage move into predetermined column widths for clean terminal output.
     * 
     * @param in The graph node containing the earnings setup data.
     * @param query The 12-dimensional query vector used to calculate the similarity and distance.
     * @return std::string The formatted row string ready for standard output.
     */
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