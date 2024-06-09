#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <set>
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

Graph construirGrafo(int potenciaNodos, int potenciaAristas) {
    int v = 1 << potenciaNodos; // v = 2^potenciaNodos
    int e = 1 << potenciaAristas; // e = 2^potenciaAristas

    // Crear grafo vacío
    Graph grafo;
    for (int i = 0; i < v; ++i) {
        grafo.addNode(i);
    }

    // Inicializar generador de números aleatorios
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    uniform_int_distribution<> node_dis(0, v - 1);

    // Generar un árbol cobertor mínimo para asegurar que el grafo es conexo
    for (int i = 1; i < v; ++i) {
        int random_node = node_dis(gen) % i; // Elegir un nodo aleatorio en el rango [0, i-1]
        double weight = dis(gen);
        grafo.addEdge(i, random_node, weight);
    }

    // Añadir las aristas restantes hasta alcanzar el número requerido
    set<pair<int, int>> existing_edges;
    for (const auto& node : grafo.nodes) {
        for (const auto& neighbor : node.second.neighbors) {
            existing_edges.insert({min(node.first, neighbor.first), max(node.first, neighbor.first)});
        }
    }

    while (existing_edges.size() < static_cast<size_t>(e)) {
        int start = node_dis(gen);
        int end = node_dis(gen);
        if (start != end && existing_edges.find({min(start, end), max(start, end)}) == existing_edges.end()) {
            double weight = dis(gen);
            grafo.addEdge(start, end, weight);
            existing_edges.insert({min(start, end), max(start, end)});
        }
    }

    return grafo;
}