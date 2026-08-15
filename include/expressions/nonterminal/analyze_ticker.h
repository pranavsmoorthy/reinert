#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_TICKER_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_TICKER_H_

#include "expressions/terminal/string_expression.h"
#include "expressions/terminal/number_expression.h"
#include "analyze_expression.h"

#include "context.h"

#include <any>

// Alias for the specific VectorForge node type utilized in the application's graph
using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

/**
 * @class AnalyzeTickerExpression
 * @brief An AST node that performs a vector database search by retrieving live financial data for a specific ticker.
 * 
 * This class inherits from AnalyzeExpression to utilize profile management. When executed, it resolves 
 * the ticker symbol, fetches the latest market and SEC data to build a dynamic coordinate vector, 
 * and queries the vector database to find historical earnings setups with similar technical and fundamental profiles.
 */
class AnalyzeTickerExpression : public AnalyzeExpression {
    private:
        /**
         * @brief Pointer to the AST node containing the parsed stock ticker symbol.
         */
        StringExpression* ticker_expression_;
        
        /**
         * @brief Pointer to the AST node containing the manually provided consecutive EPS beats.
         */
        NumberExpression* consecutive_eps_beats_expression_;
        
        /**
         * @brief Pointer to the AST node containing the manually provided average EPS surprise.
         */
        NumberExpression* avg_eps_surprise_expressions_;

    public:
        /**
         * @brief Constructs a new AnalyzeTickerExpression.
         * 
         * @param ticker The StringExpression holding the stock ticker symbol.
         * @param consecutive_eps_beats The NumberExpression holding the consecutive earnings beats track record.
         * @param avg_eps_surprise The NumberExpression holding the historical average earnings surprise.
         */
        AnalyzeTickerExpression(
            StringExpression* ticker, 
            NumberExpression* consecutive_eps_beats, 
            NumberExpression* avg_eps_surprise);

        /**
         * @brief Destructor responsible for safely deleting the child terminal expressions from memory.
         */
        ~AnalyzeTickerExpression();

        /**
         * @brief Executes the ticker analysis command.
         * 
         * Retrieves the ticker string, fetches external data using the application's Retriever utilities, 
         * constructs the 12-dimensional query vector, applies the active search profile, and outputs 
         * the nearest neighbor graph nodes to the terminal.
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& ctx) const override;
};

#endif