#pragma once

#include "expression.h"
#include "context.h"

#include <any>
#include <string>

class StringExpression : public Expression {
    private:
        std::string value_;
    
    public:
        StringExpression(const std::string& s);
        std::any Execute(const Context& /*ctx*/) const override;
};