#include <iostream>
#include <vector>
#include <list>
#include <utility> // Para std::pair

class Graph {
private:
    int V; // Número de vértices
    std::vector<std::list<std::pair<int, double>>> adj; // Lista de adyacencia

public:
    // Constructor
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Función para añadir una arista al grafo
    void addEdge(int u, int v, double weight) {
        adj[u].emplace_back(v, weight);
        adj[v].emplace_back(u, weight); // Debido a que es un grafo no dirigido
    }

    // Función para imprimir el grafo (para pruebas)
    void printGraph() const {
        for (int i = 0; i < V; ++i) {
            std::cout << "Nodo " << i << ": ";
            for (const auto& neighbor : adj[i]) {
                std::cout << " -> (" << neighbor.first << ", " << neighbor.second << ")";
            }
            std::cout << std::endl;
        }
    }

    // Getters
    int getV() const { return V; }
    const std::list<std::pair<int, double>>& getAdj(int u) const { return adj[u]; }
};

int main() {
    int V = 5; // Ejemplo con 5 vértices
    Graph g(V);

    // Añadir algunas aristas
    g.addEdge(0, 1, 0.5);
    g.addEdge(0, 4, 1.2);
    g.addEdge(1, 2, 0.8);
    g.addEdge(1, 3, 0.7);
    g.addEdge(1, 4, 0.9);
    g.addEdge(2, 3, 1.1);
    g.addEdge(3, 4, 0.3);

    // Imprimir el grafo
    g.printGraph();

    return 0;
}
