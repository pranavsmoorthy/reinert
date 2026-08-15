#pragma once

#ifndef INCLUDE_TERMINAL_QUIT_EXPRESSION_H_
#define INCLUDE_TERMINAL_QUIT_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

/**
 * @class QuitExpression
 * @brief An AST node responsible for terminating the application.
 * 
 * This terminal expression takes no arguments. When executed, it signals the 
 * application's main loop or interpreter to shut down and exit gracefully.
 */
class QuitExpression : public Expression {
    public:
        /**
         * @brief Executes the quit command.
         * 
         * Triggers the termination sequence for the application's execution environment.
         * 
         * @param ctx The global memory and state context of the application (unused in this expression).
         * @return std::any An empty std::any object (though the process typically halts before this is consumed).
         */
        std::any Execute(Context& /*ctx*/) const override;
};

#endif