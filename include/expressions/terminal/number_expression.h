#pragma once

#ifndef INCLUDE_NUMBER_COORD_EXPRESSION_H_
#define INCLUDE_NUMBER_COORD_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

class NumberExpression : public Expression {
    private:
        double value_;
    
    public:
        NumberExpression(double i);
        std::any Execute(const Context& /*ctx*/) const override;
};

#endif