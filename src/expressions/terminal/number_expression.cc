#pragma once

#include "../../../include/expression.h"
#include "../../../include/context.h"

#include <any>

class NumberExpression : public Expression {
    private:
        double value_;
    
    public:
        NumberExpression(double i) {
            value_ = i;
        }

        std::any Execute(const Context& ctx) const override {
            return value_;
        }
};