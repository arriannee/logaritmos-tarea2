#include <vector>
#include <map>

// Definición de una arista
struct Edge {
    int start;      // Nodo de origen
    int end;        // Nodo de destino
    double weight;  // Peso de la arista

    Edge(int start, int end, double weight) : start(start), end(end), weight(weight) {}
};

// Definición de un nodo
struct Node {
    int id;                                  // Identificación del nodo (no necesariamente es un número)
    std::map<int, std::vector<Edge>> matrix;  // El int identifica al nodo, el vector contiene las aristas

    Node(int id) : id(id) {}
};

// Definición de un grafo
struct graph{
    /* data */
};

