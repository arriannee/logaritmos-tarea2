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
    int num;                                  // Número del nodo
    std::map<int, std::vector<Edge>> matrix;  // El int identifica al nodo, el vector contiene las aristas

    Node(int num) : num(num) {}
};

// Definición de un grafo
struct graph{
    /* data */
};

