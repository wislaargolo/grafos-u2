#include <iostream>
#include <iomanip>
#include <limits>

#include "../graph/DirectedAdjacencyMatrixGraph.h"
#include "../graph/UndirectedAdjacencyMatrixGraph.h"
#include "../graph/IGraph.h"
#include "../utils/GraphIO.h"
#include "../Djikstra.h"

template<typename Node>
void print_result(const IGraph<Node>& graph, DjikstraResult& result) {
    std::cout << "Shortest distances from the start node:\n";

    // Tamanho fixo para alinhamento
    const int label_width = 12;
    const int data_width = 6;

    // Cabeçalho da tabela
    std::cout << std::left << std::setw(label_width) << "Nodes" << std::right << " | ";
    for(size_t i = 0; i < graph.get_order(); ++i) {
        std::cout << std::setw(data_width) << graph.get_node(i);
    }
    std::cout << "\n";

    // Linha separadora
    std::cout << std::setfill('-') << std::setw(label_width) << "" << "---";
    for(size_t i = 0; i < graph.get_order(); ++i) {
        std::cout << std::setw(data_width) << "";
    }

    std::cout << std::setfill(' ') << "\n";

    // Linha das distâncias
    std::cout << std::left << std::setw(label_width) << "Distances" << std::right << " | ";
    for(size_t i = 0; i < graph.get_order(); ++i) {
        if(result.distances[i] == std::numeric_limits<double>::infinity()) {
            std::cout << std::setw(data_width) << "INF";
        } else {
            std::cout << std::setw(data_width) << std::setprecision(2) << std::fixed << result.distances[i];
        }
    }

    std::cout << "\n";

    // Linha dos predecessores
    std::cout << std::left << std::setw(label_width) << "Predecessors" << std::right << " | ";
    for(size_t i = 0; i < graph.get_order(); ++i) {
        if(result.predecessors[i] == -1) {
            std::cout << std::setw(data_width) << "NULL";
        } else {
            std::cout << std::setw(data_width) << graph.get_node(result.predecessors[i]);
        }
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Creating graph from file 'graph-djikstra.txt'...\n";
    DirectedAdjacencyListGraph<char> graph;
    std::vector<std::vector<double>> weights;
    populate_graph_weighted_from_file("data/graph-djikstra.txt", graph, weights);
    graph.print();
    print_weights_matrix(weights, graph);
    auto result = djikstra(graph, weights, 'A');
    print_result(graph, result);


    std::cout << "\nCreating graph from file 'graph-djikstra-und.txt'...\n";
    UndirectedAdjacencyListGraph<int> graph_und;
    std::vector<std::vector<double>> weights_und;
    populate_graph_weighted_from_file("data/graph-djikstra-und.txt", graph_und, weights_und, false);
    graph_und.print();
    print_weights_matrix(weights_und, graph_und);
    auto result_und = djikstra(graph_und, weights_und, 1);
    print_result(graph_und, result_und);

    return 0;
}
