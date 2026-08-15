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
    std::string profile_name = std::any_cast<std::string>(profile_expression_ -> Execute(ctx));
    SearchProfile profile = ctx.profile_map.GetProfile(profile_name);

    std::cout << "Name             : " << profile.name << std::endl;
    std::cout << "Nearest Clusters : " << profile.nearest_clusters << std::endl;
    std::cout << "Nearest Nodes    : " << profile.nearest_nodes << std::endl;
    std::cout << "Cluster EF       : " << profile.cluster_ef << std::endl;
    std::cout << "Node EF          : " << profile.node_ef << std::endl;

    return {};
}