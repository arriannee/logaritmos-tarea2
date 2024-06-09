#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>

#include "fibonacci.cpp" // Incluir la implementación de Fibonacci Heap

using namespace std;

void dijkstraWithFibonacci(Graph& graph, int root) {
    unordered_map<int, FibonacciNode*> nodeMap;
    FibonacciHeap fibHeap;

    unordered_map<int, double> distances;
    unordered_map<int, int> previous;

    // Inicializar distancias y nodos previos
    for (const auto& node : graph.nodes) {
        distances[node.first] = numeric_limits<double>::infinity();
        previous[node.first] = -1;
    }
    distances[root] = 0.0;

    // Insertar todos los nodos en la Cola de Fibonacci
    for (const auto& node : graph.nodes) {
        FibonacciNode* fibNode = new FibonacciNode(node.first, distances[node.first]);
        fibHeap.insert(fibNode);
        nodeMap[node.first] = fibNode;
    }

    // Mientras la Cola de Fibonacci no esté vacía
    while (!fibHeap.empty()) {
        FibonacciNode* u = fibHeap.extractMin();
        if (distances[u->id] == numeric_limits<double>::infinity()) {
            delete u; // Liberar el nodo extraído
            break;
        }
        cout << "Nodo actual: " << u->id << ", distancia: " << distances[u->id] << "\n"; // Depuración
        for (const auto& neighbor : graph.nodes[u->id].matrix) {
            int v = neighbor.first;
            for (const Edge& edge : neighbor.second) {
                double alt = distances[u->id] + edge.weight;
                if (alt < distances[v]) {
                    cout << "Actualizando distancia del nodo " << v << " de " << distances[v] << " a " << alt << "\n"; // Depuración
                    distances[v] = alt;
                    previous[v] = u->id;
                    fibHeap.decreaseKey(nodeMap[v], alt);
                }
            }
        }
        cout << "Eliminando nodo: " << u->id << "\n"; // Depuración
        delete u; // Liberar el nodo extraído
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

    // Liberar memoria de los nodos restantes en el map
    for (const auto& pair : nodeMap) {
        delete pair.second;
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
    
    /*
    cout << "PRUEBA 1: 10 nodos, 20 aristas \n";
    Graph grafo1 = construirGrafo(10, 20); // 10 nodos, 12 aristas
    imprimirGrafo(grafo1);
    dijkstraWithFibonacci(grafo1, 0);
    */
    

    cout << "PRUEBA 2: 2^10 nodos, 2^12 aristas \n";
    Graph grafo2 = construirGrafo(pow(2,10), pow(2,12));
    imprimirGrafo(grafo2);
    dijkstraWithFibonacci(grafo2, 0);
}

int main() {
    testDijkstraWithFibonacci(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}