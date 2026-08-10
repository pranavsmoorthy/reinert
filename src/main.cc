#include "vectorforge/graph.h"
#include "vectorforge/serializer.h"
#include "vectorforge/node.h"

#include "vectorforge/config/find_many_nodes_config.h"

#include "../data/model_resources/earnings_struct.h"
#include "../data/model_resources/earnings_struct_serialize_config.h"

#include "context.h"
#include "formatter.h"

#include "expressions/nonterminal/analyze_manual.h"

#include <vector>
#include <array>
#include <iostream>

using NodeType = vectorforge::node::Node<EarningsStruct, double, 12, 16>;

int main() {
    vectorforge::serializer::Serializer<EarningsStruct, double, 12, 16> ser;
    EarningsStructSerializerConfig config;

    vectorforge::graph::Graph<EarningsStruct, double, 12, 16> model;
    ser.Load(model, "data/earnings_model.bin", config);

    Context ctx;
    ctx.graph = model;

    vectorforge::config::FindManyNodesConfig default_config;
    std::array<double, 12> coords = {
        0.15894109112297136,0.5602602689000122,0.22320469052531317,0.4602780726811463,1.0,0.29413482328662344,0.27283334788156455,7.69359936547264e-18,0.4870065208313257,0.5812886707384382,0.08821350275979944,0.6831275678442332
    };

    CoordExpression* c_ex = new CoordExpression(coords);
    AnalyzeManualExpression am_ex(c_ex);

    am_ex.Execute(ctx);

    return 0;
}