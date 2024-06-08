#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <tuple>
#include <limits>

using namespace std;

// Definición de una arista
struct Edge {
    int start;      // Nodo de origen
    int end;        // Nodo de destino
    double weight;  // Peso de la arista

    Edge() : start(-1), end(-1), weight(0.0) {} // Constructor por defecto
    Edge(int start, int end, double weight) : start(start), end(end), weight(weight) {}
};

// Definición de un nodo
struct Node {
    int id;                             // Identificación del nodo (no necesariamente es un número)
    map<int, Edge> neighbors;           // El int identifica al nodo vecino, edge identifica el arista entre el nodo y su vecino
    int Qpos;                           // Posición en el arreglo Q

    // Constructor por defecto necesario para std::map
    Node() : id(-1), Qpos(-1) {}

    Node(int id) : id(id), Qpos(-1) {}
};

// Definición de un grafo
struct Graph {
    map<int, Node> nodes; // Mapa de nodos por su identificación

    // Agregar un nodo al grafo
    void addNode(int id) {
        nodes.emplace(id, Node(id));
    }

    // Agregar una arista al grafo
    void addEdge(int start, int end, double weight) {
        nodes[start].neighbors[end] = Edge(start, end, weight);
    }
};

Graph construirGrafo(int numeroi, int numeroj){
    // Para generar pesos aleatorios y uniformes dentro del rango (0..1]
    mt19937 gen(random_device{}());
    uniform_real_distribution<double> distribucion(0.0, 1.0);

    Graph grafo;

    // Añadimos 2^numeroi nodos al grafo
    int numNodos = pow(2, numeroi);
    for(int i = 0; i < numNodos; i++){
        grafo.addNode(i);
    }

    // Para cada nodo i, se lo conecta con un nodo aleatorio elegido en [0..i-1]
    for(int i = 1; i < numNodos; i++){
        uniform_int_distribution<> dis1(0, i-1);
        int vecino = dis1(gen);
        double peso = distribucion(gen);
        grafo.addEdge(i, vecino, peso);
        grafo.addEdge(vecino, i, peso);  // Añadir la arista en la dirección opuesta
    }
    // Saliendo de este for se garantiza la conectividad

    // Añadir las 2^numeroj aristas adicionales
    int totalAristas = pow(2, numeroj);
    for(int j = 0; j < totalAristas; j++){
        uniform_int_distribution<> dis2(0, numNodos - 1);
        int nodo1 = dis2(gen);
        int nodo2 = dis2(gen);
        // Para que no sean el mismo nodo
        while(nodo1 == nodo2){
            nodo2 = dis2(gen);
        }
        double peso = distribucion(gen);
        grafo.addEdge(nodo1, nodo2, peso);
        grafo.addEdge(nodo2, nodo1, peso);  // Añadir la arista en la dirección opuesta
    }
    
    std::cout << "Grafo construido" << std::endl;
    
    return grafo;
}

// Función para imprimir un grafo
void imprimirGrafo(const Graph& grafo) {
    for (const auto& node : grafo.nodes) {
        cout << "Nodo " << node.first << ":\n";
        for (const auto& neighbor : node.second.neighbors) {
            cout << "  Vecino " << neighbor.first << " con peso " << neighbor.second.weight << "\n";
        }
    }
}

// Definición de la estructura Heap
struct Heap{
    vector<tuple<double, Node>> pares;

    // Se tiene en la estructura de un heap, siendo i el índice de un nodo:
    // El padre de un nodo siempre está en (i-1)/2
    int parent(int i){
        return ((i-1)/2);
    }

    // El hijo izquierdo de un nodo siempre está en (2i)+1
    int left(int i){
        return ((2*i)+1);
    }

    // El hijo derecho de un nodo siempre está en (2i)+2
    int right(int i){
        return ((2*i)+2);
    }
    
