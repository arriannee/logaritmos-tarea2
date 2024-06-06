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
    for (auto& node : graph.nodes) {
        int nodeId = node.first;
        double distance = (nodeId == raiz) ? 0 : numeric_limits<double>::infinity();
        nodeMap[nodeId] = new FibonacciNode(nodeId, distance);
        Q.insert(nodeMap[nodeId]);
    }

    int iteration = 0;
    // 6. Mientras Q no se encuentre vacío, repetimos:
    while (!Q.empty()) {
        cout << "Iteración: " << iteration++ << " - Tamaño del heap: " << Q.nodeCount << endl;

        // Obtenemos el nodo con la menor distancia en Q y lo eliminamos.
        FibonacciNode* minNode = Q.extractMin();
        if (!minNode) {
            cout << "Error: extractMin devolvió nullptr. Terminando la ejecución." << endl;
            break;
        }

        int v = minNode->id;
        cout << "Procesando nodo " << v << " con distancia " << distancias[v] << endl;

        // Verificar los vecinos del nodo actual
        if (graph.nodes[v].neighbors.empty()) {
            cout << "Nodo " << v << " no tiene vecinos." << endl;
        } else {
            cout << "Nodo " << v << " tiene " << graph.nodes[v].neighbors.size() << " vecinos." << endl;
        }

        // Por cada vecino u del nodo v:
        for (const auto& u : graph.nodes[v].neighbors) {
            int uId = u.first;
            double aristaUV = u.second.weight;
            double alt = distancias[v] + aristaUV;
            cout << "Procesando vecino " << uId << " con arista peso " << aristaUV << " y distancia alternativa " << alt << endl;

            // Si la distancia guardada para u (distancias[u]) es mayor a la distancia guardada para v (distancias[v]) más el peso de la arista (u, v)
            if (alt < distancias[uId]) {
                // Actualizamos el valor de la distancia de u,
                distancias[uId] = alt;
                // Guardamos v como el nodo previo de u y
                previos[uId] = v;
                // Actualizamos la distancia del nodo u en Q utilizando decreaseKey
                auto it = nodeMap.find(uId);
                if (it != nodeMap.end()) {
                    Q.decreaseKey(it->second, alt);
                    cout << "Actualizando nodo " << uId << " con nueva distancia " << alt << endl;
                } else {
                    cout << "Error: Nodo " << uId << " no encontrado en el mapa de nodos" << endl;
                }
            } else {
                cout << "Nodo " << uId << " ya tiene una distancia menor o igual a " << alt << endl;
            }
        }

        // Verificar si el heap sigue teniendo elementos
        cout << "Tamaño del heap después de procesar nodo " << v << ": " << Q.nodeCount << endl;

        // Verificación adicional del estado del heap
        if (iteration > graph.nodes.size() * 2) {
            cout << "Error: Número de iteraciones excedido, posible ciclo o problema en el heap." << endl;
            break;
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