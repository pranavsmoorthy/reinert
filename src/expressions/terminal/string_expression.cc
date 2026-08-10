#pragma once

#include "../../../include/expression.h"
#include "../../../include/context.h"

#include <any>
#include <string>

class StringExpression : public Expression {
    private:
        std::string value_;
    
    public:
        StringExpression(const std::string& s) {
            value_ = s;
        }

        std::any Execute(const Context& ctx) const override {
            return value_;
        }
};