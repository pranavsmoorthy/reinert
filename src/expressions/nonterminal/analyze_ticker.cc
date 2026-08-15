#include "expressions/terminal/string_expression.h"
#include "expressions/terminal/number_expression.h"

#include "expressions/nonterminal/analyze_expression.h"
#include "expressions/nonterminal/analyze_ticker.h"

#include "utils/vector_builder.h"

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
    NumberExpression* avg_eps_surprise) {
        ticker_expression_ = ticker;
        consecutive_eps_beats_expression_ = consecutive_eps_beats;
        avg_eps_surprise_expressions_ = avg_eps_surprise;
}

AnalyzeTickerExpression::~AnalyzeTickerExpression() {
    delete ticker_expression_;
    ticker_expression_ = nullptr;

    delete consecutive_eps_beats_expression_;
    consecutive_eps_beats_expression_ = nullptr;

    delete avg_eps_surprise_expressions_;
    avg_eps_surprise_expressions_ = nullptr;
}

std::any AnalyzeTickerExpression::Execute(Context& ctx) const {
    if (ticker_expression_ == nullptr) {
        ThrowCannotExecuteCommand("Ticker cannot be null");
    }

    std::string ticker = std::any_cast<std::string>(ticker_expression_ -> Execute(ctx));
    std::string cik_num = ctx.cik_map.GetCIK(ticker);

    double consecutive_eps_beats = std::any_cast<double>(consecutive_eps_beats_expression_ -> Execute(ctx));
    double avg_eps_surprise = std::any_cast<double>(avg_eps_surprise_expressions_ -> Execute(ctx));

    VectorBuilder::SetJsonData(ticker, ctx);

    std::array<double, 12> coords = Preprocessor::TransformCoordinates(VectorBuilder::MakeCoords(consecutive_eps_beats, avg_eps_surprise));

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

    int went_up = 0;

    for (NodeType* n : closest) {
        std::cout << Formatter::FormatNode(*n, coords) << std::endl;

        if ((n->GetData()).move_pct > 0) {
            went_up++;
        }
    }

    std::cout << "Went Up: " << went_up << " out of " << profile->nearest_nodes << std::endl;

    return {};
}
