#include "expressions/expression.h"
#include "context.h"

#include "expressions/terminal/quit_expression.h"
#include "context.h"

#include <any>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

std::any QuitExpression::Execute(Context& ctx) const {
    if (ctx.updated_config) {
        nlohmann::json j = ctx.app_config;

    std::ofstream file("config.json");

        if (!file.is_open()) {
            std::cerr << "Error: Could not open config.json to save new configuration.\n";
        } else {
            file << j.dump(4);
            std::cout << "Successfully saved new configuration to config.json.\n";
        }
    }

    std::exit(EXIT_SUCCESS);

    return {};
}