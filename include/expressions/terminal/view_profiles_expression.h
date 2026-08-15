#pragma once

#ifndef INCLUDE_VIEW_PROFILES_EXPRESSION_H_
#define INCLUDE_VIEW_PROFILES_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

/**
 * @class ViewProfilesExpression
 * @brief An AST node responsible for displaying a list of all available search profiles.
 * 
 * This terminal expression takes no arguments. When executed, it queries the 
 * application's context to retrieve all loaded search profiles and prints 
 * their names to the standard output[cite: 21].
 */
class ViewProfilesExpression : public Expression {
    public:
        /**
         * @brief Executes the view profiles command.
         * 
         * Accesses the ProfileMapper within the global context to fetch the 
         * list of all configured search profiles and displays them to the user.
         * It does not modify any application state[cite: 21].
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& ctx) const override;
};

#endif