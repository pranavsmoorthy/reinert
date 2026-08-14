#include "expressions/nonterminal/view_profile_expression.h"
#include "profile.h"

#include <iostream>

ViewProfileExpression::ViewProfileExpression(StringExpression* profile) {
    profile_expression_ = profile;
}

ViewProfileExpression::~ViewProfileExpression() {
    delete profile_expression_;
    profile_expression_ = nullptr;
}

std::any ViewProfileExpression::Execute(Context& ctx) const {
    auto it = ctx.search_profiles.find(std::any_cast<std::string>(profile_expression_ -> Execute(ctx))); 

    if (it != ctx.search_profiles.end()) {
        SearchProfile profile = it -> second;

        std::cout << "Name             : " << profile.name << std::endl;
        std::cout << "Nearest Clusters : " << profile.nearest_clusters << std::endl;
        std::cout << "Nearest Nodes    : " << profile.nearest_nodes << std::endl;
        std::cout << "Cluster EF       : " << profile.cluster_ef << std::endl;
        std::cout << "Node EF          : " << profile.node_ef << std::endl;
    } else {
        std::cerr << "Profile not found." << std::endl;
    }

    return {};
}