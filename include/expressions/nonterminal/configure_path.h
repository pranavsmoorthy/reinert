#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_CONFIGURE_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_CONFIGURE_H_

#include "expressions/terminal/string_expression.h"
#include "context.h"

#include <any>

// Alias for the specific VectorForge node type utilized in the application's graph
using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

/**
 * @class ConfigureExpression
 * @brief An AST node responsible for updating the application's core configuration paths.
 * 
 * This class evaluates provided string expressions representing file paths and updates 
 * the application's global configuration state (e.g., CIK map, graph model, and search profiles).
 */
class ConfigureExpression : public Expression {
    private:
        /**
         * @brief Pointer to the AST node containing the CIK map JSON file path.
         */
        StringExpression* cik_map_path_expression_;
        
        /**
         * @brief Pointer to the AST node containing the VectorForge graph model binary path.
         */
        StringExpression* graph_model_path_expression_;
        
        /**
         * @brief Pointer to the AST node containing the directory path for search profiles.
         */
        StringExpression* search_profiles_path_expression_;

    public:
        /**
         * @brief Constructs a new ConfigureExpression.
         * 
         * @param cik_map_path The StringExpression holding the CIK map file path.
         * @param graph_model_path The StringExpression holding the graph model file path.
         * @param search_profiles_path The StringExpression holding the search profiles directory path.
         */
        ConfigureExpression(
            StringExpression* cik_map_path, 
            StringExpression* graph_model_path,
            StringExpression* search_profiles_path);

        /**
         * @brief Destructor responsible for safely deleting the child terminal expressions from memory.
         */
        ~ConfigureExpression();

        /**
         * @brief Executes the configuration command.
         * 
         * Resolves the stored string expressions to update the Context's AppConfig, 
         * marks the configuration as updated, and ensures the new paths are applied 
         * to the application state.
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An empty std::any object upon successful completion.
         */
        std::any Execute(Context& ctx) const override;
};

#endif