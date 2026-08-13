#include "vectorforge/graph.h"
#include "vectorforge/serializer.h"
#include "vectorforge/node.h"

#include "vectorforge/config/find_many_nodes_config.h"

#include "../data/model_resources/earnings_struct.h"
#include "../data/model_resources/earnings_struct_serialize_config.h"

#include "context.h"

#include "interpreter/lexer.h"
#include "interpreter/parser.h"

#include "expressions/nonterminal/analyze_manual.h"

#include "utils/vector_builder.h"

#include <vector>
#include <array>
#include <iostream>
#include <stdexcept>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

int main() {
    vectorforge::serializer::Serializer<EarningsStruct, double, 12, 16> ser;
    EarningsStructSerializerConfig config;

    vectorforge::graph::Graph<EarningsStruct, double, 12, 16> model;
    ser.Load(model, "data/earnings_model.bin", config);

    VectorBuilder::SetSpyVixTnxData();

    Context ctx;
    ctx.graph = model;
    if (!ctx.cik_map.Load("data/dataset_resources/tickers.json")) {
        std::cerr << "Terminal failed to boot: CIK database missing." << std::endl;
        return 1;
    }

    std::string input = "";

    while (true) {
        std::cout << ">>  ";
        input = "";
        std::getline(std::cin >> std::ws, input);

        Lexer lex(input);
        Parser parse(lex.Tokenize());

        Expression* ast = nullptr;
        
        try {
            Expression* ast = parse.Parse();
            if (ast != nullptr) {
                ast -> Execute(ctx);
            }
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }

        if (ast != nullptr) {
            delete ast;
            ast = nullptr;
        }
    }

    return 0;
}