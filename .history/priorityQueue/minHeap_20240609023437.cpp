#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <queue>

#include "../graph.cpp"

using namespace std;

// Implementación confiable de Dijkstra usando un min-heap de c++ para comparación
pair<vector<double>, vector<int>> dijkstraWithMinHeap(const Graph& graph, int raiz) {
    vector<double> distancias(graph.nodes.size(), numeric_limits<double>::infinity());
    vector<int> previos(graph.nodes.size(), -1);

    distancias[raiz] = 0;
    previos[raiz] = -1;

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    pq.emplace(0, raiz);

    while (!pq.empty()) {
        double dist = pq.top().first;
        int v = pq.top().second;
        pq.pop();

        if (dist > distancias[v]) {
            continue;
        }

        for (const auto& u : graph.nodes.at(v).neighbors) {
            int uId = u.first;
            double aristaUV = u.second.weight;
            double alt = distancias[v] + aristaUV;

            if (alt < distancias[uId]) {
                distancias[uId] = alt;
                previos[uId] = v;
                pq.emplace(alt, uId);
            }
        }
    }

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

// Función para probar Dijkstra con min-heap 
void testDijkstraWithMinHeap(int i, int j) {
    const int numExperiments = 50;
    ofstream timingFile("tiempos_dijkstra_heap.txt", ios::app); 

    if (timingFile.is_open()) {
        for (int experiment = 0; experiment < numExperiments; ++experiment) {
            cout << "Ejecutando experimento " << experiment + 1 << " para v = 2^" << i << " y e = 2^" << j << "\n";
            Graph grafo = construirGrafo(i, j); // Asumiendo que esta función ya está definida
            auto start = chrono::high_resolution_clock::now();
            auto resultado = dijkstraWithMinHeap(grafo, 0);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;

            timingFile << "Experimento " << experiment + 1 << " para v = 2^" << i << " y e = 2^" << j << ": " << elapsed.count() << " segundos\n";

            // Imprimir resultados de Dijkstra en archivos separados (comentado para evitar llenar de archivos)
            // string filename = "resultados_dijkstra_heap_" + to_string(i) + "_" + to_string(j) + "_" + to_string(experiment + 1) + ".txt";
            // imprimirResultadosDijkstraEnArchivo(resultado, filename);
        }
        timingFile.close();
    } else {
        cerr << "No se pudo abrir el archivo tiempos_dijkstra_heap.txt para escritura.\n";
    }
}

int main() {
    vector<pair<int, int>> configuraciones = {{10, 16}, {10, 17}, {10, 18}, {12, 16}, {12, 17}, {12, 18}, {12,19}, {12, 20}, {12, 21}, {12, 22}, {14, 16}, {14, 17}, {14, 18}, {14,19}, {14, 20}, {14, 21}, {14, 22}};
    
    int numPrueba = 1;

    for (const auto& config : configuraciones) {
        int i = config.first;
        int j = config.second;
        cout << " \nPrueba " << numPrueba++ << ": v = 2^" << i << " y e = 2^" << j << "\n";
        testDijkstraWithMinHeap(i, j);
    }

    cout << "FIN\n";
    return 0;
}