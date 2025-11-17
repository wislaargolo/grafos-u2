#include <iostream>
#include <iomanip>
#include <limits>

#include "../graph/DirectedAdjacencyMatrixGraph.h"
#include "../graph/UndirectedAdjacencyMatrixGraph.h"
#include "../graph/IGraph.h"
#include "../utils/GraphIO.h"
#include "../FloydWarshall.h"

int main() {

    std::cout << "Creating graph from file 'digraph-floyd-warshall.txt'...\n";
    DirectedAdjacencyMatrixGraph<char> graph;
    std::vector<std::vector<double>> weights;
    populate_graph_weighted_from_file("data/digraph-floyd-warshall.txt", graph, weights);
    // populate_graph_from_file("data/digraph-floyd-warshall.txt", graph);
    graph.print();
    print_weights_matrix(weights, graph);
    auto results = floyd_warshall(graph, weights);
    print_floyd_warshall_result(results, graph);

    return 0;
}