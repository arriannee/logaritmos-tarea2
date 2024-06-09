#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "../graph.cpp"

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

    // 6. Mientras Q no se encuentre vacío, repetimos:
    while (!Q.empty()) {
        // Obtenemos el nodo con la menor distancia en Q y lo eliminamos.
        FibonacciNode* minNode = Q.extractMin();
        if (!minNode) {
            break;
        }

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
                auto it = graph.nodes.find(uId);
                if (it != graph.nodes.end()) {
                    Q.decreaseKey(nodeMap[uId], alt);
                }
            }
        }

        // Liberar memoria del nodo procesado
        delete minNode;
    }
    
    /*
    // Liberar memoria de todos los nodos restantes en nodeMap
    for (auto& pair : nodeMap) {
        if (pair.second) {
            delete pair.second;
            pair.second = nullptr;
        }
    }
    */

    // 7. Retornamos el arreglo de previos y distancias.
    return {distancias, previos};
}


// Función para imprimir resultados de Dijkstra en un archivo de texto
void imprimirResultadosDijkstraEnArchivo(const pair<vector<double>, vector<int>>& resultado, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        // Imprimir distancias
        outFile << "Distancias:\n";
        for (size_t i = 0; i < resultado.first.size(); ++i) {
            outFile << "Nodo " << i << ": " << resultado.first[i] << "\n";
        }

        // Imprimir previos
        outFile << "Previos:\n";
        for (size_t i = 0; i < resultado.second.size(); ++i) {
            outFile << "Nodo " << i << ": " << resultado.second[i] << "\n";
        }
        outFile.close();
    } else {
        cerr << "No se pudo abrir el archivo " << filename << " para escritura.\n";
    }
}

// Función para probar Dijkstra con cola de Fibonacci en un grafo grande
void testDijkstraWithFibonacci(int i, int j) {
    const int numExperiments = 50;
    ofstream timingFile("tiempos_dijkstra_fibonacci.txt", ios::app); // Abre en modo append para no sobrescribir

    if (timingFile.is_open()) {
        for (int experiment = 0; experiment < numExperiments; ++experiment) {
            cout << "Ejecutando experimento " << experiment + 1 << " para v = 2^" << i << " y e = 2^" << j << "\n";
            Graph grafo = construirGrafo(i, j); // Asumiendo que esta función ya está definida
            auto start = chrono::high_resolution_clock::now();
            auto resultado = dijkstraWithFibonacciHeap(grafo, 0);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;

            timingFile << "Experimento " << experiment + 1 << " para v = 2^" << i << " y e = 2^" << j << ": " << elapsed.count() << " segundos\n";

            // Imprimir resultados de Dijkstra en archivos separados (comentaddo pq se llena de archivos)
            // string filename = "resultados_dijkstra_fibonacci_" + to_string(i) + "_" + to_string(j) + "_" + to_string(experiment + 1) + ".txt";
            // imprimirResultadosDijkstraEnArchivo(resultado, filename);
        }
        timingFile.close();
    } else {
        cerr << "No se pudo abrir el archivo tiempos_dijkstra_fibonacci.txt para escritura.\n";
    }
}

int main() {
    vector<pair<int, int>> configuraciones = {{10, 16}, {10, 17}, {10, 18}, {12, 16}, {12, 17}, {12, 18}, {12,19}, {12, 20}, {12, 21}, {12, 22}, {14, 16}, {14, 17}, {14, 18}, {14,19}, {14, 20}, {14, 21}, {14, 22}};
    
    int numPrueba = 1;

    for (const auto& config : configuraciones) {
        int i = config.first;
        int j = config.second;
        cout << " \nPrueba " << numPrueba++ << ": v = 2^" << i << " y e = 2^" << j << "\n";
        testDijkstraWithFibonacci(i, j);
    }

    cout << "FIN\n";
    return 0;
}
