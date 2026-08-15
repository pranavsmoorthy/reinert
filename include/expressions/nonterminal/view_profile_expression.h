#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_VIEW_PROFILE_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_VIEW_PROFILE_H_

#include "expressions/terminal/string_expression.h"
#include "context.h"

#include <any>

/**
 * @class ViewProfileExpression
 * @brief An AST node responsible for displaying the configuration details of a specific search profile.
 * 
 * This class evaluates a provided string expression representing a profile's name, 
 * retrieves the corresponding profile from the application's memory, and outputs 
 * its mathematical and configuration parameters to the terminal.
 */
class ViewProfileExpression : public Expression {
    private:
        /**
         * @brief Pointer to the AST node containing the target profile's name.
         */
        StringExpression* profile_expression_;

    public:
        /**
         * @brief Constructs a new ViewProfileExpression.
         * 
         * @param profile The StringExpression holding the name of the profile to view.
         */
        ViewProfileExpression(StringExpression* profile);

        /**
         * @brief Destructor responsible for safely deleting the child terminal expression from memory.
         */
        ~ViewProfileExpression();

        /**
         * @brief Executes the profile viewing command.
         * 
         * Resolves the stored string expression to retrieve the target profile name, 
         * queries the Context's ProfileMapper for the profile data, and prints the 
         * profile's configuration (such as ef values and node limits) to standard output.
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& ctx) const override;
};

#endif