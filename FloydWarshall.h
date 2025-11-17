#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <vector>
#include <iostream>
#include <limits>
#include "graph/IGraph.h"

/**
 * @struct FloydWarshallResult
 * @brief Estrutura para armazenar o resultado do algoritmo de Floyd-Warshall.
 */
template<typename Node>
struct FloydWarshallResult {
    // std::vector<std::vector<double>> distances; // matriz de distâncias mínimas entre todos os pares de nós
    // std::vector<std::vector<Node>> predecessors; // matriz de predecessores para reconstrução dos caminhos mais curtos
    std::vector<DirectedAdjacencyListGraph<Node>> shortest_paths_trees; // grafos representando os caminhos mais curtos entre todos os pares de nós

    FloydWarshallResult(size_t order = 0)
        // : distances(order, std::vector<double>(order, std::numeric_limits<double>::infinity()))
        // , predecessors(order, std::vector<int>(order, -1))
        : shortest_paths_trees(order, DirectedAdjacencyListGraph<Node>(order)) {}
};


/**
 * @brief Implementa o algoritmo de Floyd-Warshall.
 * @param graph O grafo onde o algoritmo será aplicado.
 * @param weights A matriz de pesos das arestas do grafo.
 * @return O resultado do algoritmo contendo a matriz de distâncias e predecessores.
 */
template<typename Node>
FloydWarshallResult<Node> floyd_warshall(const IGraph<Node>& graph, 
    const std::vector<std::vector<double>>& weights) {
    
    FloydWarshallResult<Node> result(graph.get_order());

    // Inicializa a matriz de distâncias e predecessores
    for (size_t i = 0; i < graph.get_order(); ++i) {
        for (size_t j = 0; j < graph.get_order(); ++j) {
            result.distances[i][j] = weights[i][j];
            if (i != j && weights[i][j] != std::numeric_limits<double>::infinity()) {
                result.predecessors[i][j] = i;
            }
        }

    return result;
    }
}

template<typename Node>
void print_floyd_warshall_result(const FloydWarshallResult<Node>& result, const IGraph<Node>& graph) {
    // Implementação da função de impressão do resultado do Floyd-Warshall
    std::cout << "Floyd-Warshall Result:\n";

    for (size_t i = 0; i < graph.get_order(); ++i) {
        for (size_t j = 0; j < graph.get_order(); ++j) {
            if (result.distances[i][j] == std::numeric_limits<double>::infinity()) {
                std::cout << "INF ";
            } else {
                std::cout << result.distances[i][j] << " ";
            }
        }
        std::cout << "\n";
    }

    // Imprime as árvores de caminhos mais curtos
    for (size_t i = 0; i < graph.get_order(); ++i) {
        std::cout << "Shortest paths tree from node " << i << ":\n";
        for (const auto& tree : result.shortest_paths_trees) {
            tree.print_graph();
        }
    }
}

#endif // FLOYD_WARSHALL_H