#pragma once

#ifndef INCLUDE_PROFILE_H_
#define INCLUDE_PROFILE_H_

#include "vectorforge/config/find_many_nodes_config.h"

#include <string>
#include <stdexcept>

struct Context;

struct SearchProfile : public vectorforge::config::FindManyNodesConfig {
    std::string name = "";
    const Context& ctx;

    SearchProfile(const Context& context) : ctx(context) {}
    void AdditionalValidate() const override;
};

#endif