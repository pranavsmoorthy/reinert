#pragma once

#include "../../../include/expression.h"
#include "../../../include/context.h"

#include <any>
#include <array>

class CoordExpression : public Expression {
    private:
        std::array<double, 12> value_;
    
    public:
        CoordExpression(const std::array<double, 12>& a) {
            value_ = a;
        }

        std::any Execute(const Context& /*ctx*/) const override {
            return value_;
        }
};