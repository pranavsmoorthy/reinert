#include "expressions/nonterminal/configure_path.h"
#include "expressions/terminal/string_expression.h"

#include <iostream>

ConfigureExpression::ConfigureExpression(
    StringExpression* cik_map_path, 
    StringExpression* graph_model_path,
    StringExpression* search_profiles_path) {
        cik_map_path_expression_ = cik_map_path;
        graph_model_path_expression_ = graph_model_path;
        search_profiles_path_expression_ = search_profiles_path;
    }

ConfigureExpression::~ConfigureExpression() {
    delete cik_map_path_expression_;
    cik_map_path_expression_ = nullptr;

    delete graph_model_path_expression_;
    graph_model_path_expression_ = nullptr;

    delete search_profiles_path_expression_;
    search_profiles_path_expression_ = nullptr;
}

std::any ConfigureExpression::Execute(Context& ctx) const {
    ctx.app_config.cik_map_path = std::any_cast<std::string>(cik_map_path_expression_->Execute(ctx));
    ctx.app_config.graph_model_path = std::any_cast<std::string>(graph_model_path_expression_->Execute(ctx));
    ctx.app_config.search_profiles_path = std::any_cast<std::string>(search_profiles_path_expression_->Execute(ctx));

    ctx.updated_config = true;

    std::cout << "Run QUIT and reopen application to see changes" << std::endl;

    return {};
}