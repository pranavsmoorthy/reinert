#include "expressions/expression.h"
#include "context.h"
#include "expressions/terminal/coord_expression.h"

#include <any>
#include <array>
    
CoordExpression::CoordExpression(const std::array<double, 12>& a) {
    value_ = a;
}

std::any CoordExpression::Execute(const Context& /*ctx*/) const {
    return value_;
}