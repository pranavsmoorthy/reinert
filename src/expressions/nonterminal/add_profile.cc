#include "expressions/nonterminal/add_profile.h"
#include "profile.h"

#include <string>
#include <array>
#include <cmath>
#include <stdexcept>
#include <iostream>

AddProfileExpression::AddProfileExpression(
    StringExpression* name,
    NumberExpression* nearest_clusters,
    NumberExpression* nearest_nodes,
    NumberExpression* clusters_ef,
    NumberExpression* node_ef) {
        name_expression_ = name;
        nearest_clusters_expression_ = nearest_clusters;
        nearest_node_expression_ = nearest_nodes;
        clusters_ef_expression_ = clusters_ef;
        node_ef_expression_ = node_ef;
    }
    
AddProfileExpression::~AddProfileExpression() {
    delete name_expression_;
    name_expression_ = nullptr;

    delete nearest_clusters_expression_;
    nearest_clusters_expression_ = nullptr;

    delete nearest_node_expression_;
    nearest_node_expression_ = nullptr;

    delete clusters_ef_expression_;
    clusters_ef_expression_ = nullptr;

    delete node_ef_expression_;
    node_ef_expression_ = nullptr;
}

std::any AddProfileExpression::Execute(Context& ctx) const {
    std::array<double, 4> vals;

    // 0 = nearest clusters
    // 1 = nearest nodes
    // 2 = cluster ef
    // 3 = node ef

    vals[0] = std::any_cast<double>(nearest_clusters_expression_-> Execute(ctx));
    vals[1] = std::any_cast<double>(nearest_node_expression_-> Execute(ctx));
    vals[2] = std::any_cast<double>(clusters_ef_expression_ -> Execute(ctx));
    vals[3] = std::any_cast<double>(node_ef_expression_-> Execute(ctx));

    try {
        SearchProfile sp(ctx);

        sp.name = std::any_cast<std::string>(name_expression_ -> Execute(ctx));
        sp.nearest_clusters = static_cast<std::size_t>(vals[0]);
        sp.nearest_nodes = static_cast<std::size_t>(vals[1]);
        sp.cluster_ef = static_cast<std::size_t>(vals[2]);
        sp.node_ef = static_cast<std::size_t>(vals[3]);
        sp.Validate();

        ctx.search_profiles.insert({sp.name, sp});
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return {};
}