#include <iostream>
#include <iomanip>
#include <limits>

#include "../graph/DirectedAdjacencyMatrixGraph.h"
#include "../graph/UndirectedAdjacencyMatrixGraph.h"
#include "../utils/GraphIO.h"
#include "../BellmanFord.h"

int main() {
    std::cout << "Creating graph from file 'graph-bellman.txt'...\n";
    DirectedAdjacencyMatrixGraph<char> graph;
    std::vector<std::vector<double>> weights;
    populate_graph_weighted_from_file("data/graph-bellman.txt", graph, weights);
    graph.print();
    print_weights_matrix(weights, graph);
    auto result = bellman_ford(graph, weights, 'A');
    print_bellman_ford_result(result, graph);

    std::cout << "\nCreating graph from file 'graph-bellman-negative.txt'...\n";
    DirectedAdjacencyMatrixGraph<char> graph_neg;
    std::vector<std::vector<double>> weights_neg;
    populate_graph_weighted_from_file("data/graph-bellman-negative.txt", graph_neg, weights_neg);
    graph_neg.print();
    print_weights_matrix(weights_neg, graph_neg);
    auto result_neg = bellman_ford(graph_neg, weights_neg, 'A');
    print_bellman_ford_result(result_neg, graph_neg);
    

    std::cout << "\nCreating graph from file 'graph-bellman-und.txt'...\n";
    UndirectedAdjacencyMatrixGraph<int> graph_und;
    std::vector<std::vector<double>> weights_und;
    populate_graph_weighted_from_file("data/graph-bellman-und.txt", graph_und, weights_und, false);
    graph_und.print();
    print_weights_matrix(weights_und, graph_und);
    auto result_und = bellman_ford(graph_und, weights_und, 1);
    print_bellman_ford_result(result_und, graph_und);

    return 0;
}