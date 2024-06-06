#include <vector>
#include <map>
#include <iostream>
#include <random>

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
    int id;                                   // Identificación del nodo (no necesariamente es un número)
    map<int, std::vector<Edge>> matrix;  // El int identifica al nodo, el vector contiene las aristas

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
        nodes[start].matrix[end].emplace_back(start, end, weight);
    }
};

// Función para construir un grafo aleatorio con numeroi nodos y 2^numeroj aristas
Graph construirGrafo(int numNodos, int numAristas) {
    // Para generar pesos aleatorios y uniformes dentro del rango (0..1]
    mt19937 gen(random_device{}());
    uniform_real_distribution<double> distribucion(0.0, 1.0);

    Graph grafo;

    // Añadimos numeroi nodos al grafo
    for (int i = 0; i < numNodos; ++i) {
        grafo.addNode(i);
    }

    // Para cada nodo i, se lo conecta con un nodo aleatorio elegido en [0..i-1]
    for (int i = 1; i < numNodos; ++i) {
        uniform_int_distribution<> dis1(0, i - 1);
        int randomNode = dis1(gen);
        double weight = distribucion(gen);
        grafo.addEdge(i, randomNode, weight);
    }

    // Añadir las numAristas restantes
    for (int j = 0; j < numAristas - (numNodos - 1); ++j) {
        uniform_int_distribution<> dis2(0, numNodos - 1);
        int nodo1 = dis2(gen);
        int nodo2 = dis2(gen);
        while (nodo1 == nodo2) {
            nodo2 = dis2(gen);
        }
        double weight = distribucion(gen);
        grafo.addEdge(nodo1, nodo2, weight);
    }

    return grafo;
}