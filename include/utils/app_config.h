#pragma once

#ifndef INCLUDE_UTILS_APP_CONFIG_H_
#define INCLUDE_UTILS_APP_CONFIG_H_

#include <string>
#include <nlohmann/json.hpp>

struct AppConfig {
    std::string cik_map_path = "NOCONF";
    std::string graph_model_path = "NOCONF";
    std::string search_profiles_path = "NOCONF";

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AppConfig, cik_map_path, graph_model_path, search_profiles_path)

    bool FullyConfigured() {
        return (cik_map_path != "NOCONF") &&
               (graph_model_path != "NOCONF") &&
               (search_profiles_path != "NOCONF");
    }
};

#endif