#include "expressions/expression.h"
#include "context.h"

#include "expressions/terminal/quit_expression.h"

#include <any>
#include <cstdlib>

std::any QuitExpression::Execute(const Context& /*ctx*/) const {
    std::exit(EXIT_SUCCESS);
    return {};
}