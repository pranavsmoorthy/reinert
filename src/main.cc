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

#include "utils/app_config.h"
#include "utils/vector_builder.h"

#include <vector>
#include <array>
#include <iostream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

int main(int argc, char* argv[]) {
    std::cout << "\033[2J\033[H" << std::flush;
    std::cout << "Welcome to reinert!" << std::endl;

    std::filesystem::path exe_path = std::filesystem::absolute(argv[0]);
    std::filesystem::path project_root = exe_path.parent_path();
    std::filesystem::current_path(project_root);

    AppConfig app_config;
    std::string config_file = "config.json";

    if (std::filesystem::exists(config_file)) {
        try {
            std::ifstream file(config_file);
            nlohmann::json j;
            file >> j;
            app_config = j.get<AppConfig>(); 
        } catch (const std::exception& e) {
            std::cerr << "Warning: config.json is corrupted. Using defaults.\n";
        }
    } else {
        std::cout << "No config.json found. Generating default configuration...\n";
        std::ofstream file(config_file);
        nlohmann::json j = app_config; 
        file << j.dump(4);         
    }

    Context ctx;
    ctx.app_config = app_config;

    bool model_loaded = false;
    bool cik_loaded = false;
    bool profiles_loaded = false;

    if (app_config.FullyConfigured()) {
        try {
            vectorforge::serializer::Serializer<EarningsStruct, double, 12, 16> ser;
            EarningsStructSerializerConfig config;

            vectorforge::graph::Graph<EarningsStruct, double, 12, 16> model;

            try {
                ser.Load(model, app_config.graph_model_path, config);
                ctx.graph = model;
                model_loaded = true;
            } catch (const std::runtime_error&) {
                std::cerr << "Terminal failed to boot: Could not load model. Try reconfiguring the model path." << std::endl;
            }
            
            if (!ctx.cik_map.Load(app_config.cik_map_path)) {
                std::cerr << "Terminal failed to boot: CIK database missing. Try reconfiguring the CIK path." << std::endl;
            } else {
                cik_loaded = true;
            }

            if (!ctx.profile_map.Load(app_config.search_profiles_path, ctx)) {
                std::cerr << "Terminal failed to boot: Profile folder missing. Try reconfiguring the Profile path." << std::endl;
            } else {
                profiles_loaded = true;
            }
        } catch (const std::runtime_error&) {
            std::cerr << "Path linked to wrong file/folder. Sending to configure mode." << std::endl;
        }
    }

    if (model_loaded && cik_loaded && profiles_loaded) {
        std::string input = "";
        VectorBuilder::SetSpyVixTnxData();

        while (true) {
            std::cout << ">>  ";
            input = "";
            std::getline(std::cin >> std::ws, input);

            Lexer* lex = new Lexer(input);
            Parser* parse = new Parser(lex -> Tokenize());

            Expression* ast = nullptr;
            
            try {
                Expression* ast = parse -> Parse();

                if (ast != nullptr) {
                    ast -> Run(ctx);
                } else {
                    std::cout << "Couldn't find expression" << std::endl;
                }
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }

            if (ast != nullptr) {
                delete ast;
                ast = nullptr;
            }

            delete lex;
            lex = nullptr;

            delete parse;
            parse = nullptr;
        }
    } else {
        std::cerr << "Some paths remain unconfigured or improperly set. Run CONFIGURE to set these paths" << std::endl;
        std::cerr << "Make sure the profiles path is an EMPTY FOLDER" << std::endl;
        std::cerr << "Can only run CONFIGURE, CLEAR, and QUIT" << std::endl;
        std::cerr << "Run QUIT after configuring" << std::endl;
        std::string input = "";

        while (true) {
            std::cout << "[CONFIGURE MODE] >>  ";
            input = "";
            std::getline(std::cin >> std::ws, input);

            Lexer* lex = new Lexer(input);
            Parser* parse = new Parser(lex -> Tokenize(), true);

            Expression* ast = nullptr;
            
            try {
                Expression* ast = parse -> Parse();
                if (ast != nullptr) {
                    ast -> Run(ctx);
                }
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }

            if (ast != nullptr) {
                delete ast;
                ast = nullptr;
            }

            delete lex;
            lex = nullptr;

            delete parse;
            parse = nullptr;
        }
    }

    return 0;
}