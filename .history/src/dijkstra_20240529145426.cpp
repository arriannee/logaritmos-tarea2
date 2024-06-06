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

// Función para probar Dijkstra con cola de Fibonacci
void testDijkstraWithFibonacci() {
    Graph graph; // Crear un grafo

    // Agregar nodos al grafo
    graph.addNode(0);
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);

    // Agregar aristas al grafo
    graph.addEdge(0, 1, 2.5);
    graph.addEdge(0, 2, 1.0);
    graph.addEdge(1, 2, 2.0);
    graph.addEdge(1, 3, 4.0);
    graph.addEdge(2, 3, 3.0);
    graph.addEdge(3, 4, 2.5);

    // Ejecutar Dijkstra desde el nodo 0
    dijkstraWithFibonacci(graph, 0);

    // Reconstruir y mostrar el camino más corto desde 0 a 4
    map<int, int> previos = {{0, -1}, {1, 0}, {2, 0}, {3, 2}, {4, 3}};
    vector<int> camino = reconstruirCamino(previos, 4);

    cout << "Camino más corto desde 0 hasta 4: ";
    for (int nodo : camino) {
        cout << nodo << " ";
    }
    cout << "\n";
}

int main() {
    testDijkstraWithFibonacci(); // Llamar a la función de prueba
    cout << "hola\n"; // Imprimir "hola" para verificar que main se ejecuta correctamente
    return 0;
}