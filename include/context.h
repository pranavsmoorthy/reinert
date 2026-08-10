#pragma once

#ifndef INCLUDE_CONTEXT_H_
#define INCLUDE_CONTEXT_H_

#include "vectorforge/graph.h"
#include "vectorforge/config/find_many_nodes_config.h"

#include "../data/model_resources/earnings_struct.h"
#include "profile.h"

#include <string>
#include <unordered_map>

struct Context {
    vectorforge::graph::Graph<EarningsStruct, double, 12, 16> graph;
    std::unordered_map<std::string, SearchProfile> search_profiles;
};

#endif