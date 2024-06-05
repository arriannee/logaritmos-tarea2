#include "heap.cpp"

void dijkstraWithHeap(Graph& graph, int raiz){
    // 1. Definimos dos arreglos de tamaño |V|, distancias y previos.
    vector<double> distancias;
    vector<int> previos;
    double infinito = numeric_limits<double>::infinity();

    // 3. Definimos la distancia del nodo raíz como 0, su nodo previo como −1, 
    distancias[raiz] = 0;
    previos[raiz] = -1;
    // y agregamos el par (distancia = 0, nodo = raíz) a Q.
    Heap Q;
    Node raiz(raiz);
    tuple<double, Node> par(0, raiz);
    Q.insertHeap(par);

    // 4. Por cada nodo v distinto de la raíz en el grafo:
    for (const auto& node : graph.nodes){
        // Definimos distancias[v] como infinita y previos[v] como indefinido.
        distancias[node.first] = infinito;
        previos[node.first] = -1;

        // Agregamos el par (distancia = ∞, nodo = v) a Q.
        tuple<double, Node> par(infinito, node.second);
        Q.insertHeap(par);
    }

    // 6. Mientras Q no se encuentre vacío, repetimos:
    while(!Q.pares.empty()){
        // Obtenemos el par (d, v) con menor distancia en Q y lo eliminamos.
        tuple<double, Node> dv = Q.extractMin();
        // Por cada vecino u del nodo v:
        for(const auto& u : graph.nodes[get<1>(dv).id].neighbors){
            double distanciasU = distancias[u.first];
            double distanciasV = distancias[get<1>(dv).id];
            double aristaUV = u.second.weight;
            // Si la distancia guardada para u (distancias[u]) es mayor a la distancia guardada para v (distancias[v]) más el peso de la arista (u, v) 
            if (distanciasU > (distanciasV + aristaUV)){
                // actualizamos el valor de la distancia de u, 
                distancias[u.first] = (distanciasV + aristaUV);
                // guardamos v como el nodo previo de u y 
                previos[u.first] =  get<1>(dv).id;
                // actualizamos la distancia del par que representa al nodo u en Q utilizando decreaseKey
            }
        }
    }
    
    // 7. Retornamos el arreglo de previos y distancias.
    return;
}

    
