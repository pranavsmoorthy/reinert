#include "expressions/terminal/string_expression.h"
#include "context.h"

#include <any>
#include <string>

StringExpression::StringExpression(const std::string& s) {
    value_ = s;
}

std::any StringExpression::Execute(Context& /*ctx*/) const {
    return value_;
}