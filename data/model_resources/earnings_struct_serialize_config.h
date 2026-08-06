#ifndef DATA_EARNINGS_STRUCT_SERIALIZE_CONFIG_H_
#define DATA_EARNINGS_STRUCT_SERIALIZE_CONFIG_H_

#include "earnings_struct.h"
#include "vectorforge/config/serializer_config.h"

#include <sstream>
#include <vector>
#include <string>

struct EarningsStructSerializerConfig : public vectorforge::config::SerializerConfig<EarningsStruct> {
    std::string GenerateSerializationString(const EarningsStruct& in) const override {
        return in.ticker + "|" +
            in.date + "|" +
            std::to_string(in.move_pct) + "|" +
            ((in.label) ? "1" : "0");
    }

    EarningsStruct DeserializeString(const std::string& in) const override {
        std::size_t delim = in.find('|');
        if (delim == std::string::npos) {
            return EarningsStruct(); 
        }

        EarningsStruct es;
        std::vector<std::string> tokens;
    
        std::stringstream ss(in);
        std::string token;
        
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 4) return EarningsStruct();

        es.ticker = tokens[0];
        es.date = tokens[1];
        es.move_pct = std::stod(tokens[2]);
        es.label = (tokens[3] == "1");

        return es;
    }
};

#endif