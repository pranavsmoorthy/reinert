#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_VIEW_PROFILE_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_VIEW_PROFILE_H_

#include "expressions/terminal/string_expression.h"
#include "context.h"

#include <any>

class ViewProfileExpression : public Expression {
    private:
        StringExpression* profile_expression_;

    public:
        ViewProfileExpression(StringExpression* profile);
        ~ViewProfileExpression();

        std::any Execute(Context& ctx) const override;
};

#endif