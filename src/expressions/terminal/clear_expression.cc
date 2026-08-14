#include "expressions/expression.h"
#include "context.h"

#include "expressions/terminal/clear_expression.h"

#include <any>
#include <iostream>

std::any ClearExpression::Execute(Context& /*ctx*/) const {
    std::cout << "\033[2J\033[H" << std::flush;
    return {};
}