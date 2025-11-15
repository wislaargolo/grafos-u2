#ifndef PRIM_H
#define PRIM_H

#include <vector>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <algorithm>

#include "graph/IGraph.h"
#include "graph/UndirectedAdjacencyListGraph.h"

/**
 * @struct PrimResult
 * @brief Estrutura para armazenar o resultado do algoritmo de Prim.
 **/
template<typename Node>
struct PrimResult {
    UndirectedAdjacencyListGraph<Node> tree;
    double total_weight = 0;
};

/**
 * @brief Imprime o estado atual de Prim usando vetores Z, N e T.
 */
template<typename Node>
void print_prim_state(const std::string& title,
                      const IGraph<Node>& graph,
                      const std::vector<int>& Z,
                      const std::vector<int>& N,
                      const PrimResult<Node>& result) {

    std::cout << "\n" << title << "\n";
    std::cout << "-----------------------------\n";

    // Z
    std::cout << "Z { ";
    if (Z.empty()) std::cout << "(vazio)";
    else for (int i : Z) std::cout << graph.get_node(i) << " ";
    std::cout << "}\n";

    // N
    std::cout << "N { ";
    if (N.empty()) std::cout << "(vazio)";
    else for (int i : N) std::cout << graph.get_node(i) << " ";
    std::cout << "}\n";

    // T
    std::cout << "T (arestas): { ";
    if (result.tree.get_size() == 0)
        std::cout << "(vazia)";
    else
        result.tree.print();
    std::cout << " }\n";

    std::cout << "-----------------------------\n";
}

/**
 * @brief Implementação do algoritmo de Prim baseado fielmente no pseudocódigo da imagem.
 */
template<typename Node>
PrimResult<Node> prim(const IGraph<Node>& graph,
                      const std::vector<std::vector<double>>& weights,
                      const Node& start) {

    PrimResult<Node> result;
    size_t n = graph.get_order();

    if (n == 0) return result;

    // Adiciona todos os vértices ao grafo árvore
    for (const auto& node : graph.get_nodes()) {
        result.tree.add_node(node);
    }

    int start_index = graph.get_index(start);
    if (start_index == -1) {
        throw std::runtime_error("Start node does not exist in the graph.");
    }

    std::vector<int> Z;
    std::vector<int> N;

    Z.push_back(start_index);

    for (int i = 0; i < n; i++) {
        if (i != start_index) N.push_back(i);
    }

    print_prim_state("Estado Inicial", graph, Z, N, result);

    while (N.size() > 0) {

        double min_weight = std::numeric_limits<double>::infinity();
        int best_j = -1;
        int best_k = -1;

        // Procurar aresta mínima com j ∈ Z e k ∈ N
        for (int j : Z) {
            for (int k : N) {
                if (weights[j][k] < min_weight) {
                    min_weight = weights[j][k];
                    best_j = j;
                    best_k = k;
                }
            }
        }

        // Grafo desconectado
        if (best_k == -1) break;

        Z.push_back(best_k);
        N.erase(std::remove(N.begin(), N.end(), best_k), N.end());

        result.tree.add_edge(graph.get_node(best_j), graph.get_node(best_k));
        result.total_weight += min_weight;
    }

    print_prim_state("Estado Final", graph, Z, N, result);
    return result;
}

/**
 * @brief Imprime o resultado final do algoritmo de Prim.
 */
template<typename Node>
void print_prim_result(PrimResult<Node>& result) {
    std::cout << "\nPrim's Algorithm Result (Resumo): \n";
    std::cout << "Total weight: " << result.total_weight << "\n";
}

#endif // PRIM_H
