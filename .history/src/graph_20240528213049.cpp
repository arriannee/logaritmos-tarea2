#include <vector>
#include <map>
#include <iostream>

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
    int id;                         // Identificación del nodo (no necesariamente es un número)
    map<int, vector<Edge>> matrix;  // El int identifica al nodo, el vector contiene las aristas

    // Constructor por defecto necesario para map
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
        nodes[start].matrix[end].emplace_back(start, end, weight);
    }

    // Obtener los vecinos de un nodo
    vector<Edge> getNeighbors(int node) {
        vector<Edge> neighbors;
        for (const auto& pair : nodes[node].matrix) {
            for (const auto& edge : pair.second) {
                neighbors.push_back(edge);
            }
        }
        return neighbors;
    }
};
