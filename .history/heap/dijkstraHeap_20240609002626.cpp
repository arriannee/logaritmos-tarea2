#include "heap.cpp"

pair<vector<double>, vector<int>> dijkstraWithHeap(Graph& graph, int raiz) {
    int n = graph.nodes.size();
    double infinito = numeric_limits<double>::infinity();
    vector<double> distancias(n, numeric_limits<double>::infinity());
    vector<int> previos(n, -1);

    // Definimos la distancia del nodo raíz como 0, su nodo previo como −1
    distancias[raiz] = 0;
    previos[raiz] = -1;

    // Estructura Heap
    Heap Q;
    Node raizNode(raiz);
    tuple<double, Node> par(0.0, raizNode);
    Q.insertHeap(graph, par);

    // Inicializamos el heap con los otros nodos
    for (const auto& node : graph.nodes) {
        if (node.first != raiz) {
            tuple<double, Node> par(infinito, node.second);
            Q.insertHeap(graph, par);
        }
    }

    // Mientras Q no se encuentre vacío, repetimos:
    while (!Q.pares.empty()) {
        // Obtenemos el par (d, v) con menor distancia en Q y lo eliminamos.
        tuple<double, Node> dv = Q.extractMin(graph);
        int v = get<1>(dv).id;
         double distanciasV = distancias[v];
        // Por cada vecino u del nodo v:
        for (const auto& u : graph.nodes[v].neighbors) {
            int uId = u.first;
            double distanciasU = distancias[uId];       
            double aristaUV = u.second.weight;
            double alt = distanciasV + aristaUV;

            // Si la distancia guardada para u (distancias[u]) es mayor a la distancia guardada para v (distancias[v]) más el peso de la arista (u, v) 
            if (distanciasU > alt) {           
                // Actualizamos el valor de la distancia de u,
                distancias[uId] = alt;
                // Guardamos v como el nodo previo de u 
                previos[uId] = v;
                // Actualizamos
                Q.decreaseKey(graph, graph.nodes[uId].Qpos, alt);

            }
        }
    }

    // Retornamos el arreglo de previos y distancias.
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

// Función para probar Dijkstra con heap binario en un grafo grande
void testDijkstraWithHeap(int i, int j) {
    const int numExperiments = 50;
    ofstream timingFile("tiempos_dijkstra_heap.txt", ios::app); // Abre en modo append para no sobrescribir

    if (timingFile.is_open()) {
        for (int experiment = 0; experiment < numExperiments; ++experiment) {
            cout << "Ejecutando experimento " << experiment + 1 << " para v = 2^" << i << " y e = 2^" << j << "\n";
            Graph grafo = construirGrafo(i, j); // Asumiendo que esta función ya está definida
            auto start = chrono::high_resolution_clock::now();
            auto resultado = dijkstraWithHeap(grafo, 0);
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
        testDijkstraWithHeap(i, j);
    }

    cout << "FIN\n";
    return 0;
}