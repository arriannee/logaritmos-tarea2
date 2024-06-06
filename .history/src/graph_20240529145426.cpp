#include <vector>
#include <map>
#include <iostream>

// Definición de una arista
struct Edge {
    int start;      // Nodo de origen
    int end;        // Nodo de destino
    double weight;  // Peso de la arista

    Edge(int start, int end, double weight) : start(start), end(end), weight(weight) {}
};

// Definición de un nodo
struct Node {
    int id;                                   // Identificación del nodo (no necesariamente es un número)
    std::map<int, std::vector<Edge>> matrix;  // El int identifica al nodo, el vector contiene las aristas

    // Constructor por defecto necesario para std::map
    Node() : id(-1) {}

    Node(int id) : id(id) {}
};

// Definición de un grafo
struct Graph {
    std::map<int, Node> nodes; // Mapa de nodos por su identificación

    // Agregar un nodo al grafo
    void addNode(int id) {
        nodes.emplace(id, Node(id));
    }

    // Agregar una arista al grafo
    void addEdge(int start, int end, double weight) {
        nodes[start].matrix[end].emplace_back(start, end, weight);
    }
};