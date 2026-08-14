#include "expressions/terminal/view_profiles_expression.h"

#include <iostream>
#include <vector>

std::any ViewProfilesExpression::Execute(Context& ctx) const {
    if (ctx.search_profiles.size() <= 0) {
        std::cout << "No profiles found" << std::endl;
    } else {
        for (const auto& kv : ctx.search_profiles) {
            std:: cout << kv.first << std::endl;
        }
    }

    return {};
}