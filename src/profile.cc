#include "vectorforge/config/find_many_nodes_config.h"

#include "profile.h"
#include "context.h"

#include <stdexcept>

void to_json(nlohmann::json& j, const SearchProfile& profile) {
    j = nlohmann::json{
        {"name", profile.name},
        {"nearest_clusters", profile.nearest_clusters},
        {"nearest_nodes", profile.nearest_nodes},
        {"cluster_ef", profile.cluster_ef},
        {"node_ef", profile.node_ef}
    };
}

void from_json(const nlohmann::json& j, SearchProfile& profile) {
    try {
        j.at("nearest_clusters").get_to(profile.nearest_clusters);
        j.at("nearest_nodes").get_to(profile.nearest_nodes);
        j.at("cluster_ef").get_to(profile.cluster_ef);
        j.at("node_ef").get_to(profile.node_ef);
        j.at("name").get_to(profile.name);

        profile.Validate();
        profile.verified = true;
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Malformed SearchProfile JSON: " + std::string(e.what()));
    }
}

void SearchProfile::AdditionalValidate() const {
    if (!verified) {
        try {
            ctx.profile_map.GetProfile(name);
            throw std::runtime_error("Search Profile with given name " + name + " already exists");
        } catch (const std::runtime_error&) {}
    }
}