#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "graph/IGraph.h"
#include "graph/UndirectedAdjacencyListGraph.h"
#include "utils/Dfs.h"

/**
 * @struct KruskalResult
 * @brief Estrutura para armazenar o resultado do algoritmo de Kruskal.
 *
 * Armazena a árvore geradora mínima e seu custo total.
 **/
template<typename Node>
struct KruskalResult {
    UndirectedAdjacencyListGraph<Node> tree;
    int total_weight = 0;
};

/**
 * @struct WeightedEdge
 * @brief Estrutura auxiliar para armazenar arestas com peso e facilitar a ordenação.
 */
struct WeightedEdge {
    int from;
    int to;
    double weight;

    /**
     * @brief Sempre que você precisar comparar duas WeightedEdge com o operador <, o critério é o campo weight.
     */
    bool operator<(const WeightedEdge& other) const {
        return weight < other.weight;
    }
};

/**
 * @brief Implementa o algoritmo de Kruskal para encontrar a Árvore Geradora Mínima.
 * @param graph O grafo Onde o algoritmo será aplicado.
 * @param weights A matriz de pesos das arestas do grafo.
 * @return O resultado do algoritmo contendo a MST e seu peso total.
 */
template<typename Node>
KruskalResult<Node> kruskal(const IGraph<Node>& graph,
                           const std::vector<std::vector<double>>& weights) {

    KruskalResult<Node> result;
    size_t n = graph.get_order();

    // Retorna resultado vazio se o grafo estiver vazio
    if (n == 0) {
        return result;
    }

    std::vector<WeightedEdge> all_edges;
    std::vector<EdgeIndex> edge_indices = graph.get_all_edges();

    // Percorrer todas as arestas do grafo e armazená-las com seus pesos
    for (const auto& edge : edge_indices) {
        all_edges.push_back({edge.from, edge.to, weights[edge.from][edge.to]});
    }

    // Ordena as arestas em ordem crescente de peso
    std::sort(all_edges.begin(), all_edges.end());

    // Adiciona todos os nós do grafo original à árvore resultante
    for (const auto& node : graph.get_nodes()) {
        result.tree.add_node(node);
    }

    // Loop principal do algoritmo de Kruskal
    int edges_added_count = 0;
    for (const auto& edge : all_edges) {

        Node node_from = graph.get_node(edge.from);
        Node node_to = graph.get_node(edge.to);

        // Executa a DFS na árvore parcial (result.tree) para verificar alcançabilidade
        DFSResult<Node> dfs_result;

        try {
            dfs_result = dfs_unidirectional(result.tree, node_from);
        } catch (const std::invalid_argument& e) {
            continue;
        }

        // Se node_to está no mapa de descoberta e seu tempo é > 0, ele foi alcançado.
        bool forms_cycle = (dfs_result.discovery.count(node_to) && dfs_result.discovery.at(node_to) > 0);

        if (!forms_cycle) {
            // "T <- T U h_i"
            result.tree.add_edge(node_from, node_to);
            result.total_weight += edge.weight;
            edges_added_count++;

            // parar quando a árvore tiver n-1 arestas
            if (edges_added_count >= n - 1) {
                break;
            }
        }
    }

    return result;
}

/**
 * @brief Imprime o resultado do algoritmo de Kruskal.
 * @param result O resultado do algoritmo.
 */
template<typename Node>
void print_kruskal_result(KruskalResult<Node>& result) {
    std::cout << "Kruskal's Algorithm Result: \n";
    std::cout << "Result: \n";
    result.tree.print();
    std::cout << "Total weight: " << result.total_weight << "\n";
}


#endif
