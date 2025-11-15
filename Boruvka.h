#ifndef BORUVKA_H
#define BORUVKA_H

#include <vector>
#include <queue>
#include <iostream>
#include <limits>
#include "graph/IGraph.h"
#include "graph/UndirectedAdjacencyListGraph.h"

/**
 * @struct DivideBlocksResult
 * @brief Estrutura para armazenar o resultado da função de dividir blocos.
 **/
struct DivideBlocksResult {
    std::vector<std::vector<int>> blocks; // vetor de blocos encontrados
    std::vector<int> block_index; // índice do bloco de cada nó, para verificar se nós pertencem ao mesmo bloco

    DivideBlocksResult(size_t order = 0)
        : block_index(order, -1) {};
};

/**
 * @struct BoruvkaResult
 * @brief Estrutura para armazenar o resultado do algoritmo de Boruvka.
 **/
template<typename Node>
struct BoruvkaResult {
    UndirectedAdjacencyListGraph<Node> tree; // árvore geradora mínima encontrada utilizando o algoritmo
    double total_weight = 0; // peso total mínimo encontrado
};

/**
 * @brief Função para determinar quais os componentes conexos da árvore em seu estado atual.
 * @param tree Árvore que está sendo gerada pelo boruvka.
 * @return O resultado determinando os blocos e qual o bloco de cada nó.
 */
template<typename Node>
DivideBlocksResult divide_blocks(IGraph<Node>& tree) {
    DivideBlocksResult result(tree.get_order());

    // Para cada nó da árvore com bloco ainda indeterminado, executa uma BFS para encontrar um bloco
    for (size_t node = 0; node < tree.get_order(); node++) {
        if (result.block_index[node] == -1) {
            // Índice do novo bloco a ser adicionado
            int curr_block = result.blocks.size();

            // Bloco atual, que é um vetor de nós
            std::vector<int> block;
            // Fila para execução da BFS
            std::queue<int> queue;

            // Adiciona o nó inicial na fila
            queue.push(node);

            // Enquanto a fila não for vazia
            while (!queue.empty()) {
                // Pega o primeiro nó da fil
                int current = queue.front();

                queue.pop();

                // Adiciona o nó no bloco atual
                block.push_back(current);
                result.block_index[current] = curr_block;

                // Todos os vizinhos do nó sem bloco determinado são adicionados na fila
                for (size_t neighbor : tree.get_neighbors_indices(current)) {
                    if (result.block_index[neighbor] == -1) {
                        queue.push(neighbor);
                    }
                }
            }

            // Adiciona o bloco atual na lista de blocos
            result.blocks.push_back(block);
        }
    }

    return result;
}

/**
 * @brief Implementa o algoritmo de Boruvka.
 * @param graph O grafo onde o algoritmo será aplicado.
 * @param weights A matriz de pesos das arestas do grafo.
 * @return O resultado do algoritmo contendo a árvore geradora mínima e o seu peso total.
 */
template<typename Node>
BoruvkaResult<Node> boruvka(IGraph<Node>& graph, const std::vector<std::vector<double>>& weights) {
    BoruvkaResult<Node> result;

    // Adiciona todos os nós do grafo na árvore
    for (Node& node : graph.get_nodes()) {
        result.tree.add_node(node);
    }

    DivideBlocksResult divided_blocks(graph.get_order());

    // Para todos os nós, informa que incialmente eles pertencem a um bloco com apenas eles mesmos
    for (size_t node = 0; node < graph.get_order(); node++) {
        std::vector<int> block(1, node);

        divided_blocks.blocks.push_back(block);
        divided_blocks.block_index[node] = node;
    }

    // Enquanto o número de blocos for maior que um
    while (divided_blocks.blocks.size() > 1) {
        // Vetor de arestas que serão adicionadas ao final da iteração
        std::vector<EdgeIndex> new_edges;

        // Para cada bloco da árvore
        for (auto& block : divided_blocks.blocks) {
            // Armazena a aresta escolhida para ser adicionada para esse bloco
            EdgeIndex chosen_edge;
            // Armazena o peso da aresta escolhida, que deve ser o menor possível
            double min_weight = std::numeric_limits<double>::infinity();

            // Percorre todas as arestas do bloco
            for (int node : block) {
                for (int neighbor : graph.get_neighbors_indices(node)) {
                    // Caso a aresta vá para um nó que não pertence a esse bloco
                    if (divided_blocks.block_index[node] != divided_blocks.block_index[neighbor]) {
                        // Caso a aresta tenha um peso menor que o menor peso encontrado até então
                        if (weights[node][neighbor] < min_weight) {
                            // Redefine a aresta escolhida
                            chosen_edge.from = node;
                            chosen_edge.to = neighbor;
                            min_weight = weights[node][neighbor];
                        }
                    }
                }
            }

            // Adiciona a aresta na lista de arestas escolhidas
            new_edges.push_back(chosen_edge);
        }

        // Adiciona todas as arestas na árvore
        for (auto& edge : new_edges) {
            result.tree.add_edge(graph.get_node(edge.from), graph.get_node(edge.to));
        }

        // Depois da atualização da árvore, redefine quais são os blocos da árvore
        divided_blocks = divide_blocks(result.tree);
    }

    // Calcula o peso total somando o peso de todas as arestas da árvore
    for (auto& edge : result.tree.get_all_edges()) {
        result.total_weight += weights[edge.from][edge.to];
    }

    return result;
}

#endif
