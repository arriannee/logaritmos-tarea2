#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <fstream>

#include "fibonacci.cpp"

using namespace std;


pair<vector<double>, vector<int>> dijkstraWithFibonacciHeap(Graph& graph, int raiz) {
    vector<double> distancias(graph.nodes.size(), numeric_limits<double>::infinity());
    vector<int> previos(graph.nodes.size(), -1);

    distancias[raiz] = 0;
    previos[raiz] = -1;

    FibonacciHeap Q;
    unordered_map<int, std::unique_ptr<FibonacciNode>> nodeMap;

    for (auto& node : graph.nodes) {
        int nodeId = node.first;
        double distance = (nodeId == raiz) ? 0 : numeric_limits<double>::infinity();
        nodeMap[nodeId] = std::make_unique<FibonacciNode>(nodeId, distance);
        Q.insert(nodeMap[nodeId]);
    }

    while (!Q.empty()) {
        std::unique_ptr<FibonacciNode> minNode = Q.extractMin();
        if (!minNode) {
            std::cout << "Error: extractMin devolvió nullptr. Terminando la ejecución." << std::endl;
            break;
        }

        int v = minNode->id;
        std::cout << "Procesando nodo " << v << " con distancia " << distancias[v] << std::endl;

        for (const auto& u : graph.nodes[v].neighbors) {
            int uId = u.first;
            double aristaUV = u.second.weight;
            double alt = distancias[v] + aristaUV;
            std::cout << "Procesando vecino " << uId << " con arista peso " << aristaUV << " y distancia alternativa " << alt << std::endl;

            if (alt < distancias[uId]) {
                distancias[uId] = alt;
                previos[uId] = v;
                Q.decreaseKey(nodeMap[uId].get(), alt);
                std::cout << "Actualizando nodo " << uId << " con nueva distancia " << alt << std::endl;
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

// Función para probar Dijkstra con cola de Fibonacci en un grafo grande
void testDijkstraWithFibonacci() {
    cout << "PRUEBA 1: 2^10 nodos, 2^12 aristas \n";
    Graph grafo1 = construirGrafo(10, 12); // 10 nodos, 12 aristas
    auto resultado = dijkstraWithFibonacciHeap(grafo1, 0);

    // Imprimir resultados de Dijkstra
    imprimirResultadosDijkstraEnArchivo(resultado, "resultados_dijkstra_fibonacci.txt");
}

int main() {
    testDijkstraWithFibonacci(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}
