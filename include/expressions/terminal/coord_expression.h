#pragma once

#ifndef INCLUDE_TERMINAL_COORD_EXPRESSION_H_
#define INCLUDE_TERMINAL_COORD_EXPRESSION_H_

#include "expression.h"
#include "context.h"

#include <any>
#include <array>

class CoordExpression : public Expression {
    private:
        std::array<double, 12> value_;
    
    public:
        CoordExpression(const std::array<double, 12>& a);
        std::any Execute(const Context& /*ctx*/) const override;
};

#endif