#pragma once

#ifndef INCLUDE_STRING_COORD_EXPRESSION_H_
#define INCLUDE_STRING_COORD_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>
#include <string>

/**
 * @class StringExpression
 * @brief A terminal AST node that encapsulates a string value.
 * 
 * This class is used to store and evaluate raw string literals parsed from 
 * the user's input within the execution environment, such as file paths, 
 * profile names, or stock tickers.
 */
class StringExpression : public Expression {
    private:
        /**
         * @brief The string value stored in this node.
         */
        std::string value_;
    
    public:
        /**
         * @brief Constructs a new StringExpression.
         * 
         * @param s The string value to encapsulate within the AST node.
         */
        StringExpression(const std::string& s);

        /**
         * @brief Executes the terminal expression.
         * 
         * Resolves this terminal node by returning its underlying string value, 
         * allowing parent non-terminal expressions (like ConfigureExpression or 
         * AnalyzeTickerExpression) to consume the raw text.
         * 
         * @param ctx The global memory and state context of the application (unused in this expression).
         * @return std::any A std::any object containing the std::string value.
         */
        std::any Execute(Context& /*ctx*/) const override;
};

#endif