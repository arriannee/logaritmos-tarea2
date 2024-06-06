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

        // Por cada vecino u del nodo v:
        for (const auto& u : graph.nodes[v].neighbors) {
            int u_id = u.first;
            double distanciasU = distancias[u_id];
            double distanciasV = distancias[v];
            double aristaUV = u.second.weight;
            double alt = distanciasV + aristaUV;

            // Si la distancia guardada para u (distancias[u]) es mayor a la distancia guardada para v (distancias[v]) más el peso de la arista (u, v) 
            if (distanciasU > alt) {
                // Actualizamos el valor de la distancia de u,
                distancias[u_id] = alt;
                // Guardamos v como el nodo previo de u 
                previos[u_id] = v;
                // Actualizamos la distancia del par que representa al nodo u en Q utilizando decreaseKey
                ptrdiff_t uIndex = graph.nodes[u.second.end].par - &Q.pares[0];
                Q.decreaseKey(graph, uIndex, alt);
            }
        }
    }

    // Retornamos el arreglo de previos y distancias.
    return {distancias, previos};
}

// Función para imprimir el grafo en un archivo de texto
void imprimirGrafoEnArchivo(const Graph& grafo, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& node : grafo.nodes) {
            outFile << "Nodo " << node.first << ":\n";
            for (const auto& neighbor : node.second.neighbors) {
                outFile << "  Vecino " << neighbor.first << " con peso " << neighbor.second.weight << "\n";
            }
        }
        outFile.close();
    } else {
        cerr << "No se pudo abrir el archivo " << filename << " para escritura.\n";
    }
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
void testDijkstraWithHeap() {
    int potenciaNodos = 10;
    int potenciaAristas = 12;
    Graph grafo1 = construirGrafo(potenciaNodos, potenciaAristas);

    // Imprimir el grafo generado
    imprimirGrafoEnArchivo(grafo1, "grafo.txt");

    auto resultado = dijkstraWithHeap(grafo1, 0);

    // Imprimir resultados de Dijkstra
    imprimirResultadosDijkstraEnArchivo(resultado, "resultados_dijkstra.txt");
}

int main() {
    testDijkstraWithHeap(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}