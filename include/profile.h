#ifndef INCLUDE_PROFILE_H_
#define INCLUDE_PROFILE_H_

#include "vectorforge/config/find_many_nodes_config.h"
#include "context.h"

#include <string>
#include <stdexcept>

struct SearchProfile : public vectorforge::config::FindManyNodesConfig {
    std::string name = "";
    Context& ctx;

    void AdditionalValidate() const override {
        auto it = ctx.search_profiles.find(name);

        if (it != ctx.search_profiles.end()) {
            throw std::logic_error("Search Profile with given name already exists");
        }
    }
};

#endif