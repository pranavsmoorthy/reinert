#pragma once

#ifndef INCLUDE_TERMINAL_CLEAR_EXPRESSION_H_
#define INCLUDE_TERMINAL_CLEAR_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

class ClearExpression : public Expression {
    public:
        std::any Execute(const Context& /*ctx*/) const override;
};

#endif