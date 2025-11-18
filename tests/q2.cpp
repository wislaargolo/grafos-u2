#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

#include "../Prim.h"
#include "../utils/GraphIO.h"
#include "../graph/UndirectedAdjacencyListGraph.h"

/**
 * @brief Função principal para testar o algoritmo de Prim.
 */
int main() {

    const std::string filename = "data/graph.txt";
    std::cout << "\nCreating graph from file '" << filename << "'...\n";

    UndirectedAdjacencyListGraph<int> graph;
    std::vector<std::vector<double>> weights;

    populate_graph_weighted_from_file(filename, graph, weights, false);

    graph.print();
    print_weights_matrix(weights, graph);

    int start_node = 1;

    std::cout << "Running Prim's Algorithm starting from node '" << start_node << "'...\n";

    auto result = prim(graph, weights, start_node);

    print_prim_result(result);

    return 0;
}
