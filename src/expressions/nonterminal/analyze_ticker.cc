#include "expressions/terminal/string_expression.h"
#include "expressions/terminal/number_expression.h"

#include "expressions/nonterminal/analyze_ticker.h"

#include "context.h"
#include "interpreter/preprocessor.h"
#include "utils/formatter.h"
#include "utils/retriever.h"

#include <nlohmann/json.hpp>

#include <any>
#include <iostream>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

AnalyzeTickerExpression::AnalyzeTickerExpression(
    StringExpression* ticker, 
    NumberExpression* consecutive_eps_beats, 
    NumberExpression* avg_eps_surprise, 
    StringExpression* profile) {
        ticker_expression_ = ticker;
        profile_expression_ = profile;

        consecutive_eps_beats_expression_ = consecutive_eps_beats;
        avg_eps_surprise_expressions_ = avg_eps_surprise;
}

AnalyzeTickerExpression::~AnalyzeTickerExpression() {
    delete ticker_expression_;
    ticker_expression_ = nullptr;

    delete profile_expression_;
    profile_expression_ = nullptr;
}

std::any AnalyzeTickerExpression::Execute(const Context& ctx) const {
    if (ticker_expression_ == nullptr) {
        ThrowCannotExecuteCommand("Ticker cannot be null");
    }

    std::string ticker = std::any_cast<std::string>(ticker_expression_ -> Execute(ctx));
    std::string cik_num = ctx.cik_map.GetCIK(ticker);

    double consecutive_eps_beats = std::any_cast<double>(consecutive_eps_beats_expression_ -> Execute(ctx));
    double avg_eps_surprise = std::any_cast<double>(avg_eps_surprise_expressions_ -> Execute(ctx));

    VectorBuilder::SetJsonData(ticker, ctx);

    for (double d : VectorBuilder::MakeCoords(consecutive_eps_beats, avg_eps_surprise)) {
        std::cout << d << " ";
    }

    std::cout << std::endl;

    std::array<double, 12> coords = Preprocessor::TransformCoordinates(VectorBuilder::MakeCoords(consecutive_eps_beats, avg_eps_surprise));

    SearchProfile default_profile(ctx); 
    const SearchProfile* profile = nullptr;

    if (profile_expression_ == nullptr) {
        profile = &default_profile;
    } else {
        auto it = ctx.search_profiles.find(std::any_cast<std::string>(profile_expression_ -> Execute(ctx)));

        if (it == ctx.search_profiles.end()) {
            ThrowCannotExecuteCommand("Profile could not be found");
        } else {
            profile = &(it -> second);
        }
    }

    std::vector<NodeType*> closest = ctx.graph.FindNearestKNodes(coords, *profile);

    std::cout << "Ticker     Date            Similarity     Deviation     Went Up?     Percent Change" << std::endl;

    for (NodeType* n : closest) {
        std::cout << Formatter::FormatNode(*n, coords) << std::endl;
    }

    return {};
}
