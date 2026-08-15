#pragma once

#ifndef INCLUDE_UTILS_APP_CONFIG_H_
#define INCLUDE_UTILS_APP_CONFIG_H_

#include <string>
#include <nlohmann/json.hpp>

/**
 * @struct AppConfig
 * @brief Encapsulates the core file path configurations required by the application.
 * 
 * This structure holds the essential paths needed for the application to function 
 * correctly, including data mappings, the vector database model, and user profiles. 
 * It utilizes the nlohmann/json library for seamless serialization and deserialization.
 */
struct AppConfig {
    /**
     * @brief The file path to the CIK (Central Index Key) mapping JSON file.
     */
    std::string cik_map_path = "NOCONF";

    /**
     * @brief The file path to the compiled VectorForge graph model binary.
     */
    std::string graph_model_path = "NOCONF";

    /**
     * @brief The directory path where search profile JSON configurations are stored.
     */
    std::string search_profiles_path = "NOCONF";

    /**
     * @brief Macro that defines intrusive serialization/deserialization logic for nlohmann::json.
     * 
     * Allows the AppConfig struct to be easily converted to and from a JSON object.
     */
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AppConfig, cik_map_path, graph_model_path, search_profiles_path)

    /**
     * @brief Validates whether all required configuration paths have been established.
     * 
     * Checks the internal path variables against the default "NOCONF" state to ensure 
     * the user has completed the setup process before allowing standard application execution.
     * 
     * @return true If all paths have been successfully updated from their default state.
     * @return false If one or more paths are still set to "NOCONF".
     */
    bool FullyConfigured() const {
        return (cik_map_path != "NOCONF") &&
               (graph_model_path != "NOCONF") &&
               (search_profiles_path != "NOCONF");
    }
};

#endif