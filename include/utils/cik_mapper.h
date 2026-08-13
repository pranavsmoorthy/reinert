#pragma once

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

class CIKMapper {
private:
    std::unordered_map<std::string, std::string> ticker_to_cik_;

    std::string PadCIK(const std::string& raw_cik) const {
        std::string padded = raw_cik;
        while (padded.length() < 10) {
            padded = "0" + padded;
        }
        return padded;
    }

public:
    bool Load(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open CIK mapping file: " << filepath << std::endl;
            return false;
        }

        try {
            json j;
            file >> j; 

            // Iterate through the outer keys ("0", "1", "2"...)
            for (auto& [key, obj] : j.items()) {
                
                // Safety check: ensure the value is actually an object with our target keys
                if (!obj.is_object() || !obj.contains("ticker") || !obj.contains("cik_str")) {
                    continue;
                }

                // Extract the true ticker string from inside the object
                std::string actual_ticker = obj["ticker"].get<std::string>();
                
                std::string raw_cik;
                
                // Extract the CIK from inside the object
                if (obj["cik_str"].is_number()) {
                    raw_cik = std::to_string(obj["cik_str"].get<int>());
                } else if (obj["cik_str"].is_string()) {
                    raw_cik = obj["cik_str"].get<std::string>();
                }
                
                // Map the real ticker to the padded CIK
                ticker_to_cik_[actual_ticker] = PadCIK(raw_cik);
            }
            return true;
        } catch (const json::exception& e) {
            std::cerr << "Error parsing CIK JSON: " << e.what() << std::endl;
            return false;
        }
    }

    std::string GetCIK(const std::string& ticker) const {
        auto it = ticker_to_cik_.find(ticker);
        if (it != ticker_to_cik_.end()) {
            return it->second;
        }
        throw std::runtime_error("Ticker not found in CIK database: " + ticker);
    }
};