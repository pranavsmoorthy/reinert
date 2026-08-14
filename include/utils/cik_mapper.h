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

            for (auto& [key, obj] : j.items()) {
                
                if (!obj.is_object() || !obj.contains("ticker") || !obj.contains("cik_str")) {
                    continue;
                }

                std::string actual_ticker = obj["ticker"].get<std::string>();
                
                std::string raw_cik;
                
                if (obj["cik_str"].is_number()) {
                    raw_cik = std::to_string(obj["cik_str"].get<int>());
                } else if (obj["cik_str"].is_string()) {
                    raw_cik = obj["cik_str"].get<std::string>();
                }
                
                ticker_to_cik_[actual_ticker] = PadCIK(raw_cik);
            }
            return true;
        } catch (const json::exception& e) {
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