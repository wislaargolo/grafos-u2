#include <iostream>
#include <vector>
#include <iomanip> // Para formatação
#include <limits>   // Para std::numeric_limits

#include "../Prim.h"
#include "../utils/GraphIO.h"
#include "../graph/UndirectedAdjacencyListGraph.h" // Implementação de grafo não-direcionado

/**
 * @brief Função principal para testar o algoritmo de Prim.
 */
int main() {

    // Define o arquivo de dados a ser usado
    const std::string filename = "data/graph-prim.txt";
    std::cout << "\nCreating graph from file '" << filename << "'...\n";

    UndirectedAdjacencyListGraph<char> graph;
    std::vector<std::vector<double>> weights;

    populate_graph_weighted_from_file(filename, graph, weights, false);

    graph.print();
    print_weights_matrix(weights, graph);

    char start_node = 'a';

    std::cout << "Running Prim's Algorithm starting from node '" << start_node << "'...\n";

    auto result = prim(graph, weights, start_node);

    print_prim_result(result);

    return 0;
}
