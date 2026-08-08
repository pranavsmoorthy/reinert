#include "../../include/expression.h"
#include "../../include/context.h"

class NumberExpression : public Expression<double, 0> {
    private:
        double value_;
    
    public:
        NumberExpression(double i) {
            value_ = i;
        }

        double Execute(const Context& ctx) const override {
            return value_;
        }
};