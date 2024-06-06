#include <vector>
#include <map>
#include <iostream>
#include <random>
#include <tuple>
#include <set>

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
        // nodes[start].matrix[end].emplace_back(end, start, weight);
    }
};

// Función para construir un grafo aleatorio con numNodos nodos y numAristas aristas
Graph construirGrafo(int numNodos, int numAristas) {

    // Máximo número de aristas en un grafo no dirigido
    int maxAristas = (numNodos * (numNodos - 1)) / 2;
    if (numAristas > maxAristas) {
        cout << "Número de aristas excede el máximo posible para un grafo no dirigido. Usando " << maxAristas << " aristas en su lugar." << endl;
        numAristas = maxAristas;
    }

    mt19937 gen(random_device{}());
    uniform_real_distribution<double> distribucion(0.0, 1.0);

    Graph grafo;

    // Añadimos numNodos nodos al grafo
    for (int i = 0; i < numNodos; ++i) {
        grafo.addNode(i);
    }

    // Conectar cada nodo con un nodo anterior para asegurar conectividad
    for (int i = 1; i < numNodos; ++i) {
        uniform_int_distribution<> dis1(0, i - 1);
        int randomNode = dis1(gen);
        double weight = distribucion(gen);
        grafo.addEdge(i, randomNode, weight);
    }

    // Añadir las numAristas restantes
    set<pair<int, int>> aristas;
    for (int j = 0; j < numAristas - (numNodos - 1); ++j) {
        int nodo1, nodo2;
        do {
            uniform_int_distribution<> dis2(0, numNodos - 1);
            nodo1 = dis2(gen);
            nodo2 = dis2(gen);
        } while (nodo1 == nodo2 || aristas.count({min(nodo1, nodo2), max(nodo1, nodo2)}));
        double weight = distribucion(gen);
        grafo.addEdge(nodo1, nodo2, weight);
        aristas.insert({min(nodo1, nodo2), max(nodo1, nodo2)});
    }

    return grafo;
}