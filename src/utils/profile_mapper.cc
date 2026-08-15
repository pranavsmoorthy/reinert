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

const std::vector<std::string> ProfileMapper::GetAllProfiles() const {
    std::vector<std::string> profile_names;

    profile_names.reserve(search_profiles_.size()); 

    for (const auto& kv : search_profiles_) {
        profile_names.push_back(kv.first);
    }

    return profile_names;
}

void ProfileMapper::DeleteProfile(const std::string& profile_name, Context& ctx) {
    auto it = search_profiles_.find(profile_name);
    if (it == search_profiles_.end()) {
        std::cerr << "Profile '" << profile_name << "' not found in memory." << std::endl;
    }

    search_profiles_.erase(it);

    try {
        fs::path filepath = fs::path(ctx.app_config.search_profiles_path) / (profile_name + ".json");
        
        if (fs::exists(filepath)) {
            fs::remove(filepath);
        } else {
            std::cerr << "Profile removed from memory, but file " 
                      << filepath << " was not found on disk." << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem Error while deleting profile: " << e.what() << std::endl;
    }
}