#pragma once

#ifndef INCLUDE_TERMINAL_QUIT_EXPRESSION_H_
#define INCLUDE_TERMINAL_QUIT_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

class QuitExpression : public Expression {
    public:
        std::any Execute(Context& /*ctx*/) const override;
};

#endif