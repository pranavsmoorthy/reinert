#pragma once

#ifndef INCLUDE_NUMBER_COORD_EXPRESSION_H_
#define INCLUDE_NUMBER_COORD_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

/**
 * @class NumberExpression
 * @brief A terminal AST node that encapsulates a numeric value.
 * 
 * This class is used to store and evaluate raw double-precision numbers 
 * parsed from the user's input within the execution environment.
 */
class NumberExpression : public Expression {
    private:
        /**
         * @brief The double-precision floating-point value stored in this node.
         */
        double value_;
    
    public:
        /**
         * @brief Constructs a new NumberExpression.
         * 
         * @param i The numeric value to encapsulate within the AST node.
         */
        NumberExpression(double i);

        /**
         * @brief Executes the terminal expression.
         * 
         * Resolves this terminal node by returning its underlying numeric value, 
         * allowing parent non-terminal expressions (like AnalyzeTickerExpression or 
         * AddProfileExpression) to consume the raw number.
         * 
         * @param ctx The global memory and state context of the application (unused in this expression).
         * @return std::any A std::any object containing the double value.
         */
        std::any Execute(Context& /*ctx*/) const override;
};

#endif