#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <chrono>

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
        int vecino = dis1(gen);
        double peso = distribucion(gen);
        grafo.addEdge(grafo.nodes[i].id, grafo.nodes[vecino].id, peso);
        grafo.addEdge(grafo.nodes[vecino].id, grafo.nodes[i].id, peso);  // Añadir la arista en la dirección opuesta
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
        double peso = distribucion(gen);
        grafo.addEdge(grafo.nodes[nodo1].id, grafo.nodes[nodo2].id, peso);
        grafo.addEdge(grafo.nodes[nodo2].id, grafo.nodes[nodo1].id, peso);  // Añadir la arista en la dirección opuesta
    }
    
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

// Nodo de la cola de Fibonacci
struct FibonacciNode {
    int id;
    double key;
    FibonacciNode* parent;
    FibonacciNode* child;
    FibonacciNode* left;
    FibonacciNode* right;
    int degree;
    bool mark;

    FibonacciNode(int id, double key)
        : id(id), key(key), parent(nullptr), child(nullptr),
          left(this), right(this), degree(0), mark(false) {}
};

// Cola de Fibonacci (H)
struct FibonacciHeap {
    FibonacciNode* minNode;
    int nodeCount;

    FibonacciHeap() : minNode(nullptr), nodeCount(0) {}

    bool empty() const {
        return minNode == nullptr;
    }

    void insert(FibonacciNode* node) {
        if (minNode == nullptr) {
            minNode = node;
        } else {
            node->left = minNode;
            node->right = minNode->right;
            minNode->right->left = node;
            minNode->right = node;
            if (node->key < minNode->key) {
                minNode = node;
            }
        }
        ++nodeCount;
    }

    void cut(FibonacciNode* x, FibonacciNode* y) {
        if (x == nullptr || y == nullptr) {
            return;
        }

        // Remove x from the child list of y
        if (x->right == x) {
            y->child = nullptr;
        } else {
            x->left->right = x->right;
            x->right->left = x->left;
            if (y->child == x) {
                y->child = x->right;
            }
        }
        --y->degree;

        // Add x to the root list of the heap
        x->parent = nullptr;
        x->left = minNode;
        if (minNode == nullptr) {
            return;
        }
        x->right = minNode->right;
        if (minNode->right != nullptr) {
            minNode->right->left = x;
        }
        minNode->right = x;
        x->mark = false;
    }

