#include "expressions/nonterminal/analyze_manual.h"
#include "expressions/terminal/coord_expression.h"
#include "expressions/terminal/string_expression.h"

#include "expressions/expression.h"
#include "context.h"
#include "profile.h"
#include "utils/formatter.h"

#include "vectorforge/node.h"
#include "vectorforge/graph.h"

#include <array>
#include <any>
#include <string>
#include <iostream>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

AnalyzeManualExpression::AnalyzeManualExpression(CoordExpression* coords) {
    coord_expression_ = coords;
}

AnalyzeManualExpression::~AnalyzeManualExpression() {
    delete coord_expression_;
    coord_expression_ = nullptr;
}

std::any AnalyzeManualExpression::Execute(Context& ctx) const {
    if (coord_expression_ == nullptr) {
        ThrowCannotExecuteCommand("Coords cannot be null");
    }

    std::array<double, 12> coords = std::any_cast<std::array<double, 12>>(coord_expression_ -> Execute(ctx));
    
    SearchProfile default_profile(ctx); 
    const SearchProfile* profile = nullptr;
    const StringExpression* profile_expression = GetProfile();

    if (profile_expression == nullptr) {
        profile = &default_profile;
    } else {
        std::string profile_name = std::any_cast<std::string>(profile_expression -> Execute(ctx));
        profile = &ctx.profile_map.GetProfile(profile_name);
    }

    std::vector<NodeType*> closest = ctx.graph.FindNearestKNodes(coords, *profile);
    std::cout << "Ticker     Date            Similarity     Deviation     Went Up?     Percent Change" << std::endl;

    for (NodeType* n : closest) {
        std::cout << Formatter::FormatNode(*n, coords) << std::endl;
    }

    return {};
}