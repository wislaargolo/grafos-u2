#include <iostream>
#include <iomanip>
#include <limits>

#include "../graph/DirectedAdjacencyMatrixGraph.h"
#include "../graph/UndirectedAdjacencyMatrixGraph.h"
#include "../graph/IGraph.h"
#include "../utils/GraphIO.h"
#include "../Boruvka.h"

template<typename Node>
void print_result(BoruvkaResult<Node>& result) {
  std::cout << "Final tree: \n";

  result.tree.print();

  std::cout << "Total weight: " << result.total_weight << "\n";
}

int main() {
    std::cout << "\nCreating graph from file 'graph-boruvka.txt'...\n";
    UndirectedAdjacencyListGraph<int> graph_und;
    std::vector<std::vector<double>> weights_und;
    populate_graph_weighted_from_file("data/graph-boruvka.txt", graph_und, weights_und, false);
    graph_und.print();
    print_weights_matrix(weights_und, graph_und);
    auto result_und = boruvka(graph_und, weights_und);
    print_result(result_und);

    return 0;
}
