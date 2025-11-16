#include <iostream>
#include <iomanip>
#include <limits>

#include "../graph/DirectedAdjacencyListGraph.h"
#include "../graph/DirectedAdjacencyMatrixGraph.h"
#include "../graph/DirectedIncidenceMatrixGraph.h"
#include "../utils/GraphIO.h"
#include "../BellmanFord.h"

int main() {
    std::cout << "Creating directed graphs from file 'digraph.txt'...\n\n";
    DirectedAdjacencyMatrixGraph<int> graph_matrix;
    std::vector<std::vector<double>> weights;
    populate_graph_weighted_from_file("data/digraph.txt", graph_matrix, weights);
    graph_matrix.print();
    print_weights_matrix(weights, graph_matrix);
    auto result_matrix = bellman_ford(graph_matrix, weights, 1);
    print_bellman_ford_result(result_matrix, graph_matrix);

    DirectedAdjacencyListGraph<int> graph_list;
    populate_graph_weighted_from_file("data/digraph.txt", graph_list, weights);
    graph_list.print();
    print_weights_matrix(weights, graph_list);
    auto result_list = bellman_ford(graph_list, weights, 1);
    print_bellman_ford_result(result_list, graph_list);

    DirectedIncidenceMatrixGraph<int> graph_incidence;
    populate_graph_weighted_from_file("data/digraph.txt", graph_incidence, weights);
    graph_incidence.print();
    print_weights_matrix(weights, graph_incidence);
    auto result_incidence = bellman_ford(graph_incidence, weights, 1);
    print_bellman_ford_result(result_incidence, graph_incidence);

    return 0;
}