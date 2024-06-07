#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <fstream>

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

        x->parent = nullptr;
        x->left = minNode;
        x->right = minNode->right;
        minNode->right->left = x;
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
            std::cerr << "New key is greater than current key.\n";
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
        cout << "decreaseKey: Nodo " << x->id << " actualizado a " << k << endl;
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
        cout << "extractMin: Nodo " << z->id << " extraído" << endl;
        return z;
    }

    void consolidate() {
        int D = static_cast<int>(std::log2(nodeCount)) + 1;
        vector<FibonacciNode*> A(D, nullptr);
        vector<FibonacciNode*> rootNodes;
        FibonacciNode* x = minNode;

        if (x != nullptr) {
            do {
                rootNodes.push_back(x);
                x = x->right;
            } while (x != minNode);

            for (FibonacciNode* w : rootNodes) {
                x = w;
                int d = x->degree;
                while (A[d] != nullptr) {
                    FibonacciNode* y = A[d];
                    if (x->key > y->key) {
                        swap(x, y);
                    }
                    link(y, x);
                    A[d] = nullptr;
                    ++d;
                }
                A[d] = x;
            }

            minNode = nullptr;
            for (FibonacciNode* y : A) {
                if (y != nullptr) {
                    if (minNode == nullptr) {
                        minNode = y;
                        minNode->left = minNode;
                        minNode->right = minNode;
                    } else {
                        y->left = minNode;
                        y->right = minNode->right;
                        minNode->right->left = y;
                        minNode->right = y;
                        if (y->key < minNode->key) {
                            minNode = y;
                        }
                    }
                }
            }
        }
        cout << "consolidate: Consolidacion completada" << endl;
    }

    void link(FibonacciNode* y, FibonacciNode* x) {
        y->left->right = y->right;
        y->right->left = y->left;
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
            cout << "Error: extractMin devolvió nullptr. Terminando la ejecución." << endl;
            break;
        }

        int v = minNode->id;
        cout << "Procesando nodo " << v << " con distancia " << distancias[v] << endl;

        // Por cada vecino u del nodo v:
        for (const auto& u : graph.nodes[v].neighbors) {
            int uId = u.first;
            double aristaUV = u.second.weight;
            double alt = distancias[v] + aristaUV;
            cout << "Procesando vecino " << uId << " con arista peso " << aristaUV << " y distancia alternativa " << alt << endl;

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
                    cout << "Actualizando nodo " << uId << " con nueva distancia " << alt << endl;
                } else {
                    cout << "Error: Nodo " << uId << " no encontrado en el mapa de nodos" << endl;
                }
            } else {
                cout << "Nodo " << uId << " ya tiene una distancia menor o igual a " << alt << endl;
            }
        }

        // Liberar memoria del nodo procesado
        delete minNode;

        // Verificación adicional del estado del heap
        cout << "Tamaño del heap después de procesar nodo " << v << ": " << Q.nodeCount << endl;
        if (iteration++ > graph.nodes.size() * 2) {
            cout << "Error: Número de iteraciones excedido, posible ciclo o problema en el heap." << endl;
            break;
        }
    }

    // Liberar memoria de todos los nodos restantes en nodeMap
    for (auto& pair : nodeMap) {
        delete pair.second;
    }

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
void testDijkstraWithFibonacci() {
    cout << "PRUEBA 1: 2^10 nodos, 2^12 aristas \n";
    Graph grafo1 = construirGrafo(10, 12); // 10 nodos, 12 aristas
    auto resultado = dijkstraWithFibonacciHeap(grafo1, 0);

    // Imprimir resultados de Dijkstra
    imprimirResultadosDijkstraEnArchivo(resultado, "resultados_dijkstra_fibonacci.txt");
}

int main() {
    testDijkstraWithFibonacci(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}
