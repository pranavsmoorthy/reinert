#pragma once

#ifndef INCLUDE_PROFILE_H_
#define INCLUDE_PROFILE_H_

#include "vectorforge/config/find_many_nodes_config.h"

#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

struct Context;

struct SearchProfile : public vectorforge::config::FindManyNodesConfig {
    std::string name = "";
    Context& ctx;
    bool verified = false;

    SearchProfile(Context& context) : ctx(context) {}
    void AdditionalValidate() const override;
};

void to_json(nlohmann::json& j, const SearchProfile& profile);
void from_json(const nlohmann::json& j, SearchProfile& profile);

#endif