#pragma once

#include "profile.h"

#include <unordered_map>
#include <string>
#include <vector>

// Forward declaration of Context if it is passed by reference in methods, 
// assuming it's included elsewhere or handled in the broader project scope.
class Context;

/**
 * @class ProfileMapper
 * @brief A utility class responsible for managing and storing vector search profiles.
 * 
 * The ProfileMapper maintains an in-memory dictionary of SearchProfile objects, 
 * allowing the application to quickly load, retrieve, add, and delete specific 
 * user-configured search parameters (like ef_search, ef_construction, and node limits).
 */
class ProfileMapper {
private:
    /**
     * @brief Internal hash map storing the relationship between profile names and their configurations.
     */
    std::unordered_map<std::string, SearchProfile> search_profiles_;

public:
    /**
     * @brief Loads all search profiles from a specified file or directory.
     * 
     * @param filepath The path to the directory or JSON file containing profile configurations.
     * @param ctx The global memory and state context of the application.
     * @return true If the profiles were successfully loaded into memory.
     * @return false If the file path was invalid or a parsing error occurred.
     */
    bool Load(const std::string& filepath, Context& ctx);

    /**
     * @brief Adds a new search profile to the internal map.
     * 
     * @param profile The SearchProfile object to be stored.
     */
    void AddProfile(const SearchProfile& profile);

    /**
     * @brief Retrieves a specific search profile by its name.
     * 
     * @param profile_name The string identifier for the desired profile.
     * @return const SearchProfile& A constant reference to the requested profile.
     * @throws std::out_of_range If the profile name does not exist in the map.
     */
    const SearchProfile& GetProfile(const std::string& profile_name) const;

    /**
     * @brief Retrieves a list of all currently loaded profile names.
     * 
     * @return const std::vector<std::string> A vector containing the names of all available profiles.
     */
    const std::vector<std::string> GetAllProfiles() const;

    /**
     * @brief Deletes a search profile from memory and disk.
     * 
     * @param profile_name The name of the profile to remove.
     * @param ctx The global memory and state context of the application.
     */
    void DeleteProfile(const std::string& profile_name, Context& ctx);
};