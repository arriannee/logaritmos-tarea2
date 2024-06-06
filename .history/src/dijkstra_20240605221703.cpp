#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>

#include "fibonacci.cpp"

using namespace std;

pair<vector<double>, vector<int>> dijkstraWithFibonacciHeap(Graph& graph, int raiz) {
    // 1. Definimos dos arreglos de tamaño |V|, distancias y previos.
    vector<double> distancias(graph.nodes.size(), numeric_limits<double>::infinity());
    vector<int> previos(graph.nodes.size(), -1);

    // 3. Definimos la distancia del nodo raíz como 0, su nodo previo como −1,
    distancias[raiz] = 0;
    previos[raiz] = -1;

    // Inicializamos la cola de Fibonacci y agregamos la raíz
    FibonacciHeap Q;
    unordered_map<int, FibonacciNode*> nodeMap; // Mapeo para acceder a los nodos en la cola de Fibonacci
    nodeMap[raiz] = new FibonacciNode(raiz, 0);
    Q.insert(nodeMap[raiz]);

    // 4. Por cada nodo v distinto de la raíz en el grafo:
    for (const auto& node : graph.nodes) {
        if (node.first != raiz) {
            // Definimos distancias[v] como infinita y previos[v] como indefinido.
            distancias[node.first] = numeric_limits<double>::infinity();
            previos[node.first] = -1;

            // Agregamos el nodo a la cola de Fibonacci con distancia infinita
            nodeMap[node.first] = new FibonacciNode(node.first, distancias[node.first]);
            Q.insert(nodeMap[node.first]);
        }
    }

    // 6. Mientras Q no se encuentre vacío, repetimos:
    while (!Q.empty()) {
        // Obtenemos el nodo con la menor distancia en Q y lo eliminamos.
        FibonacciNode* minNode = Q.extractMin();
        int v = minNode->id;
        
        // Por cada vecino u del nodo v:
        for (const auto& u : graph.nodes[v].neighbors) {
            int uId = u.first;
            double aristaUV = u.second.weight;
            double alt = distancias[v] + aristaUV;

            // Si la distancia guardada para u (distancias[u]) es mayor a la distancia guardada para v (distancias[v]) más el peso de la arista (u, v)
            if (alt < distancias[uId]) {
                // Actualizamos el valor de la distancia de u,
                distancias[uId] = alt;
                // Guardamos v como el nodo previo de u y
                previos[uId] = v;
                // Actualizamos la distancia del nodo u en Q utilizando decreaseKey
                Q.decreaseKey(nodeMap[uId], alt);
            }
        }
    }

    // 7. Retornamos el arreglo de previos y distancias.
    return {distancias, previos};
}


// Función para probar Dijkstra con cola de Fibonacci en un grafo grande
void testDijkstraWithFibonacci() {
    
    
    cout << "PRUEBA 1: 2^10 nodos, 2^12 aristas \n";
    Graph grafo1 = construirGrafo(10, 12); // 10 nodos, 12 aristas
    auto resultado = dijkstraWithFibonacciHeap(grafo1, 0);

    // Imprimir distancias
    cout << "Distancias:\n";
    for (size_t i = 0; i < resultado.first.size(); ++i) {
        cout << "Nodo " << i << ": " << resultado.first[i] << "\n";
    }

    // Imprimir previos
    cout << "Previos:\n";
    for (size_t i = 0; i < resultado.second.size(); ++i) {
        cout << "Nodo " << i << ": " << resultado.second[i] << "\n";
    }

}

int main() {
    testDijkstraWithFibonacci(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}