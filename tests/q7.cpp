#include <iostream>
#include <iomanip>
#include <limits>

#include "../graph/DirectedAdjacencyMatrixGraph.h"
#include "../graph/UndirectedAdjacencyMatrixGraph.h"
#include "../graph/IGraph.h"
#include "../utils/GraphIO.h"
#include "../FloydWarshall.h"

int main() {

    // Teste do algoritmo de Floyd-Warshall, com exemplo do slide

    std::cout << "Creating graph from file 'digraph-floyd-warshall.txt'...\n";
    DirectedAdjacencyMatrixGraph<char> graph;
    std::vector<std::vector<double>> weights;
    populate_graph_weighted_from_file("data/digraph-floyd-warshall.txt", graph, weights);
    graph.print();
    print_weights_matrix(weights, graph);
    auto results = floyd_warshall(graph, weights);
    print_floyd_warshall_result(results, graph);

    // Teste do algoritmo de Floyd-Warshall, com exemplo passado no trabalho
    std::cout << "Creating graph from file 'digraph.txt'...\n";
    DirectedAdjacencyMatrixGraph<int> graph2;
    std::vector<std::vector<double>> weights2;
    populate_graph_weighted_from_file("data/digraph.txt", graph2, weights2);
    graph2.print();
    print_weights_matrix(weights2, graph2);
    auto results2 = floyd_warshall(graph2, weights2);
    print_floyd_warshall_result(results2, graph2);

    return 0;
}