#pragma once

#ifndef INCLUDE_TERMINAL_COORD_EXPRESSION_H_
#define INCLUDE_TERMINAL_COORD_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>
#include <array>

/**
 * @class CoordExpression
 * @brief A terminal AST node that encapsulates a 12-dimensional coordinate vector.
 * 
 * This class is used to store and evaluate raw numerical coordinates, typically 
 * provided by the user for manual vector database queries within the execution environment.
 */
class CoordExpression : public Expression {
    private:
        /**
         * @brief The 12-element array representing the specific point in the vector space.
         */
        std::array<double, 12> value_;
    
    public:
        /**
         * @brief Constructs a new CoordExpression.
         * 
         * @param a The 12-dimensional array of double-precision coordinates to store.
         */
        CoordExpression(const std::array<double, 12>& a);

        /**
         * @brief Executes the terminal expression.
         * 
         * Resolves this terminal node by returning its underlying 12-dimensional array, 
         * allowing parent non-terminal expressions to use the raw coordinates.
         * 
         * @param ctx The global memory and state context of the application (unused in this expression).
         * @return std::any A std::any object containing the std::array<double, 12> value.
         */
        std::any Execute(Context& /*ctx*/) const override;
};

#endif