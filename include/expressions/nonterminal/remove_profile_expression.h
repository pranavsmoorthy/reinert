#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_REMOVE_PROFILE_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_REMOVE_PROFILE_H_

#include "expressions/terminal/number_expression.h"
#include "expressions/terminal/string_expression.h"

#include "context.h"

#include <any>

/**
 * @class RemoveProfileExpression
 * @brief An AST node responsible for deleting an existing search profile.
 * 
 * This class evaluates a provided string expression representing a profile's name, 
 * and subsequently removes that profile from the application's active state and file system.
 */
class RemoveProfileExpression : public Expression {
    private:
        /**
         * @brief Pointer to the AST node containing the target profile's name.
         */
        StringExpression* name_expression_;

    public:
        /**
         * @brief Constructs a new RemoveProfileExpression.
         * 
         * @param name The StringExpression holding the name of the profile to delete.
         */
        RemoveProfileExpression(StringExpression* name);

        /**
         * @brief Destructor responsible for safely deleting the child terminal expression from memory.
         */
        ~RemoveProfileExpression();

        /**
         * @brief Executes the profile removal command.
         * 
         * Resolves the stored string expression to retrieve the target profile name, 
         * then instructs the Context's ProfileMapper to erase the profile from memory 
         * and delete its associated JSON configuration file from the disk.
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& ctx) const override;
};

#endif