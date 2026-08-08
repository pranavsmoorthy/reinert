#include "../../include/expression.h"
#include "../../include/context.h"

#include <string>

class StringExpression : public Expression<std::string, 0> {
    private:
        std::string value_;
    
    public:
        StringExpression(std::string s) {
            value_ = s;
        }

        std::string Execute(const Context& ctx) const override {
            return value_;
        }
};