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

// Función para probar Dijkstra con cola de Fibonacci en un grafo grande
void testDijkstraWithHeap() {
    cout << "PRUEBA 1: 2^10 nodos, 2^12 aristas \n";
    Graph grafo1 = construirGrafo(14, 22); // 10 nodos, 12 aristas
    //imprimirGrafo(grafo1);
    auto resultado = dijkstraWithHeap(grafo1, 0);

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
    testDijkstraWithHeap(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}