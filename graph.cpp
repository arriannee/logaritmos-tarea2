#include <iostream>
#include <random>
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

Graph construirGrafo(int numeroi, int numeroj){
    // Para generar pesos aleatorios y uniformes dentro del rango (0..1]
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> distribucion(0.0, 1.0);

    Graph grafo;

    // Añadimos 2^i nodos al grafo
    for(int i=0; i<numeroi; i++){
        grafo.addNode(i);
    }

    // Para cada nodo i, se lo conecta con un nodo aleatorio elegido en [1..i −1]
    for(int i=1; i<numeroi; i++){
        std::uniform_int_distribution<> dis1(0, i-1);
        grafo.addEdge(grafo.nodes[i].id, grafo.nodes[dis1(gen)].id, distribucion(gen));
    }
    // Saliendo de este for se garantiza la conectividad

    // Añadir las 2^j − (v −1) aristas restantes
    for(int j=0; j<(pow(2,numeroj)-(numeroi-1)); j++){
        std::uniform_int_distribution<> dis2(0, numeroi);
        int nodo1 = grafo.nodes[dis2(gen)].id;
        int nodo2 = grafo.nodes[dis2(gen)].id;
        // Para que no sean el mismo nodo(?
        while(nodo1 == nodo2){
            nodo2 = grafo.nodes[dis2(gen)].id;
        }
        grafo.addEdge(grafo.nodes[nodo1].id, grafo.nodes[nodo2].id, distribucion(gen));
    }
    return grafo;
}