#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <iostream>

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

Graph construirGrafo(int i, int j) {
    int numNodos = 1 << i; // 2^i nodos
    int numAristas = 1 << j; // 2^j aristas

    Graph grafo;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.1, 1.0);

    // Agregar nodos al grafo
    for (int n = 0; n < numNodos; ++n) {
        grafo.nodes[n] = Node{n, {}};
    }

    // Crear un árbol cobertor para asegurar que el grafo sea conexo
    for (int n = 1; n < numNodos; ++n) {
        int randomNode = rand() % n;
        double weight = dis(gen);
        grafo.nodes[n].neighbors.push_back({randomNode, weight});
        grafo.nodes[randomNode].neighbors.push_back({n, weight});
    }

    // Agregar las aristas adicionales
    unordered_set<pair<int, int>, pair_hash> existingEdges;
    for (int n = 0; n < numNodos; ++n) {
        for (const auto& neighbor : grafo.nodes[n].neighbors) {
            existingEdges.insert({min(n, neighbor.end), max(n, neighbor.end)});
        }
    }

    while (existingEdges.size() < numAristas) {
        int u = rand() % numNodos;
        int v = rand() % numNodos;
        if (u != v) {
            pair<int, int> edge = {min(u, v), max(u, v)};
            if (existingEdges.find(edge) == existingEdges.end()) {
                double weight = dis(gen);
                grafo.nodes[u].neighbors.push_back({v, weight});
                grafo.nodes[v].neighbors.push_back({u, weight});
                existingEdges.insert(edge);
            }
        }
    }

    return grafo;
}

