#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "Dfs.h"
#include "./graph/UndirectedAdjacencyListGraph.h"
#include "./graph/UndirectedAdjacencyMatrixGraph.h"

/**
 * @brief Popula um grafo com dados de um arquivo de texto.
 *
 * @details O formato esperado do arquivo é: uma linha de cabeçalho inicial e
 * linhas seguintes no formato "u,v", onde 'u' e 'v' são os nós
 * que formam uma aresta.
 *
 * @tparam Node O tipo de dado dos nós do grafo.
 * @param filename O caminho para o arquivo de entrada.
 * @param graph O objeto grafo a ser populado.
 * @throws std::runtime_error se o arquivo não puder ser aberto.
 */
template<typename Node>
void populate_graph_from_file(const std::string& filename, IGraph<Node>& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    /*Ignora a primeira linha de cabeçalho*/
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string part1, part2;

        if (std::getline(ss, part1, ',') && std::getline(ss, part2)) {
            std::stringstream s1(part1);
            std::stringstream s2(part2);

            Node u, v;
            s1 >> u;
            s2 >> v;

            graph.add_edge(u, v);
        }
    }

    file.close();
}

/**
 * @brief Adiciona um nó ao grafo a partir de uma representação em string.
 *
 * @tparam Node O tipo de dado dos nós do grafo.
 * @param str A string contendo o valor do nó a ser adicionado.
 * @param graph O grafo onde o nó será inserido.
 */
template<typename Node>
void add_node_from_string(const std::string& str, IGraph<Node>& graph) {
    Node node;
    std::stringstream ss(str);
    ss >> node;
    graph.add_node(node);
}

/**
 * @brief Remove um nó do grafo a partir de uma representação em string.
 *
 * @tparam Node O tipo de dado dos nós do grafo.
 * @param str A string contendo o valor do nó a ser removido.
 * @param graph O grafo de onde o nó será removido.
 */
template<typename Node>
void remove_node_from_string(const std::string& str, IGraph<Node>& graph) {
    Node node;
    std::stringstream ss(str);
    ss >> node;
    graph.remove_node(node);
} 

#endif // GRAPH_IO_H
