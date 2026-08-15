#pragma once

#include "profile.h"

#include <unordered_map>
#include <string>
#include <vector>

class ProfileMapper {
private:
    std::unordered_map<std::string, SearchProfile> search_profiles_;

public:
    bool Load(const std::string& filepath, Context& ctx);
    void AddProfile(const SearchProfile& profile);
    const SearchProfile& GetProfile(const std::string& profile_name) const;
    const std::vector<std::string> GetAllProfiles();
};