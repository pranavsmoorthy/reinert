#pragma once

#ifndef INCLUDE_EXPRESSIONS_TERMINAL_ADD_PROFILE_H_
#define INCLUDE_EXPRESSIONS_TERMINAL_ADD_PROFILE_H_

#include "expressions/terminal/number_expression.h"
#include "expressions/terminal/string_expression.h"

#include "context.h"

#include <any>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

class AddProfileExpression : public Expression {
    private:
        StringExpression* name_expression_;
        NumberExpression* nearest_clusters_expression_;
        NumberExpression* nearest_node_expression_;
        NumberExpression* clusters_ef_expression_;
        NumberExpression* node_ef_expression_;

    public:
        AddProfileExpression(StringExpression* name,
            NumberExpression* nearest_clusters,
            NumberExpression* nearest_nodes,
            NumberExpression* clusters_ef,
            NumberExpression* node_ef);

        ~AddProfileExpression();

        std::any Execute(Context& ctx) const override;
};

#endif