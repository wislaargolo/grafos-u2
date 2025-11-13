#ifndef HIERHOLZER_H
#define HIERHOLZER_H

#include <vector>
#include <stack>
#include <list>
#include <optional>
#include <unordered_map>

#include "graph/IGraph.h"
#include "utils/GraphAlgorithms.h"

/**
 * @struct HierholzerResult
 * @brief Estrutura para armazenar o resultado do algoritmo de Hierholzer.
 */
template<typename Node>
struct HierholzerResult {
    std::list<Node> circuit; // ciclo ou caminho euleriano encontrado.
    bool has_eulerian_cycle;  // indica se o grafo possui ciclo euleriano.
    bool has_eulerian_path;  // indica se o grafo possui caminho euleriano.

    HierholzerResult()
        : has_eulerian_cycle(false), has_eulerian_path(false) {}
};

/**
 * @brief Implementação do Algoritmo de Hierholzer para encontrar ciclos ou caminhos eulerianos em grafos.
 * @param graph O grafo a ser percorrido.
 * @param start_index O índice do nó inicial para começar a busca.
 * @param remove_edge Função para remover uma aresta do grafo quando visitada.
 * @return Uma lista com o ciclo ou caminho euleriano encontrado.
 */
template<typename Node, class RemoveEdgeFunc>
std::list<Node> hierholzer(const IGraph<Node>& graph, int start_index, RemoveEdgeFunc remove_edge) {
    std::list<Node> circuit;
    std::stack<int> curr_path;
    int current_index = start_index;

    // Cria uma cópia da lista de adjacência para manipulação
    std::unordered_map<int, std::list<int>> adj_list;
    for (size_t i = 0; i < graph.get_order(); i++) {
        auto neighbors = graph.get_neighbors_indices(i);
        adj_list[i] = std::list<int>(neighbors.begin(), neighbors.end());
    }


    while(curr_path.size() > 0 || !adj_list[current_index].empty()) {
        // Se o nó atual não tem mais arestas não visitadas, adiciona-o ao resultado
        if (adj_list[current_index].empty()) {
            circuit.push_front(graph.get_node(current_index));
            current_index = curr_path.top();
            curr_path.pop();
        } else {
            // Caso possua arestas não visitadas, continua o percurso
            curr_path.push(current_index);
            int next_index = adj_list[current_index].front();
            // Remove a aresta atual da lista de adjacência. A implementação depende se o grafo é direcionado ou não.
            remove_edge(adj_list, current_index, next_index);
            current_index = next_index;
        }
    }

    circuit.push_front(graph.get_node(current_index));

    return circuit;
}

/**
 * @brief Função principal para encontrar ciclos ou caminhos eulerianos em grafos não direcionados.
 * @param graph O grafo não direcionado a ser analisado.
 * @return o resultado contendo o ciclo ou caminho euleriano, se existir.
 */
template<typename Node>
HierholzerResult<Node> hierholzer_undirected(const IGraph<Node>& graph) {
    if(graph.get_order() == 0) {
        return HierholzerResult<Node>();
    }

    std::optional<int> start_path;
    std::optional<int> start_cycle;

    size_t odd_degree_count = 0;
    for(size_t i = 0; i < graph.get_order(); ++i) {
        size_t degree = graph.get_out_degree(graph.get_node(i));

        // Identifica um nó inicial para o ciclo, se existir
        if(degree > 0 && !start_cycle.has_value()) {
            start_cycle = i;
        }

        // Se o grau for par, não importa porque a regra é que no máximo dois nós podem ter grau ímpar
        if(degree % 2 == 0) {
            continue;
        }

        // Identifica um nó inicial para o caminho, se existir
        if(!start_path.has_value()) {
            start_path = i;
        }

        // Conta o número de nós com grau ímpar
        odd_degree_count++;
    }

    std::optional<int> current_index;
    HierholzerResult<Node> result;

    if(odd_degree_count == 0) {
        // Todos os nós possuem grau par: ciclo euleriano existe
        current_index = start_cycle;
        result.has_eulerian_cycle = true;
        result.has_eulerian_path = true;
    } else if(odd_degree_count <= 2) {
        // No máximo dois nós com grau ímpar: caminho euleriano existe
        current_index = start_path;
        result.has_eulerian_path = true;
    } else {
        // Caso contrário, não há caminho ou ciclo euleriano
        return HierholzerResult<Node>();
    }

    // Em grafos não direcionados, a remoção da aresta deve ser feita em ambas as direções
    auto remove_edge = [](std::unordered_map<int, std::list<int>>& adj_list, int from, int to) {
        adj_list[from].remove(to);
        adj_list[to].remove(from);
    };

    result.circuit = hierholzer(graph, *current_index, remove_edge);
    return result;
}

/**
 * @brief Função principal para encontrar ciclos ou caminhos eulerianos em grafos direcionados.
 * @param graph O grafo direcionado a ser analisado.
 * @return o resultado contendo o ciclo ou caminho euleriano, se existir.
 */
template<typename Node>
HierholzerResult<Node> hierholzer_directed(const IGraph<Node>& graph) {

    if(graph.get_order() == 0) {
        return HierholzerResult<Node>();
    }

    std::optional<int> start_path;
    std::optional<int> start_cycle;

    size_t out_degree = 0, in_degree = 0;
    // Mantém a contagem de nós com grau de entrada e saída diferentes
    size_t in = 0, out = 0;

    for(size_t i = 0; i < graph.get_order(); ++i) {

        // Obtém os graus de entrada e saída do nó atual
        out_degree = graph.get_out_degree(graph.get_node(i));
        in_degree = graph.get_in_degree(graph.get_node(i));

        // Identifica um nó inicial para o ciclo, se existir. Pode ser qualquer nó com arestas de saída.
        if(out_degree > 0 && !start_cycle.has_value()) {
            start_cycle = i;
        }

        // Se os graus de entrada e saída forem iguais, passa para o próximo nó
        if(out_degree == in_degree) {
            continue;
        }

        // Identifica um nó inicial para o caminho, se existir
        if(out_degree == in_degree + 1) {
            start_path = i;
            out++;
        } else if(in_degree == out_degree + 1) {
            in++;
        } else {
            return HierholzerResult<Node>();
        }
    }

    std::optional<int> current_index;
    HierholzerResult<Node> result;

    // Determina o ponto de partida com base nos graus dos nós
    if(out == 0 && in == 0) {
        // Todos os nós possuem mesmo grau: ciclo euleriano existe
        current_index = start_cycle;
        result.has_eulerian_cycle = true;
        result.has_eulerian_path = true;
    } else if(out == 1 && in == 1) {
        // No máximo um nó com grau de saída maior por 1 e no máximo um nó com grau de entrada maior por 1: caminho euleriano existe
        current_index = start_path;
        result.has_eulerian_path = true;
    } else {
        // Caso contrário, não há caminho ou ciclo euleriano
        return HierholzerResult<Node>();
    }

    // Em grafos direcionados, a remoção da aresta é feita apenas na direção da aresta
    auto remove_edge = [](std::unordered_map<int, std::list<int>>& adj_list, int from, int to) {
        adj_list[from].remove(to);
    };

    result.circuit = hierholzer(graph, *current_index, remove_edge);
    return result;
}


#endif