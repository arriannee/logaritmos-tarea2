#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>

#include "fibonacci.cpp" // Incluir la implementación de Fibonacci Heap

using namespace std;

void dijkstraWithFibonacciHeap(Graph& graph, int raiz) {
    int n = graph.nodes.size();
    vector<double> distancias(n, numeric_limits<double>::infinity());
    vector<int> previos(n, -1);
    vector<FibonacciNode*> nodePointers(n, nullptr);

    distancias[raiz] = 0;

    FibonacciHeap Q;
    FibonacciNode* raizNode = new FibonacciNode(raiz, 0.0);
    Q.insert(raizNode);
    nodePointers[raiz] = raizNode;

    for (const auto& nodePair : graph.nodes) {
        int nodeId = nodePair.first;
        if (nodeId != raiz) {
            FibonacciNode* node = new FibonacciNode(nodeId, numeric_limits<double>::infinity());
            Q.insert(node);
            nodePointers[nodeId] = node;
        }
    }

    while (!Q.empty()) {
        FibonacciNode* minNode = Q.extractMin();
        int v = minNode->id;
        double distV = minNode->key;

        for (const auto& uPair : graph.nodes[v].neighbors) {
            const Edge& u = uPair.second;
            double alt = distV + u.weight;

            if (distancias[u.end] > alt) {
                distancias[u.end] = alt;
                previos[u.end] = v;
                Q.decreaseKey(nodePointers[u.end], alt);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        cout << "Nodo " << i << ": distancia = " << distancias[i] << ", previo = " << previos[i] << endl;
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
    Graph grafo1 = construirGrafo(100, 200); // 10 nodos, 12 aristas
    dijkstraWithFibonacciHeap(grafo1, 0);
    
    

    cout << "PRUEBA 2: 2^10 nodos, 2^12 aristas \n";
    Graph grafo2 = construirGrafo(pow(2,10), pow(2,12));
    dijkstraWithFibonacciHeap(grafo2, 0);
}

int main() {
    testDijkstraWithFibonacci(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}