    void cascadingCut(FibonacciNode* y) {
        FibonacciNode* z = y->parent;
        if (z != nullptr) {
            if (!y->mark) {
                y->mark = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

    void decreaseKey(FibonacciNode* x, double k) {
        if (k > x->key) {
            return;
        }
        x->key = k;

        FibonacciNode* y = x->parent;
        if (y != nullptr && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->key < minNode->key) {
            minNode = x;
        }
    }

    FibonacciNode* extractMin() {
        FibonacciNode* z = minNode;
        if (z != nullptr) {
            if (z->child != nullptr) {
                FibonacciNode* x = z->child;
                do {
                    FibonacciNode* next = x->right;
                    x->left = minNode;
                    x->right = minNode->right;
                    minNode->right->left = x;
                    minNode->right = x;
                    x->parent = nullptr;
                    x = next;
                } while (x != z->child);
            }
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right) {
                minNode = nullptr;
            } else {
                minNode = z->right;
                consolidate();
            }
            --nodeCount;
        }
        return z;
    }

    void consolidate() {
        if (minNode == nullptr) return;

        int D = static_cast<int>(std::log2(nodeCount)) + 1;
        vector<FibonacciNode*> A(D + 1, nullptr);  // Ajuste: +1 para asegurar espacio suficiente

    // Guardar todos los nodos raíz en una lista temporal
    vector<FibonacciNode*> rootNodes;
    FibonacciNode* x = minNode;
    do {
        rootNodes.push_back(x);
        x = x->right;
    } while (x != minNode);

    // Eliminar todos los nodos raíz del heap y consolidar
    for (FibonacciNode* w : rootNodes) {
        x = w;
        int d = x->degree;
        while (A[d] != nullptr) {
            FibonacciNode* y = A[d];
            if (!x || !y) {
                cerr << "Error: Null pointer encountered in consolidate. x or y is null." << endl;
                if (x) cout << "x: " << x->id << ", key: " << x->key << endl;
                if (y) cout << "y: " << (y ? to_string(y->id) : "null") << ", key: " << (y ? to_string(y->key) : "null") << endl;
                break;
            }
            if (x->key > y->key) {
                swap(x, y);
            }
            link(y, x);
            A[d] = nullptr;
            ++d;
        }
        A[d] = x;
    }

    // Reconstruir el heap a partir de los nodos consolidados
    minNode = nullptr;
    for (int i = 0; i <= D; ++i) {
        if (A[i] != nullptr) {
            if (minNode == nullptr) {
                minNode = A[i];
                minNode->left = minNode;
                minNode->right = minNode;
            } else {
                A[i]->left = minNode;
                A[i]->right = minNode->right;
                minNode->right->left = A[i];
                minNode->right = A[i];
                if (A[i]->key < minNode->key) {
                    minNode = A[i];
                }
            }
        }
    }
}

void link(FibonacciNode* y, FibonacciNode* x) {
    // Verificación de punteros nulos
    if (!y || !x) {
        cerr << "Error: Null pointer encountered in link. x or y is null." << endl;
        if (y) cout << "y: " << y->id << ", key: " << y->key << endl;
        if (x) cout << "x: " << x->id << ", key: " << x->key << endl;
        return;
    }
    // Remover y de la lista de raíces
    y->left->right = y->right;
    y->right->left = y->left;

    // Hacer y hijo de x
    y->parent = x;
    if (x->child == nullptr) {
        x->child = y;
        y->right = y;
        y->left = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    ++x->degree;
    y->mark = false;
}

};

pair<vector<double>, vector<int>> dijkstraWithFibonacciHeap(Graph& graph, int raiz) {
    // 1. Definimos dos arreglos de tamaño |V|, distancias y previos.
    vector<double> distancias(graph.nodes.size(), numeric_limits<double>::infinity());
    vector<int> previos(graph.nodes.size(), -1);

    // 3. Definimos la distancia del nodo raíz como 0, su nodo previo como −1,
    distancias[raiz] = 0;
    previos[raiz] = -1;

    // Inicializamos la cola de Fibonacci y agregamos la raíz
    FibonacciHeap Q;
    unordered_map<int, FibonacciNode*> nodeMap; // Mapeo para acceder a los nodos en la cola de Fibonacci
    for (auto& node : graph.nodes) {
        int nodeId = node.first;
        double distance = (nodeId == raiz) ? 0 : numeric_limits<double>::infinity();
        nodeMap[nodeId] = new FibonacciNode(nodeId, distance);
        Q.insert(nodeMap[nodeId]);
    }

    int iteration = 0;
    // 6. Mientras Q no se encuentre vacío, repetimos:
    while (!Q.empty()) {
        // Obtenemos el nodo con la menor distancia en Q y lo eliminamos.
        FibonacciNode* minNode = Q.extractMin();
        if (!minNode) {
            break;
        }

        int v = minNode->id;

        // Por cada vecino u del nodo v:
        for (const auto& u : graph.nodes[v].neighbors) {
            int uId = u.first;
            double aristaUV = u.second.weight;
            double alt = distancias[v] + aristaUV;

            // Si la distancia guardada para u (distancias[u]) es mayor a la distancia guardada para v (distancias[v]) más el peso de la arista (u, v)
            if (alt < distancias[uId]) {
                // Actualizamos el valor de la distancia de u,
                distancias[uId] = alt;
                // Guardamos v como el nodo previo de u y
                previos[uId] = v;
                // Actualizamos la distancia del nodo u en Q utilizando decreaseKey
                auto it = graph.nodes.find(uId);
                if (it != graph.nodes.end()) {
                    Q.decreaseKey(nodeMap[uId], alt);
                }
            }
        }

        // Liberar memoria del nodo procesado
        delete minNode;

        // Verificación adicional del estado del heap
        if (iteration++ > graph.nodes.size() * 2) {
            break;
        }
    }
    
    /*
    // Liberar memoria de todos los nodos restantes en nodeMap
    for (auto& pair : nodeMap) {
        if (pair.second) {
            delete pair.second;
            pair.second = nullptr;
        }
    }
    */

    // 7. Retornamos el arreglo de previos y distancias.
    return {distancias, previos};
}


// Función para imprimir resultados de Dijkstra en un archivo de texto
void imprimirResultadosDijkstraEnArchivo(const pair<vector<double>, vector<int>>& resultado, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        // Imprimir distancias
        outFile << "Distancias:\n";
        for (size_t i = 0; i < resultado.first.size(); ++i) {
            outFile << "Nodo " << i << ": " << resultado.first[i] << "\n";
        }

        // Imprimir previos
        outFile << "Previos:\n";
        for (size_t i = 0; i < resultado.second.size(); ++i) {
            outFile << "Nodo " << i << ": " << resultado.second[i] << "\n";
        }
        outFile.close();
    } else {
        cerr << "No se pudo abrir el archivo " << filename << " para escritura.\n";
    }
}

// Función para probar Dijkstra con cola de Fibonacci en un grafo grande
void testDijkstraWithFibonacci(int i, int j) {
    const int numExperiments = 50;
    ofstream timingFile("tiempos_dijkstra_fibonacci.txt", ios::app); // Abre en modo append para no sobrescribir

    if (timingFile.is_open()) {
        for (int experiment = 0; experiment < numExperiments; ++experiment) {
            cout << "Ejecutando experimento " << experiment + 1 << " para v = 2^" << i << " y e = 2^" << j << "\n";
            Graph grafo = construirGrafo(i, j); // Asumiendo que esta función ya está definida
            auto start = chrono::high_resolution_clock::now();
            auto resultado = dijkstraWithFibonacciHeap(grafo, 0);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;

            timingFile << "Experimento " << experiment++ << " para v = 2^" << i << " y e = 2^" << j << ": " << elapsed.count() << " segundos\n";

            // Imprimir resultados de Dijkstra en archivos separados
            string filename = "resultados_dijkstra_fibonacci_" + to_string(i) + "_" + to_string(j) + "_" + to_string(experiment++) + ".txt";
            imprimirResultadosDijkstraEnArchivo(resultado, filename);
        }
        timingFile.close();
    } else {
        cerr << "No se pudo abrir el archivo tiempos_dijkstra_fibonacci.txt para escritura.\n";
    }
}

int main() {
    vector<pair<int, int>> configuraciones = {{10, 16}, {10, 17}, {10, 18}, {12, 16}, {12, 17}, {12, 18}, {12,19}, {12, 20}, {12, 21}, {12, 22}, {14, 16}, {14, 17}, {14, 18}, {14,19}, {14, 20}, {14, 21}, {14, 22}};
    
    int numPrueba = 1;

    for (const auto& config : configuraciones) {
        int i = config.first;
        int j = config.second;
        cout << "Prueba " << numPrueba++ << ": v = 2^" << i << " y e = 2^" << j << "\n";
        testDijkstraWithFibonacci(i, j);
    }

    cout << "FIN\n";
    return 0;
}