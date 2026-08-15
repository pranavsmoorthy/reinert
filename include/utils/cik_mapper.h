#pragma once

#include <unordered_map>
#include <string>

class CIKMapper {
private:
    std::unordered_map<std::string, std::string> ticker_to_cik_;
    std::string PadCIK(const std::string& raw_cik) const;

public:
    bool Load(const std::string& filepath);
    std::string GetCIK(const std::string& ticker) const;
};