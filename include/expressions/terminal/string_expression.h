#pragma once

#ifndef INCLUDE_STRING_COORD_EXPRESSION_H_
#define INCLUDE_STRING_COORD_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>
#include <string>

class StringExpression : public Expression {
    private:
        std::string value_;
    
    public:
        StringExpression(const std::string& s);
        std::any Execute(Context& /*ctx*/) const override;
};

#endif