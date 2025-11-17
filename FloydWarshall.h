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
    std::vector<std::vector<double>> shortest_paths; // matriz de predecessores para reconstrução dos caminhos mais curtos
    std::vector<std::vector<int>> predecessors; // matriz de precessores para reconstrução dos caminhos mais curtos
    std::vector<IGraph<Node>> shortest_paths_trees; // grafos representando os caminhos mais curtos entre todos os pares de nós

    FloydWarshallResult(size_t order = 0)
        : shortest_paths(order, std::vector<int>(order, std::numeric_limits<double>::infinity()))
        , predecessors(order, std::vector<int>(order, -1)) {}
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
    
    FloydWarshallResult<Node> result;
    return result;
    }


template<typename Node>
void print_floyd_warshall_result(const FloydWarshallResult<Node>& result, const IGraph<Node>& graph) {
    // Implementação da função de impressão do resultado do Floyd-Warshall
}


#endif // FLOYD_WARSHALL_H