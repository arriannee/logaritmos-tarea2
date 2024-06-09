#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <algorithm>

#include "fibonacci.cpp"

// Implementación de Dijkstra utilizando Cola de Fibonacci
void dijkstraWithFibonacci(Graph& graph, int root) {
    std::map<int, NodoFibonacci*> nodeMap;
    ColaFibonacci fibHeap;

    std::map<int, double> distances;
    std::map<int, int> previous;

    for (const auto& node : graph.nodes) {
        distances[node.first] = std::numeric_limits<double>::infinity();
        previous[node.first] = -1;
    }
    distances[root] = 0.0;

    for (const auto& node : graph.nodes) {
        NodoFibonacci* fibNode = new NodoFibonacci(node.first, distances[node.first]);
        fibHeap.insert(fibNode);
        nodeMap[node.first] = fibNode;
    }

    while (!fibHeap.empty()) {
        NodoFibonacci* u = fibHeap.extractMin();
        for (const auto& neighbor : graph.nodes[u->id].matrix) {
            int v = neighbor.first;
            for (const Edge& edge : neighbor.second) {
                double alt = distances[u->id] + edge.weight;
                if (alt < distances[v]) {
                    distances[v] = alt;
                    previous[v] = u->id;
                    fibHeap.decreaseKey(nodeMap[v], alt);
                }
            }
        }
    }

    // Imprimir los resultados
    std::cout << "Distancias desde el nodo " << root << ":\n";
    for (const auto& d : distances) {
        std::cout << "Nodo " << d.first << ": " << d.second << "\n";
    }

    std::cout << "\nPrevios:\n";
    for (const auto& p : previous) {
        std::cout << "Nodo " << p.first << ": " << p.second << "\n";
    }
}

// Función para reconstruir el camino más corto desde el nodo raíz hasta el nodo objetivo
std::vector<int> reconstruirCamino(const std::map<int, int>& previos, int objetivo) {
    std::vector<int> camino;
    for (int en = objetivo; en != -1; en = previos.at(en)) {
        camino.push_back(en);
    }
    std::reverse(camino.begin(), camino.end());
    return camino;
}

// Función para probar Dijkstra con cola de Fibonacci
void testDijkstraWithFibonacci() {
    Graph graph;

    // Agregar nodos
    graph.addNode(0);
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);

    // Agregar aristas
    graph.addEdge(0, 1, 2.5);
    graph.addEdge(0, 2, 1.0);
    graph.addEdge(1, 2, 2.0);
    graph.addEdge(1, 3, 4.0);
    graph.addEdge(2, 3, 3.0);
    graph.addEdge(3, 4, 2.5);

    // Ejecutar Dijkstra
    dijkstraWithFibonacci(graph, 0);

    // Reconstruir y mostrar el camino más corto desde 0 a 4
    std::map<int, int> previos = {{0, -1}, {1, 0}, {2, 0}, {3, 2}, {4, 3}};
    std::vector<int> camino = reconstruirCamino(previos, 4);

    std::cout << "Camino más corto desde 0 hasta 4: ";
    for (int nodo : camino) {
        std::cout << nodo << " ";
    }
    std::cout << "\n";
}

int main() {
    testDijkstraWithFibonacci();
    return 0;
}