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
            // Se há uma aresta de i para j, o predecessor de j é i 
            // ou caso i == j será considerado o próprio nó como predecessor
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
            // apenas copia os pesos das arestas
            result.distances[i][j] = weights[i][j];
        }
        result.distances[i][i] = 0; // Distância para si mesmo é zero
    }

    // Executa o algoritmo de Floyd-Warshall

    // Para cada nó intermediário k verifica se o caminho passando por k é mais curto
    // em todos os pares de nós (i, j)
    for (size_t k = 0; k < graph.get_order(); ++k)
    {
        for (size_t i = 0; i < graph.get_order(); ++i)
        {
            for (size_t j = 0; j < graph.get_order(); ++j)
            {
                // Verifica se o caminho passando por k é mais curto
                if (result.distances[i][k] + result.distances[k][j] < result.distances[i][j])
                {
                    // Atualiza a distância e o predecessor se um caminho mais curto for encontrado
                    result.distances[i][j] = result.distances[i][k] + result.distances[k][j];
                    result.predecessors[i][j] = result.predecessors[k][j];
                }
            }
        }
    }

    for (size_t i = 0; i < graph.get_order(); ++i)
    {
        Node source_node = graph.get_node(i);
        DirectedAdjacencyListGraph<Node> tree = get_shortest_paths_tree(graph, source_node, result.predecessors, result.distances);
        result.shortest_paths_trees.push_back(tree);
    }

    return result;
}


/**
 * @brief Imprime o caminho mais curto entre dois nós.
 * @param graph O grafo original.
 * @param path Vetor com os índices dos nós no caminho.
 * @param distances Matriz de distâncias do Floyd-Warshall.
 */
template <typename Node>
void print_shortest_path(
    const IGraph<Node>& graph,
    std::vector<int> path,
    const std::vector<std::vector<double>>& distances)
{
    std::cout << "  ";
    // Imprime o caminho com os pesos das arestas
    for (size_t i = 0; i < path.size(); ++i)
    {
        std::cout << "(" << graph.get_node(path[i]) << ")";
        if (i + 1 < path.size())
        {
            std::cout << " -- " << static_cast<int>(distances[path[i]][path[i + 1]]) << " --> ";
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Reconstrói o caminho mais curto de um nó origem até um nó destino.
 * @param source_idx Índice do nó de origem.
 * @param dest_idx Índice do nó de destino.
 * @param predecessors Matriz de predecessores do Floyd-Warshall.
 * @return Vetor com os índices dos nós no caminho (do destino até a origem, em ordem reversa).
 */
std::vector<int> reconstruct_path(int source_idx, int dest_idx, 
                                   const std::vector<std::vector<int>>& predecessors)
{
    std::vector<int> path;
    
    // Se não há caminho
    if (predecessors[source_idx][dest_idx] == -1) {
        return path;
    }
    
    // Reconstrói o caminho do destino até a origem
    int current = dest_idx;
    while (current != source_idx) {
        path.push_back(current);
        current = predecessors[source_idx][current];
        
        // Proteção contra ciclos (não deveria acontecer em Floyd-Warshall correto)
        if (current == -1) {
            return std::vector<int>(); // retorna vazio se houver problema
        }
    }
    path.push_back(source_idx);
    
    // Inverte para ter o caminho da origem ao destino
    std::reverse(path.begin(), path.end());
    
    return path;
}

/**
 * @brief Constrói a árvore de caminhos mais curtos a partir de um nó de origem.
 * @param graph O grafo original.
 * @param source_node O nó de origem para construir a árvore.
 * @param predecessors Matriz de predecessores do Floyd-Warshall.
 * @param distances Matriz de distâncias do Floyd-Warshall.
 * @return Um grafo direcionado representando a árvore de caminhos mais curtos.
 */
template <typename Node>
DirectedAdjacencyListGraph<Node> get_shortest_paths_tree(
    const IGraph<Node>& graph,
    const Node& source_node,
    const std::vector<std::vector<int>>& predecessors,
    const std::vector<std::vector<double>>& distances)
{
    DirectedAdjacencyListGraph<Node> tree;
    
    // Adiciona todos os nós do grafo original à árvore
    for (const auto& node : graph.get_nodes()) {
        tree.add_node(node);
    }
    
    int source_idx = graph.get_index(source_node);
    
    // Para cada nó destino alcançável
    for (size_t dest_idx = 0; dest_idx < graph.get_order(); ++dest_idx) {
        // Pula o próprio nó de origem
        if (dest_idx == static_cast<size_t>(source_idx)) {
            continue;
        }
        
        // Pula nós não alcançáveis
        if (distances[source_idx][dest_idx] == std::numeric_limits<double>::infinity()) {
            continue;
        }
        
        // Reconstrói o caminho
        std::vector<int> path = reconstruct_path(source_idx, dest_idx, predecessors);
        
        // Adiciona as arestas do caminho à árvore
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            Node from = graph.get_node(path[i]);
            Node to = graph.get_node(path[i + 1]);
            
            // Adiciona a aresta apenas se ainda não existir na árvore
            // (para evitar duplicação quando múltiplos caminhos compartilham arestas)
            if (!tree.is_adjacent(from, to)) {
                tree.add_edge(from, to);
            }
        }
    }
    
    return tree;
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

    //Imprime as árvores de caminhos mais curtos
    for (size_t i = 0; i < graph.get_order(); ++i) {
        
        std::cout << "Shortest Paths Tree from node " << graph.get_node(i) << ":\n";
        result.shortest_paths_trees[i].print();
    }

    //Imprime os caminhos mais curtos
    
    for (size_t i = 0; i < graph.get_order(); ++i)
    {
        
        std::cout << "Shortest Paths from node " << graph.get_node(i) << ":\n";
        for (size_t j = 0; j < graph.get_order(); ++j) {
            std::vector<int> path = reconstruct_path(graph.get_index(graph.get_node(i)), graph.get_index(graph.get_node(j)), result.predecessors);
            print_shortest_path(graph, path, result.distances);
        }
        std::cout << std::endl;
    }
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