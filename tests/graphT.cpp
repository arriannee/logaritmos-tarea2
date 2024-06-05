#include <iostream>
#include <random>
#include <vector>
#include <map>
using namespace std;

// Definición de un nodo
struct Node {
    int id;                             // Identificación del nodo (no necesariamente es un número)
    
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
};