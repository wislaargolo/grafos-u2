#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <vector>
#include <limits>
#include <iostream>
#include "graph/IGraph.h"

/**
 * @struct BellmanFordResult
 * @brief Estrutura para armazenar o resultado do algoritmo de Bellman-Ford.
 **/
struct BellmanFordResult {
    std::vector<double> distances; // Distâncias mínimas do nó inicial para cada nó.
    std::vector<int> predecessors; // Predecessores dos nós no caminho mais curto.
    bool has_negative_cycle; // Indica se há um ciclo negativo.

    BellmanFordResult(size_t order = 0)
        : has_negative_cycle(false)
        , distances(order, std::numeric_limits<double>::infinity())
        , predecessors(order, -1) {}

};

template<typename Node>
BellmanFordResult bellman_ford(const IGraph<Node>& graph, 
    const std::vector<std::vector<double>>& weights, const Node& start) {

    BellmanFordResult result(graph.get_order());
    int start_index = graph.get_index(start);

    result.distances[start_index] = 0;

    for(size_t i = 0; i < graph.get_order() - 1; ++i) {
        for(const EdgeIndex& edge : graph.get_all_edges()) {
            int u = edge.from;
            int v = edge.to;
            double weight = weights[u][v];

            if(result.distances[u] + weight < result.distances[v]) {
                result.distances[v] = result.distances[u] + weight;
                result.predecessors[v] = u;
            }
        }
    }

    // Verifica a presença de ciclos negativos.
    for(const EdgeIndex& edge : graph.get_all_edges()) {
        int u = edge.from;
        int v = edge.to;
        double weight = weights[u][v];

        if(result.distances[u] + weight < result.distances[v]) {
            result.has_negative_cycle = true;
            break;
        }
    }

    return result;
}

template<typename Node>
void print_bellman_ford_result(const BellmanFordResult& result, const IGraph<Node>& graph) {
    if(result.has_negative_cycle) {
        std::cout << "Graph contains a negative weight cycle.\n";
    } else {
        std::cout << "Shortest distances from the start node:\n";

        const int label_width = 12; 
        const int data_width = 6;
        
        std::cout << std::left << std::setw(label_width) << "Nodes" << std::right << " | ";
        for(size_t i = 0; i < graph.get_order(); ++i) {
            std::cout << std::setw(data_width) << graph.get_node(i);
        }

        std::cout << "\n";
        std::cout << std::setfill('-') << std::setw(label_width) << "" << "---";
        for(size_t i = 0; i < graph.get_order(); ++i) {
            std::cout << std::setw(data_width) << "";   
        }

        std::cout << std::setfill(' ') << "\n";

        std::cout << std::left << std::setw(label_width) << "Distances" << std::right << " | ";
        for(size_t i = 0; i < graph.get_order(); ++i) {
            if(result.distances[i] == std::numeric_limits<double>::infinity()) {
                std::cout << std::setw(data_width) << "INF";
            } else {
                std::cout << std::setw(data_width) << std::setprecision(2) << std::fixed << result.distances[i];
            }
        }

        std::cout << "\n";

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
}

#endif // BELLMAN_FORD_H