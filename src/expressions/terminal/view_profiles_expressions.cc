#include "expressions/terminal/view_profiles_expression.h"

#include <iostream>
#include <vector>
#include <string>

std::any ViewProfilesExpression::Execute(Context& ctx) const {
    std::vector<std::string> profiles = ctx.profile_map.GetAllProfiles();
    if (profiles.size() <= 0) {
        std::cout << "No profiles found" << std::endl;
    } else {
        for (std::string s : profiles) {
            std:: cout << s << std::endl;
        }
    }

    return {};
}