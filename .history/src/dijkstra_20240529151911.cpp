#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>

#include "fibonacci.cpp" // Incluir la implementación de Fibonacci Heap

using namespace std;

// Función para ejecutar el algoritmo de Dijkstra utilizando una Cola de Fibonacci
void dijkstraWithFibonacci(Graph& graph, int root) {
    map<int, FibonacciNode*> nodeMap; // Mapa para almacenar los nodos de Fibonacci
    FibonacciHeap fibHeap; // Cola de Fibonacci

    map<int, double> distances; // Mapa para almacenar las distancias
    map<int, int> previous; // Mapa para almacenar los nodos previos en el camino más corto

    // Inicializar distancias y nodos previos
    for (const auto& node : graph.nodes) {
        distances[node.first] = numeric_limits<double>::infinity(); // Distancia inicial infinita
        previous[node.first] = -1; // Nodo previo inicial -1 (ninguno)
    }
    distances[root] = 0.0; // La distancia al nodo raíz es 0

    // Insertar todos los nodos en la Cola de Fibonacci
    for (const auto& node : graph.nodes) {
        FibonacciNode* fibNode = new FibonacciNode(node.first, distances[node.first]);
        fibHeap.insert(fibNode);
        nodeMap[node.first] = fibNode;
    }

    // Mientras la Cola de Fibonacci no esté vacía
    while (!fibHeap.empty()) {
        FibonacciNode* u = fibHeap.extractMin(); // Extraer el nodo con la distancia mínima
        if (distances[u->id] == numeric_limits<double>::infinity()) {
            // No hay más nodos alcanzables, salir del bucle
            break;
        }
        for (const auto& neighbor : graph.nodes[u->id].matrix) {
            int v = neighbor.first; // Nodo vecino
            for (const Edge& edge : neighbor.second) {
                double alt = distances[u->id] + edge.weight; // Calcular la distancia alternativa
                if (alt < distances[v]) {
                    distances[v] = alt; // Actualizar la distancia más corta
                    previous[v] = u->id; // Actualizar el nodo previo
                    fibHeap.decreaseKey(nodeMap[v], alt); // Disminuir la clave en la Cola de Fibonacci
                }
            }
        }
    }

    // Imprimir los resultados
    cout << "Distancias desde el nodo " << root << ":\n";
    for (const auto& d : distances) {
        cout << "Nodo " << d.first << ": " << d.second << "\n";
    }

    cout << "\nPrevios:\n";
    for (const auto& p : previous) {
        cout << "Nodo " << p.first << ": " << p.second << "\n";
    }
}

// Función para reconstruir el camino más corto desde el nodo raíz hasta el nodo objetivo
vector<int> reconstruirCamino(const map<int, int>& previos, int objetivo) {
    vector<int> camino;
    for (int en = objetivo; en != -1; en = previos.at(en)) {
        camino.push_back(en); // Añadir el nodo actual al camino
    }
    reverse(camino.begin(), camino.end()); // Invertir el camino para obtener el orden correcto
    return camino;
}


// Función para probar Dijkstra con cola de Fibonacci en un grafo grande
void testDijkstraWithFibonacci() {
    cout << "PRUEBA 1: 10 nodos, 20 aristas \n";
    Graph grafo1 = construirGrafo(10, 20); // 10 nodos, 12 aristas
    dijkstraWithFibonacci(grafo1, 0);

    cout << "PRUEBA 2: 100 nodos, 400 aristas";
    Graph grafo2 = construirGrafo(100, 200);
    dijkstraWithFibonacci(grafo2, 0);
}

int main() {
    testDijkstraWithFibonacci(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}