#include <iostream>
#include <string>
#include <list>

#include "../Hierholzer.h"
#include "../graph/UndirectedAdjacencyMatrixGraph.h"
#include "../graph/UndirectedAdjacencyListGraph.h"
#include "../graph/UndirectedIncidenceMatrixGraph.h"
#include "../utils/GraphIO.h"
#include <iostream>

int main() {

    std::cout << "Creating graphs from file 'graph-cycle-8.txt'...\n\n";
    UndirectedAdjacencyMatrixGraph<int> graph_matrix;
    populate_graph_from_file("data/graph-cycle-8.txt", graph_matrix);
    graph_matrix.print();
    print_hierholzer_result(hierholzer_undirected(graph_matrix));

    UndirectedAdjacencyListGraph<int> graph_list;
    populate_graph_from_file("data/graph-cycle-8.txt", graph_list);
    graph_list.print();
    print_hierholzer_result(hierholzer_undirected(graph_list));

    UndirectedIncidenceMatrixGraph<int> graph_incidence;
    populate_graph_from_file("data/graph-cycle-8.txt", graph_incidence);
    graph_incidence.print();
    print_hierholzer_result(hierholzer_undirected(graph_incidence));
    return 0;
}