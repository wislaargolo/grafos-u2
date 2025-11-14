#include <iostream>
#include <list>
#include <string>

#include "../Hierholzer.h"
#include "../graph/DirectedAdjacencyMatrixGraph.h"
#include "../graph/DirectedAdjacencyListGraph.h"
#include "../graph/DirectedIncidenceMatrixGraph.h"
#include "../utils/GraphIO.h"


int main() {

    std::cout << "Creating graphs from file 'graph-path-9.txt'...\n\n";
    DirectedAdjacencyMatrixGraph<char> graph_matrix;
    populate_graph_from_file("data/graph-path-9.txt", graph_matrix);
    graph_matrix.print();
    print_hierholzer_result(hierholzer_directed(graph_matrix));

    DirectedAdjacencyListGraph<char> graph_list;
    populate_graph_from_file("data/graph-path-9.txt", graph_list);
    graph_list.print();
    print_hierholzer_result(hierholzer_directed(graph_list));

    DirectedIncidenceMatrixGraph<char> graph_incidence;
    populate_graph_from_file("data/graph-path-9.txt", graph_incidence);
    graph_incidence.print();
    print_hierholzer_result(hierholzer_directed(graph_incidence));
    return 0;
}