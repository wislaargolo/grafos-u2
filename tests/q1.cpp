#include <iostream>
#include <vector>
#include <iomanip>

#include "../Kruskal.h"
#include "../utils/GraphIO.h"
#include "../graph/UndirectedAdjacencyListGraph.h"

int main() {

    const std::string filename = "data/graph.txt";

    std::cout << "\nCreating graph from file '" << filename << "'...\n";

    UndirectedAdjacencyListGraph<char> graph;
    std::vector<std::vector<double>> weights;

    populate_graph_weighted_from_file(filename, graph, weights, false);

    graph.print();
    print_weights_matrix(weights, graph);

    std::cout << "Running Kruskal's Algorithm...\n";
    auto result = kruskal(graph, weights);

    print_kruskal_result(result);

    return 0;
}
