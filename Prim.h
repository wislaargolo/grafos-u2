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
 * @brief Imprime o estado atual de Prim usando o vetor 'added'.
 */
template<typename Node>
void print_prim_state(const std::string& title,
                      const IGraph<Node>& graph,
                      const std::vector<bool>& added, // MODIFICADO: Era Z e N
                      const PrimResult<Node>& result) {

    std::cout << "\n" << title << "\n";
    std::cout << "-----------------------------\n";

    size_t n = added.size();

    std::cout << "Z { ";
    bool z_empty = true;
    for (size_t i = 0; i < n; ++i) {
        if (added[i]) {
            std::cout << graph.get_node(i) << " ";
            z_empty = false;
        }
    }
    if (z_empty) std::cout << "(vazio)";
    std::cout << "}\n";

    std::cout << "N { ";
    bool n_empty = true;
    for (size_t i = 0; i < n; ++i) {
        if (!added[i]) {
            std::cout << graph.get_node(i) << " ";
            n_empty = false;
        }
    }
    if (n_empty) std::cout << "(vazio)";
    std::cout << "}\n";


    // T
    std::cout << "T { ";
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

    // Representa os nós adicionados à árvore
    std::vector<bool> added(n, false);

    added[start_index] = true;
    int nodes_added_count = 1;

    print_prim_state("Estado Inicial", graph, added, result);

    while (nodes_added_count < n) {

        double min_weight = std::numeric_limits<double>::infinity();
        int best_j = -1; // Nó em Z (from)
        int best_k = -1; // Nó em N (to)

        // Procurar aresta mínima com j ∈ Z e k ∈ N
        // usando o vetor 'added' para verificar a quais conjuntos eles pertencem.
        for (int j = 0; j < n; ++j) {
            // Se 'j' não está em Z, pula (if added[j] is false)
            if (added[j]){

                for (int k : graph.get_neighbors_indices(j)) {

                // j ∈ Z, k ∈ N, e (j, k) é uma aresta que existe.
                if (!added[k] & weights[j][k] < min_weight) {
                    min_weight = weights[j][k];
                    best_j = j;
                    best_k = k;
                }
            }
            }
        }

        // Grafo desconectado
        if (best_k == -1) break;

        added[best_k] = true;
        nodes_added_count++;

        // Adiciona a aresta à árvore e soma o peso
        result.tree.add_edge(graph.get_node(best_j), graph.get_node(best_k));
        result.total_weight += min_weight;
    }

    print_prim_state("Estado Final", graph, added, result);
    return result;
}

/**
 * @brief Imprime o resultado final do algoritmo de Prim.
 */
template<typename Node>
void print_prim_result(PrimResult<Node>& result) {
    std::cout << "\nPrim's Algorithm Result: \n";
    std::cout << "Result: ";
    result.tree.print();
    std::cout << "Total weight: " << result.total_weight << "\n";
}

#endif // PRIM_H
