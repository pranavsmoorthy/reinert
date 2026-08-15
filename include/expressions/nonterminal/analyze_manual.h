#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_MANUAL_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_MANUAL_H_

#include "expressions/terminal/coord_expression.h"
#include "expressions/terminal/string_expression.h"
#include "analyze_expression.h"

#include "context.h"

#include <any>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

class AnalyzeManualExpression : public AnalyzeExpression {
    private:
        CoordExpression* coord_expression_;

    public:
        AnalyzeManualExpression(CoordExpression* coords);
        ~AnalyzeManualExpression();

        std::any Execute(Context& ctx) const override;
};

#endif