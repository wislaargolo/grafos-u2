#ifndef DJIKSTRA_H
#define DJIKSTRA_H

#include <vector>
#include <iostream>
#include <limits>
#include "graph/IGraph.h"

/**
 * @struct DjikstraResult
 * @brief Estrutura para armazenar o resultado do algoritmo de Djikstra.
 **/
struct DjikstraResult {
    std::vector<double> distances; // Distâncias mínimas do nó inicial para cada nó.
    std::vector<int> predecessors; // Predecessores dos nós no caminho mais curto.

    DjikstraResult(size_t order = 0)
        : distances(order, std::numeric_limits<double>::infinity())
        , predecessors(order, -1) {}

};

/**
 * @brief Função para pegar o nó não-visitado de menor distância durante a execução do algoritmo.
 * @param distances Vetor de distâncias parcial.
 * @param visited Vetor que indica os nós visitados.
 * @return O íncide do nó de menor distância que ainda não foi visitado, e -1 caso não haja nenhum nó não-visitado.
 */
int get_min_node(std::vector<double>& distances, std::vector<bool>& visited) {
    // Guarda o índice do nó de menor distância e não-visitado
    int node = -1;
    // Guarda a menor distância encontrada
    double min_distance = std::numeric_limits<double>::infinity();

    // Percorre todos os nós
    for (size_t i = 0; i < distances.size(); i++) {
        // Se o nó atual não foi visitado e a distância dele é menor que a menor encontrada até o momento
        if (!visited[i] && distances[i] < min_distance) {
            // Atualiza o no e distância encontrados para ser o nó e distância do nó atual
            node = i;
            min_distance = distances[i];
        }
    }

    return node;
}

/**
 * @brief Implementa o algoritmo de Djikstra.
 * @param graph O grafo onde o algoritmo será aplicado.
 * @param weights A matriz de pesos das arestas do grafo.
 * @param start O nó inicial para o cálculo das distâncias.
 * @return O resultado do algoritmo contendo distâncias e predecessores.
 */
template<typename Node>
DjikstraResult djikstra(const IGraph<Node>& graph,
    const std::vector<std::vector<double>>& weights, const Node& start) {

    DjikstraResult result(graph.get_order());
    // Vetor que indica se o nó da posição i foi visitado ou não
    std::vector<bool> visited(graph.get_order(), false);

    int start_index = graph.get_index(start);
    // Marca a distância do nó inicial como 0 e o marca como visitado
    result.distances[start_index] = 0;
    visited[start_index] = 1;

    // Para todos os vizinhos do nó inicial, inicializa a distância como sendo o peso da aresta
    // e o predecessor como sendo o nó inicial
    for (int neighbor : graph.get_neighbors_indices(start_index)) {
        result.distances[neighbor] = weights[start_index][neighbor];
        result.predecessors[neighbor] = start_index;
    }

    // Guarda o nó que está sendo visitado no momento
    int current;

    // Pega o nó não-visitado de menor distância, e enquanto ele existir, continua no loop
    while ((current = get_min_node(result.distances, visited)) != -1) {
        // Marca o nó atual como visitado
        visited[current] = 1;

        // Para todos os vizinhos não-visitados do nó atual
        for (int neighbor : graph.get_neighbors_indices(current)) {
            if (!visited[neighbor]) {
                // Distância do nó inicial até o vizinho passando pelo nó atual
                double distance = result.distances[current] + weights[current][neighbor];

                // Se a distância passando pelo nó atual for menor que a distância atual do vizinho
                if (result.distances[neighbor] > distance) {
                    // Atualiza a distância e o predecessor do vizinho para a distância calculada e o nó atual
                    result.distances[neighbor] = distance;
                    result.predecessors[neighbor] = current;
                }
            }
        }
    }

    return result;
}

#endif
