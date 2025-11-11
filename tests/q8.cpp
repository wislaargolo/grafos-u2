#include <iostream>
#include <string>
#include <list>

#include "../Hierholzer.h"
#include "../graph/UndirectedAdjacencyMatrixGraph.h"
#include "../graph/UndirectedAdjacencyListGraph.h"
#include "../graph/UndirectedIncidenceMatrixGraph.h"
#include "../utils/GraphIO.h"

void get_hierholzer_result(IGraph<int>& graph, std::string filename) {

    std::cout << "Creating graph from file '" << filename << "'...\n";
    populate_graph_from_file(filename, graph);
    graph.print();

    std::cout << "Result of Hierholzer's Algorithm:\n";

    auto circuit = hierholzer_undirected(graph);

    std::cout << "Has Eulerian Cycle: " << (circuit.has_eulerian_cycle ? "Yes" : "No") << "\n";
    std::cout << "Has Eulerian Path: " << (circuit.has_eulerian_path ? "Yes" : "No") << "\n";
    std::cout << "Result: ";
    for (const auto& node : circuit.circuit) {
        std::cout << node << " ";
    }
    std::cout << "\n\n";
}

int main() {

    UndirectedAdjacencyMatrixGraph<int> graph_matrix;
    get_hierholzer_result(graph_matrix, "data/graph-cycle-8.txt");

    UndirectedAdjacencyListGraph<int> graph_list;
    get_hierholzer_result(graph_list, "data/graph-cycle-8.txt");

    UndirectedIncidenceMatrixGraph<int> graph_incidence;
    get_hierholzer_result(graph_incidence, "data/graph-cycle-8.txt");
    return 0;
}