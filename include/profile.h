#pragma once

#ifndef INCLUDE_PROFILE_H_
#define INCLUDE_PROFILE_H_

#include "vectorforge/config/find_many_nodes_config.h"

#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

struct Context;

/**
 * @struct SearchProfile
 * @brief Encapsulates the configuration parameters for a vector database search.
 * 
 * This structure extends VectorForge's baseline FindManyNodesConfig to include 
 * application-specific metadata, such as a profile name and a reference to the 
 * global execution context.
 */
struct SearchProfile : public vectorforge::config::FindManyNodesConfig {
    /**
     * @brief The unique string identifier for this search profile.
     */
    std::string name = "";

    /**
     * @brief Reference to the global application state and memory context.
     */
    Context& ctx;

    /**
     * @brief Flag indicating whether the profile has been successfully validated.
     */
    bool verified = false;

    /**
     * @brief Constructs a new SearchProfile.
     * 
     * @param context The global memory and state context of the application.
     */
    SearchProfile(Context& context) : ctx(context) {}

    /**
     * @brief Performs additional application-specific validation checks.
     * 
     * Overrides the base class validation to enforce any custom constraints 
     * required by the application before executing a vector search operation.
     */
    void AdditionalValidate() const override;
};

/**
 * @brief Serializes a SearchProfile object into a JSON representation.
 * 
 * @param j The target nlohmann::json object to populate.
 * @param profile The SearchProfile instance to serialize.
 */
void to_json(nlohmann::json& j, const SearchProfile& profile);

/**
 * @brief Deserializes a JSON object into a SearchProfile instance.
 * 
 * @param j The source nlohmann::json object to read from.
 * @param profile The target SearchProfile instance to populate.
 */
void from_json(const nlohmann::json& j, SearchProfile& profile);

#endif