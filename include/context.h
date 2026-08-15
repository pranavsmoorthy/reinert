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

struct Context {
    vectorforge::graph::Graph<EarningsStruct, double, 12, 16> graph;
    ProfileMapper profile_map;
    CIKMapper cik_map;
    AppConfig app_config;

    bool updated_config = false;
};

#endif