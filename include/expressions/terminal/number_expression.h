#pragma once

#include "expression.h"
#include "context.h"

#include <any>

class NumberExpression : public Expression {
    private:
        double value_;
    
    public:
        NumberExpression(double i);
        std::any Execute(const Context& /*ctx*/) const override;
};