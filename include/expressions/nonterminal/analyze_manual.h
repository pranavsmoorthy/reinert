#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_MANUAL_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_MANUAL_H_

#include "expressions/terminal/coord_expression.h"
#include "expressions/terminal/string_expression.h"
#include "analyze_expression.h"

#include "context.h"

#include <any>

// Alias for the specific VectorForge node type utilized in the application's graph
using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

/**
 * @class AnalyzeManualExpression
 * @brief An AST node that performs a vector database search using manually provided coordinates.
 * 
 * This class inherits from AnalyzeExpression to utilize profile management. When executed, 
 * it resolves its child coordinate expression and queries the Context's graph to find 
 * the nearest neighboring nodes.
 */
class AnalyzeManualExpression : public AnalyzeExpression {
    private:
        /**
         * @brief Pointer to the AST node containing the parsed vector coordinates.
         */
        CoordExpression* coord_expression_;

    public:
        /**
         * @brief Constructs a new AnalyzeManualExpression.
         * 
         * @param coords The CoordExpression holding the raw vector data to search with.
         */
        AnalyzeManualExpression(CoordExpression* coords);

        /**
         * @brief Destructor responsible for safely deleting the coordinate expression from memory.
         */
        ~AnalyzeManualExpression();

        /**
         * @brief Executes the manual analysis command.
         * 
         * Evaluates the stored coordinate expression, retrieves the relevant search profile, 
         * and queries the vector database for the nearest neighbors, formatting and printing 
         * the results to the standard output.
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& ctx) const override;
};

#endif