#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_TICKER_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_TICKER_H_

#include "expressions/terminal/string_expression.h"
#include "expressions/terminal/number_expression.h"

#include "context.h"

#include <any>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

class AnalyzeTickerExpression : public Expression {
    private:
        StringExpression* ticker_expression_;
        StringExpression* profile_expression_;

        NumberExpression* consecutive_eps_beats_expression_;
        NumberExpression* avg_eps_surprise_expressions_;

    public:
        AnalyzeTickerExpression(
            StringExpression* ticker, 
            NumberExpression* consecutive_eps_beats, 
            NumberExpression* avg_eps_surprise, 
            StringExpression* profile = nullptr);
        ~AnalyzeTickerExpression();

        std::any Execute(Context& ctx) const override;
};

#endif