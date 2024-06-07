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

    Edge(int start, int end, double weight) : start(start), end(end), weight(weight) {}
};

// Definición de un nodo
struct Node {
    int id;                             // Identificación del nodo (no necesariamente es un número)
    map<int, Edge> neighbors;           // El int identifica al nodo vecino, edge identifica el arista entre el nodo y su vecino
    tuple<double, Node>* par;           // Puntero al par en la estructura Q que lo representa

    // Constructor por defecto necesario para std::map
    Node() : id(-1) {}

    Node(int id) : id(id) {}
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
        nodes[start].neighbors[end].start = start;
        nodes[start].neighbors[end].end = end;
        nodes[start].neighbors[end].weight = weight;
    }
};

Graph construirGrafo(int numeroi, int numeroj){
    // Para generar pesos aleatorios y uniformes dentro del rango (0..1]
    mt19937 gen(random_device{}());
    uniform_real_distribution<double> distribucion(0.0, 1.0);

    Graph grafo;

    // Añadimos 2^i nodos al grafo
    for(int i=0; i<numeroi; i++){
        grafo.addNode(i);
    }

    // Para cada nodo i, se lo conecta con un nodo aleatorio elegido en [1..i −1]
    for(int i=1; i<numeroi; i++){
        uniform_int_distribution<> dis1(0, i-1);
        grafo.addEdge(grafo.nodes[i].id, grafo.nodes[dis1(gen)].id, distribucion(gen));
    }
    // Saliendo de este for se garantiza la conectividad

    // Añadir las 2^j − (v −1) aristas restantes
    for(int j=0; j<(pow(2,numeroj)-(numeroi-1)); j++){
        uniform_int_distribution<> dis2(0, numeroi);
        int nodo1 = grafo.nodes[dis2(gen)].id;
        int nodo2 = grafo.nodes[dis2(gen)].id;
        // Para que no sean el mismo nodo(?
        while(nodo1 == nodo2){
            nodo2 = grafo.nodes[dis2(gen)].id;
        }
        grafo.addEdge(grafo.nodes[nodo1].id, grafo.nodes[nodo2].id, distribucion(gen));
    }
    return grafo;
}

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
    void heapify1(Graph grafo, int i){
        int l = left(i);
        int r = right(i);
        int min = i;
        // Si el hijo izquierdo está dentro del heap y es menor que el actual
        if((l >= pares.size()) && (get<0>(pares[l]) < get<0>(pares[min]))){
            // El hijo izquierdo es más pequeño
            min = l;
        } else {
            // Si no, el actual es más pequeño
            min = i;
        }
        // Si el hijo derecho está dentro del heap y es menor que el actual
        if ((r <= pares.size()) && (get<0>(pares[r]) < get<0>(pares[min]))){
            min = r;
        }
        // Si el minimo es distinto que al inicio
        if (min != i){      
            // Se debe intercambiar par[i] con el nuevo mínimo
            swap(pares[i], pares[min]);
            // Se intercambian los punteros de los nodos que los representan
            int idActual = get<1>(pares[i]).id;
            int idHijo = get<1>(pares[min]).id;
            grafo.nodes[idActual].par = &pares[min];
            grafo.nodes[idHijo].par = &pares[i];
            // Y llamamos a heapify de su hijo por si esto causó conflicto
            heapify1(grafo, min);
        }      
    }

    // El siguiente es un heapify que revisa hacia arriba
    // i: índice del nodo donde se hará heapify
    void heapify2(Graph grafo, int i){
        int p = parent(i);
        // Si el padre está dentro del vector
        if(p >= 0) {
            // Si el valor actual es menor que el del padre
            if(get<0>(pares[i]) < get<0>(pares[p])){
                // Se debe intercambiar el actual por el padre
                swap(pares[i], pares[p]);
                // Se intercambian los punteros de los nodos que los representan
                int idActual = get<1>(pares[i]).id;
                int idPadre = get<1>(pares[p]).id;
                grafo.nodes[idActual].par = &pares[p];
                grafo.nodes[idPadre].par = &pares[i];
                // Llamamos a heapify de su padre por si esto causó conflicto
                heapify2(grafo, p);
            }
        }
    }

    void insertHeap(Graph grafo, tuple<double, Node> nuevoPar){
        // Se inserta el nuevo par al final
        pares.push_back(nuevoPar);
        // El puntero del nodo apuntará al último elemento de Q (el que se acaba de insertar)
        int idActual = get<1>(nuevoPar).id;
        grafo.nodes[idActual].par = &pares[pares.size() - 1];
        // Como lo anterior puede romper la estructura, se hace heapify
        heapify2(grafo, pares.size()-1);
    }

    tuple<double, Node> extractMin(Graph grafo){
        // El mínimo es el primer elemento en la cola de prioridad
        tuple<double, Node> min = pares[0];
        // Intercambiamos el primer por el último elemento
        pares[0] = pares.back();
        // Eliminamos el elemento que queremos extraer
        pares.pop_back();
        // Se elimina el puntero del elemento mínimo y se actualiza el puntero del último
        int idMin = get<1>(pares[0]).id;
        int idLast = get<1>(pares.back()).id;
        grafo.nodes[idMin].par = NULL;
        grafo.nodes[idLast].par = &pares[0];
        // Lo anterior puede haber roto la condición del Heap, así que se llama a Heapify sobre su primer elemento
        heapify1(grafo, 0);
        // Retornamos el elemento menor
        return min;
    }

    // Recibe el índice y el valor que se desea colocar en él
    void decreaseKey(Graph grafo, int i, double k){
        // Se cambia el valor del índice i por k
        get<0>(pares[i]) = k;
        // Lo anterior puede haber roto la condición del Heap, así que se revisará toda la rama hacia arriba    
        // Mientras el elemento i tenga un padre y el valor del padre sea mayor que el de i
        while ((i >= 1) && (get<0>(pares[parent(i)]) > get<0>(pares[i]))){
            int parentIndex = parent(i);
            // Se intercambia la posición del elemento i y su padre
            swap(pares[i], pares[parentIndex]);
            // Se intercambian los punteros
            int idActual = get<1>(pares[i]).id;
            int idParent = get<1>(pares[parentIndex]).id;
            grafo.nodes[idActual].par = &pares[parentIndex];
            grafo.nodes[idParent].par = &pares[i];
            // Se revisa nuevamente hacia arriba
            i = parent(i);
        }
    }
};

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
    Q.insertHeap(graph, par);

    // 4. Por cada nodo v distinto de la raíz en el grafo:
    for (const auto& node : graph.nodes){
        // Definimos distancias[v] como infinita y previos[v] como indefinido.
        distancias[node.first] = infinito;
        previos[node.first] = -1;

        // Agregamos el par (distancia = ∞, nodo = v) a Q.
        tuple<double, Node> par(infinito, node.second);
        Q.insertHeap(graph, par);
    }

    // 6. Mientras Q no se encuentre vacío, repetimos:
    while(!Q.pares.empty()){
        // Obtenemos el par (d, v) con menor distancia en Q y lo eliminamos.
        tuple<double, Node> dv = Q.extractMin(graph);
        // Por cada vecino u del nodo v:
        for(const auto& u : graph.nodes[get<1>(dv).id].neighbors){
            double distanciasU = distancias[u.first];
            double distanciasV = distancias[get<1>(dv).id];
            double aristaUV = u.second.weight;
            double alt = distanciasV + aristaUV;
            // Si la distancia guardada para u (distancias[u]) es mayor a la distancia guardada para v (distancias[v]) más el peso de la arista (u, v) 
            if (distanciasU > alt){
                // actualizamos el valor de la distancia de u, 
                distancias[u.first] = alt;
                // guardamos v como el nodo previo de u y 
                previos[u.first] =  get<1>(dv).id;
                // actualizamos la distancia del par que representa al nodo u en Q utilizando decreaseKey
                // El término graph.nodes[u.second.end].par es el puntero hacia el par que representa a u en la estructura Q
                // La diferencia de punteros permite calcular el índice en el que se encuentra u dentro de Q
                ptrdiff_t uIndex = graph.nodes[u.second.end].par - &Q.pares[0];
                Q.decreaseKey(graph, uIndex, alt);
            }
        }
    }
    
    // 7. Retornamos el arreglo de previos y distancias.
    return;
}

    
