#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_CONFIGURE_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_CONFIGURE_H_

#include "expressions/terminal/string_expression.h"
#include "context.h"

#include <any>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

class ConfigureExpression : public Expression {
    private:
        StringExpression* cik_map_path_expression_;
        StringExpression* graph_model_path_expression_;
        StringExpression* search_profiles_path_expression_;

    public:
        ConfigureExpression(
            StringExpression* cik_map_path, 
            StringExpression* graph_model_path,
            StringExpression* search_profiles_path);

        ~ConfigureExpression();

        std::any Execute(Context& ctx) const override;
};

#endif