#pragma once

#ifndef INCLUDE_TERMINAL_CLEAR_EXPRESSION_H_
#define INCLUDE_TERMINAL_CLEAR_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

/**
 * @class ClearExpression
 * @brief An AST node responsible for clearing the terminal screen.
 * 
 * This terminal expression takes no arguments. When executed, it issues a 
 * command to the console to clear the current terminal output, providing 
 * a clean workspace for the user.
 */
class ClearExpression : public Expression {
    public:
        /**
         * @brief Executes the clear command.
         * 
         * Clears the console/terminal screen. It does not modify the application's 
         * internal memory, database connections, or context.
         * 
         * @param ctx The global memory and state context of the application (unused in this expression).
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& /*ctx*/) const override;
};

#endif