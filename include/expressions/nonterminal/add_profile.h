#pragma once

#ifndef INCLUDE_EXPRESSIONS_TERMINAL_ADD_PROFILE_H_
#define INCLUDE_EXPRESSIONS_TERMINAL_ADD_PROFILE_H_

#include "expressions/terminal/number_expression.h"
#include "expressions/terminal/string_expression.h"

#include "context.h"

#include <any>

/**
 * @class AddProfileExpression
 * @brief An AST node responsible for the creation and registration of a new search profile.
 * 
 * This non-terminal expression holds child terminal expressions representing the configuration
 * parameters for a new profile. Upon execution, it evaluates these children to construct, 
 * register, and save the resulting profile to the application's context.
 */
class AddProfileExpression : public Expression {
    private:
        // Pointers to the child AST nodes that hold the parsed arguments for the profile
        StringExpression* name_expression_;             // Evaluates to the unique identifier for the profile
        NumberExpression* nearest_clusters_expression_; // Evaluates to the number of clusters to search
        NumberExpression* nearest_node_expression_;     // Evaluates to the number of nearest nodes to retrieve
        NumberExpression* clusters_ef_expression_;      // Evaluates to the exploration factor (ef) for cluster search
        NumberExpression* node_ef_expression_;          // Evaluates to the exploration factor (ef) for node search

    public:
        /**
         * @brief Constructs a new AddProfileExpression.
         * 
         * @param name The StringExpression holding the profile's name.
         * @param nearest_clusters The NumberExpression holding the cluster search count.
         * @param nearest_nodes The NumberExpression holding the node retrieval count.
         * @param clusters_ef The NumberExpression holding the cluster exploration factor.
         * @param node_ef The NumberExpression holding the node exploration factor.
         */
        AddProfileExpression(StringExpression* name,
            NumberExpression* nearest_clusters,
            NumberExpression* nearest_nodes,
            NumberExpression* clusters_ef,
            NumberExpression* node_ef);

        /**
         * @brief Destructor responsible for safely deleting the child terminal expressions from memory.
         */
        ~AddProfileExpression();

        /**
         * @brief Executes the logic to add the profile.
         * 
         * This method resolves the stored terminal expressions into their underlying primitive values,
         * constructs the profile object, updates the Context's ProfileMapper, and serializes the new 
         * profile to the file system.
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& ctx) const override;
};

#endif