    // El siguiente es un heapify que revisa hacia abajo
    // i: índice del nodo donde se hará heapify
    void heapify1(Graph& grafo, int i){
        int l = left(i);
        int r = right(i);
        int min = i;
        // Si el hijo izquierdo está dentro del heap y es menor que el actual
        if((l < pares.size()) && (get<0>(pares[l]) < get<0>(pares[min]))){
            // El hijo izquierdo es más pequeño
            min = l;
        }
        // Si el hijo derecho está dentro del heap y es menor que el actual
        if ((r < pares.size()) && (get<0>(pares[r]) < get<0>(pares[min]))){
            min = r;
        }
        // Si el minimo es distinto que al inicio
        if (min != i){    
            // Se debe intercambiar par[i] con el nuevo mínimo
            swap(pares[i], pares[min]);      
            // Actualizamos las Qpos de los nodos
            int idi = get<1>(pares[i]).id;
            int idmin = get<1>(pares[min]).id; 
            grafo.nodes[idi].Qpos = i;
            grafo.nodes[idmin].Qpos = min;
            // Y llamamos a heapify de su hijo por si esto causó conflicto
            heapify1(grafo, min);
        }      
    }

    // El siguiente es un heapify que revisa hacia arriba
    // i: índice del nodo donde se hará heapify
    void heapify2(Graph& grafo, int i){
        int p = parent(i);
        // Si el padre está dentro del vector
        if(p >= 0) {
            // Si el valor actual es menor que el del padre
            if(get<0>(pares[i]) < get<0>(pares[p])){  
                // Se debe intercambiar el actual por el padre
                swap(pares[i], pares[p]);
                // Actualizamos las Qpos de los nodos
                int idi = get<1>(pares[i]).id;
                int idp = get<1>(pares[p]).id;
                grafo.nodes[idi].Qpos = i;
                grafo.nodes[idp].Qpos = p;
                // Llamamos a heapify de su padre por si esto causó conflicto
                heapify2(grafo, p);
            }
        }
    }

    void insertHeap(Graph& grafo, tuple<double, Node> nuevoPar){
        // Se inserta el nuevo par al final
        pares.push_back(nuevoPar);
        int lastPair = pares.size() - 1;
        // La posición del nuevo par en Q será el final del vector de pares
        grafo.nodes[get<1>(nuevoPar).id].Qpos = lastPair;
        // Como lo anterior puede romper la estructura, se hace heapify
        heapify2(grafo, lastPair);
    }

    tuple<double, Node> extractMin(Graph& grafo){
        // El mínimo es el primer elemento en la cola de prioridad
        tuple<double, Node> min = pares[0];
        // Se elimina la posición del elemento que se va a extraer
        int idMin = get<1>(min).id;
        grafo.nodes[idMin].Qpos = -1;
        // Intercambiamos el primer por el último elemento
        swap(pares[0], pares[pares.size()-1]);
        // Eliminamos el elemento que queremos extraer
        pares.pop_back();    
        if (!pares.empty()) {
            // Actualizamos la posición del que ahora quedó primero
            int idNew = get<1>(pares[0]).id;
            grafo.nodes[idNew].Qpos = 0;
            // Lo anterior puede haber roto la condición del Heap, así que se llama a Heapify sobre su primer elemento
            heapify1(grafo, 0);
        }
        // Retornamos el elemento menor
        return min;
    }

    // Recibe el índice y el valor que se desea colocar en él
    void decreaseKey(Graph& grafo, int i, double k){
        // Se cambia el valor del índice i por k
        pares[i] = make_tuple(k, get<1>(pares[i])); 
        // Lo anterior puede haber roto la condición del Heap, así que se revisará toda la rama hacia arriba    
        // Mientras el elemento i tenga un padre y el valor del padre sea mayor que el de i
        while ((i >= 1) && (get<0>(pares[parent(i)]) > get<0>(pares[i]))){
            int parentIndex = parent(i);
            // Se intercambia la posición del elemento i y su padre
            swap(pares[i], pares[parentIndex]);    
            // Se actualizan las posiciones de los pares intercambiados  
            int iId = get<1>(pares[i]).id;
            int idparentIndex = get<1>(pares[parentIndex]).id;
            grafo.nodes[iId].Qpos = i;
            grafo.nodes[idparentIndex].Qpos = parentIndex;
            // Se revisa nuevamente hacia arriba
            i = parentIndex;
        }
    }
};

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
    Graph grafo1 = construirGrafo(10, 12); // 10 nodos, 12 aristas
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
