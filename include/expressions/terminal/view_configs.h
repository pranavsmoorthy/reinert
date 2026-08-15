#pragma once

#ifndef INCLUDE_VIEW_CONFIG_EXPRESSION_H_
#define INCLUDE_VIEW_CONFIG_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

/**
 * @class ViewConfigExpression
 * @brief An AST node responsible for displaying the current application configuration.
 * 
 * This terminal expression takes no arguments. When executed, it accesses the 
 * application's context to retrieve and print the current file paths configured 
 * for the CIK map, graph model, and search profiles directory.
 */
class ViewConfigExpression : public Expression {
    public:
        /**
         * @brief Executes the view configuration command.
         * 
         * Retrieves the AppConfig state from the global context and outputs the 
         * configured system paths to the standard output. It does not modify 
         * any application state.
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& ctx) const override;
};

#endif