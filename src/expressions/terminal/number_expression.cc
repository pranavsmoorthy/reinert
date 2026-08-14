#include "expressions/terminal/number_expression.h"
#include "context.h"

#include <any>

NumberExpression::NumberExpression(double i) {
    value_ = i;
}

std::any NumberExpression::Execute(Context& /*ctx*/) const {
    return value_;
}