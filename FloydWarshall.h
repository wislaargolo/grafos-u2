#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include "graph/IGraph.h"
#include "graph/DirectedAdjacencyListGraph.h"

/**
 * @struct FloydWarshallResult
 * @brief Estrutura para armazenar o resultado do algoritmo de Floyd-Warshall.
 */
template <typename Node>
struct FloydWarshallResult
{
    std::vector<std::vector<double>> distances;                         // matriz de distâncias mínimas entre todos os pares de nós
    std::vector<std::vector<int>> predecessors;                         // matriz de predecessores para reconstrução dos caminhos mais curtos
    std::vector<DirectedAdjacencyListGraph<Node>> shortest_paths_trees; // grafos representando os caminhos mais curtos entre todos os pares de nós

    FloydWarshallResult(size_t order = 0)
        : distances(order, std::vector<double>(order, std::numeric_limits<double>::infinity())), predecessors(order, std::vector<int>(order, -1))
    {
    }
};

/**
 * @brief Implementa o algoritmo de Floyd-Warshall.
 * @param graph O grafo onde o algoritmo será aplicado.
 * @param weights A matriz de pesos das arestas do grafo.
 * @return O resultado do algoritmo contendo a matriz de distâncias e predecessores.
 */
template <typename Node>
FloydWarshallResult<Node> floyd_warshall(const IGraph<Node> &graph,
                                         const std::vector<std::vector<double>> &weights)
{

    FloydWarshallResult<Node> result(graph.get_order());

    // Inicializa a matriz de predecessores
    for (size_t i = 0; i < graph.get_order(); ++i)
    {
        int node_i = graph.get_index(graph.get_nodes()[i]);
        for (size_t j = 0; j < graph.get_order(); ++j)
        {
            if (weights[i][j] != std::numeric_limits<double>::infinity() || i == j)
            {
                result.predecessors[i][j] = node_i;
            }
            else
            {
                result.predecessors[i][j] = -1; // Indica que não há caminho
            }
        }
    }

    // Inicializa a matriz de distâncias com os pesos das arestas
    for (size_t i = 0; i < graph.get_order(); ++i)
    {
        for (size_t j = 0; j < graph.get_order(); ++j)
        {
            result.distances[i][j] = weights[i][j];
        }
        result.distances[i][i] = 0; // Distância para si mesmo é zero
    }

    // Executa o algoritmo de Floyd-Warshall
    for (size_t k = 0; k < graph.get_order(); ++k)
    {
        for (size_t i = 0; i < graph.get_order(); ++i)
        {
            for (size_t j = 0; j < graph.get_order(); ++j)
            {
                if (result.distances[i][k] + result.distances[k][j] < result.distances[i][j])
                {
                    result.distances[i][j] = result.distances[i][k] + result.distances[k][j];
                    result.predecessors[i][j] = result.predecessors[k][j];
                }
            }
        }
    }

    return result;
}

template <typename Node>
void print_floyd_warshall_result(const FloydWarshallResult<Node> &result, const IGraph<Node> &graph)
{
    // Implementação da função de impressão do resultado do Floyd-Warshall
    std::cout << "Floyd-Warshall Result:\n";

   //Imprime a matriz de predecessores
    print_predecessors_matrix(result.predecessors, graph);

    //Imprime a matriz de distâncias
    print_distances_matrix(result.distances, graph);
}



template<typename Node>
void print_predecessors_matrix(const std::vector<std::vector<int>>& predecessors, const IGraph<Node>& graph) {
    size_t order = graph.get_order();

    std::cout << "Predecessors Matrix:\n";

    int col_width = 8;

    std::cout << "  |";
    for (size_t j = 0; j < order; ++j) {
        std::cout << std::setw(col_width) << graph.get_node(j);
    }
    std::cout << "\n";

    std::cout << std::setfill('-') << "----";
    for (size_t j = 0; j < order; ++j) {
        std::cout << std::setw(col_width) << "";
    }
    std::cout << std::setfill(' ') << "\n";

    for (size_t i = 0; i < order; ++i) {

        std::cout << graph.get_node(i);
        (graph.get_node(i) < 10) ? std::cout << "  |" : std::cout << " |"; 

        for (size_t j = 0; j < order; ++j) {
            if (j < predecessors[i].size() && predecessors[i][j] != -1) {
                std::cout << "\033[1;32m"
                          << std::setw(col_width) << std::setprecision(2) << std::fixed << graph.get_nodes()[predecessors[i][j]]
                          << "\033[0m"; 
            } else {
                std::cout << std::setw(col_width) << " -1";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}



template<typename Node>
void print_distances_matrix(const std::vector<std::vector<double>>& distances, const IGraph<Node>& graph) {
    size_t order = graph.get_order();

    std::cout << "Distances Matrix:\n";

    int col_width = 8;

    std::cout << "  |";
    for (size_t j = 0; j < order; ++j) {
        std::cout << std::setw(col_width) << graph.get_node(j);
    }
    std::cout << "\n";

    std::cout << std::setfill('-') << "----";
    for (size_t j = 0; j < order; ++j) {
        std::cout << std::setw(col_width) << "";
    }
    std::cout << std::setfill(' ') << "\n";

    for (size_t i = 0; i < order; ++i) {

        std::cout << graph.get_node(i);
        (graph.get_node(i) < 10) ? std::cout << "  |" : std::cout << " |"; 

        for (size_t j = 0; j < order; ++j) {
            if (j < distances[i].size() && distances[i][j] != std::numeric_limits<double>::infinity()) {
                std::cout << "\033[1;32m"
                          << std::setw(col_width) << std::setprecision(2) << std::fixed << distances[i][j]
                          << "\033[0m"; 
            } else {
                std::cout << std::setw(col_width) << "INF";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}


#endif // FLOYD_WARSHALL_H