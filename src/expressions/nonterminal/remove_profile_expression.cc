#include "expressions/nonterminal/remove_profile_expression.h"

RemoveProfileExpression::RemoveProfileExpression(StringExpression* name) {
    name_expression_ = name;
}

RemoveProfileExpression::~RemoveProfileExpression() {
    delete name_expression_;
    name_expression_ = nullptr;
}

std::any RemoveProfileExpression::Execute(Context& ctx) const {
    std::string name = std::any_cast<std::string>(name_expression_->Execute(ctx));
    ctx.profile_map.DeleteProfile(name, ctx);

    return {};
}