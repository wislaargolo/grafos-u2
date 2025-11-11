#include <iostream>
#include <list>
#include <string>

#include "../Hierholzer.h"
#include "../graph/DirectedAdjacencyMatrixGraph.h"
#include "../graph/DirectedAdjacencyListGraph.h"
#include "../graph/DirectedIncidenceMatrixGraph.h"
#include "../utils/GraphIO.h"

void get_hierholzer_result(IGraph<char>& graph, std::string filename) {

    std::cout << "Creating graph from file '" << filename << "'...\n";
    populate_graph_from_file(filename, graph);
    graph.print();

    std::cout << "Result of Hierholzer's Algorithm:\n";

    auto circuit = hierholzer_directed(graph);

    std::cout << "Has Eulerian Cycle: " << (circuit.has_eulerian_cycle ? "Yes" : "No") << "\n";
    std::cout << "Has Eulerian Path: " << (circuit.has_eulerian_path ? "Yes" : "No") << "\n";
    std::cout << "Result: ";
    for (const auto& node : circuit.circuit) {
        std::cout << node << " ";
    }
    std::cout << "\n\n";
}

int main() {

    DirectedAdjacencyMatrixGraph<char> graph_matrix;
    get_hierholzer_result(graph_matrix, "data/graph-path-9.txt");

    DirectedAdjacencyListGraph<char> graph_list;
    get_hierholzer_result(graph_list, "data/graph-path-9.txt");

    DirectedIncidenceMatrixGraph<char> graph_incidence;
    get_hierholzer_result(graph_incidence, "data/graph-path-9.txt");
    return 0;
}