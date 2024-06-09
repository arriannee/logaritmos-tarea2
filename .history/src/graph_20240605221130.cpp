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
        grafo.addEdge(grafo.nodes[i].id, grafo.nodes[dis1(gen)].id, distribucion(gen));
    }
    // Saliendo de este for se garantiza la conectividad

    // Añadir las 2^numeroj aristas adicionales
    int totalAristas = pow(2, numeroj);
    for(int j = 0; j < totalAristas; j++){
        uniform_int_distribution<> dis2(0, numNodos - 1);
        int nodo1 = grafo.nodes[dis2(gen)].id;
        int nodo2 = grafo.nodes[dis2(gen)].id;
        // Para que no sean el mismo nodo
        while(nodo1 == nodo2){
            nodo2 = grafo.nodes[dis2(gen)].id;
        }
        grafo.addEdge(grafo.nodes[nodo1].id, grafo.nodes[nodo2].id, distribucion(gen));
    }
    
    std::cout << "Grafo construido" << std::endl;
    
    return grafo;
}