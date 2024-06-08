#include <iostream>
#include <random>
#include <vector>
#include <map>
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
    int Qpos;                           // Posición del par en que está el nodo en la estructura Q

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
        nodes[start].neighbors[end] = Edge(start, end, weight);
    }
};

Graph construirGrafo(int numeroi, int numeroj){
    // Para generar pesos aleatorios y uniformes dentro del rango (0..1]
    mt19937 gen(random_device{}());
    uniform_real_distribution<double> distribucion(0.0, 1.0);

    Graph grafo;

    // Añadimos 2^numeroi nodos al grafo
    int numNodos = pow(2, numeroi);
    for(int i = 0; i < numNodos; i++){
        grafo.addNode(i);
    }

    // Para cada nodo i, se lo conecta con un nodo aleatorio elegido en [0..i-1]
    for(int i = 1; i < numNodos; i++){
        uniform_int_distribution<> dis1(0, i-1);
        int vecino = dis1(gen);
        double peso = distribucion(gen);
        grafo.addEdge(i, vecino, peso);
        grafo.addEdge(vecino, i, peso);  // Añadir la arista en la dirección opuesta
    }
    // Saliendo de este for se garantiza la conectividad

    // Añadir las 2^numeroj aristas adicionales
    int totalAristas = pow(2, numeroj);
    for(int j = 0; j < totalAristas; j++){
        uniform_int_distribution<> dis2(0, numNodos - 1);
        int nodo1 = dis2(gen);
        int nodo2 = dis2(gen);
        // Para que no sean el mismo nodo
        while(nodo1 == nodo2){
            nodo2 = dis2(gen);
        }
        double peso = distribucion(gen);
        grafo.addEdge(nodo1, nodo2, peso);
        grafo.addEdge(nodo2, nodo1, peso);  // Añadir la arista en la dirección opuesta
    }

    std::cout << "Grafo construido" << std::endl;
    
    return grafo;
}

// Función para imprimir un grafo
void imprimirGrafo(const Graph& grafo) {
    for (const auto& node : grafo.nodes) {
        cout << "Nodo " << node.first << ":\n";
        for (const auto& neighbor : node.second.neighbors) {
            cout << "  Vecino " << neighbor.first << " con peso " << neighbor.second.weight << "\n";
        }
    }
}