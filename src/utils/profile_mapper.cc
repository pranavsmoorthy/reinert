#include "utils/profile_mapper.h"
#include "context.h"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;

void ProfileMapper::AddProfile(const SearchProfile& profile) {
    auto it = search_profiles_.find(profile.name);

    if (it != search_profiles_.end()) {
        throw std::runtime_error("Profile already exists");
    }

    search_profiles_.insert({profile.name, profile});
}

bool ProfileMapper::Load(const std::string& folderpath, Context& ctx) {
    if (!fs::exists(folderpath) || !fs::is_directory(folderpath)) {
        std::cerr << "Profile directory does not exist: " << folderpath << std::endl;
        return false;
    }

    try {
        for (const auto& entry : fs::directory_iterator(folderpath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                std::ifstream file(entry.path());
                if (!file.is_open()) {
                    continue;
                }

                json j;
                file >> j;
                file.close();

                SearchProfile profile(ctx);
                j.get_to(profile);

                AddProfile(profile);
            }
        }
        return true;
    } catch (const json::exception& e) {
        std::cerr << "JSON Parsing Error in ProfileMapper: " << e.what() << std::endl;
        return false;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem Error in ProfileMapper: " << e.what() << std::endl;
        return false;
    }
}

const SearchProfile& ProfileMapper::GetProfile(const std::string& profile_name) const {
    auto it = search_profiles_.find(profile_name);

    if (it != search_profiles_.end()) {
        return it->second;
    }

    throw std::runtime_error("Profile not found in database: " + profile_name);
}

const std::vector<std::string> ProfileMapper::GetAllProfiles() {
    std::vector<std::string> profile_names;

    profile_names.reserve(search_profiles_.size()); 

    for (const auto& kv : search_profiles_) {
        profile_names.push_back(kv.first);
    }

    return profile_names;
}