#pragma once

#ifndef INCLUDE_CONTEXT_H_
#define INCLUDE_CONTEXT_H_

#include "vectorforge/graph.h"
#include "vectorforge/config/find_many_nodes_config.h"

#include "../data/model_resources/earnings_struct.h"

#include "profile.h"

#include "utils/app_config.h"
#include "utils/cik_mapper.h"
#include "utils/profile_mapper.h"

#include <string>
#include <unordered_map>

/**
 * @struct Context
 * @brief The global application state and memory context.
 * 
 * This structure serves as the central hub for the application's runtime data. 
 * It holds the loaded vector database graph, the mapping dictionaries for profiles 
 * and CIKs, and the core application configuration paths, passing them sequentially 
 * to the execution nodes in the AST.
 */
struct Context {
    /**
     * @brief The active VectorForge graph containing the earnings vector database.
     */
    vectorforge::graph::Graph<EarningsStruct, double, 12, 16> graph;

    /**
     * @brief The mapper responsible for storing and retrieving configured search profiles.
     */
    ProfileMapper profile_map;

    /**
     * @brief The mapper responsible for resolving stock ticker symbols to their padded CIKs.
     */
    CIKMapper cik_map;

    /**
     * @brief The application configuration tracking the paths for necessary models and maps.
     */
    AppConfig app_config;

    /**
     * @brief State flag indicating if the configuration has been modified during the current session.
     */
    bool updated_config = false;
};

#endif