#pragma once

#ifndef INCLUDE_EXPRESSIONS_TERMINAL_REMOVE_PROFILE_H_
#define INCLUDE_EXPRESSIONS_TERMINAL_REMOVE_PROFILE_H_

#include "expressions/terminal/number_expression.h"
#include "expressions/terminal/string_expression.h"

#include "context.h"

#include <any>

class RemoveProfileExpression : public Expression {
    private:
        StringExpression* name_expression_;

    public:
        RemoveProfileExpression(StringExpression* name);
        ~RemoveProfileExpression();

        std::any Execute(Context& ctx) const override;
};

#endif