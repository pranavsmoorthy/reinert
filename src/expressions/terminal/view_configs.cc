#include "expressions/terminal/view_configs.h"
#include <iostream>

std::any ViewConfigExpression::Execute(Context& ctx) const {
    std::cout << "CIK Map Path         : " << ctx.app_config.cik_map_path << std::endl;
    std::cout << "Model Path           : " << ctx.app_config.graph_model_path << std::endl;
    std::cout << "Search Profiles Path : " << ctx.app_config.search_profiles_path << std::endl;

    return {};
}