#include "earnings_struct.h"
#include "earnings_struct_serialize_config.h"

#include "interpreter/preprocessor.h"

#include "vectorforge/graph.h"
#include "vectorforge/serializer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
#include <string>

int main() {
    std::ifstream file("data/vectorforge_12d_dataset_raw.csv");

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    vectorforge::graph::Graph<EarningsStruct, double, 12, 16> earnings_model;
    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        // Ticker  Earnings_Date  Move_Pct  Went_Up  Dim0  Dim1  Dim2  Dim3  Dim4  Dim5  Dim6  Dim7  Dim8  Dim9  Dim10  Dim11
        // 0.      1              2.        3        4     5.    6.    7.    8.    9.    10.   11    12.   13.   14.    15
        EarningsStruct es;
        std::array<double, 12> coordinates;

        es.ticker   = row[0];
        es.date     = row[1];
        es.move_pct = std::stod(row[2]);
        es.label    = row[3] == "1";

        for (int i = 0; i < 12; i++) {
            coordinates[i] = std::stod(row[i + 4]);
            std::cout << coordinates[i] << " ";
        }

        std::cout << std::endl;

        std::array<double, 12> coordinates_recalculated = Preprocessor::TransformCoordinates(coordinates);
        earnings_model.AddNode(es, coordinates_recalculated);
    }

    EarningsStructSerializerConfig config;
    vectorforge::serializer::Serializer<EarningsStruct, double, 12, 16> ser;
    ser.Save(earnings_model, "data/earnings_model.bin", config);

    return 0;
}