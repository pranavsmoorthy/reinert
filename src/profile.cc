#include "vectorforge/config/find_many_nodes_config.h"

#include "profile.h"
#include "context.h"

#include <stdexcept>

void SearchProfile::AdditionalValidate() const {
    auto it = ctx.search_profiles.find(name);

    if (it != ctx.search_profiles.end()) {
        throw std::logic_error("Search Profile with given name already exists");
    }
}