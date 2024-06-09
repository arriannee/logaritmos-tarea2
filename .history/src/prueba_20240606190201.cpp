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

    Edge(int start, int end, double weight) : start(start), end(end), weight(weight) {}
};

// Definición de un grafo
struct Graph {
    map<int, vector<Edge>> adjList; // Lista de adyacencia para representar el grafo
    map<int, struct FibonacciNode*> nodePointers; // Mapa para almacenar punteros a nodos de la cola de Fibonacci

    // Agregar un nodo al grafo
    void addNode(int id) {
        adjList[id] = vector<Edge>();
    }

    // Agregar una arista al grafo
    void addEdge(int start, int end, double weight) {
        adjList[start].emplace_back(start, end, weight);
        adjList[end].emplace_back(end, start, weight); // Añadir la arista en la dirección opuesta
    }
};

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
    vector<double> distancias(graph.adjList.size(), numeric_limits<double>::infinity());
    vector<int> previos(graph.adjList.size(), -1);

    distancias[raiz] = 0;
    previos[raiz] = -1;

    FibonacciHeap Q;
    for (const auto& node : graph.adjList) {
        int nodeId = node.first;
        double distance = (nodeId == raiz) ? 0 : numeric_limits<double>::infinity();
        FibonacciNode* fibNode = new FibonacciNode(nodeId, distance);
        graph.nodePointers[nodeId] = fibNode;
        Q.insert(fibNode);
    }

    int iteration = 0;
    while (!Q.empty()) {
        FibonacciNode* minNode = Q.extractMin();
        if (!minNode) {
            cout << "Error: extractMin devolvió nullptr. Terminando la ejecución." << endl;
            break;
        }

        int v = minNode->id;
        cout << "Procesando nodo " << v << " con distancia " << distancias[v] << endl;

        for (const auto& u : graph.adjList[v]) {
            int uId = u.end;
            double aristaUV = u.weight;
            double alt = distancias[v] + aristaUV;
            cout << "Procesando vecino " << uId << " con arista peso " << aristaUV << " y distancia alternativa " << alt << endl;

            if (alt < distancias[uId]) {
                distancias[uId] = alt;
                previos[uId] = v;
                Q.decreaseKey(graph.nodePointers[uId], alt);
                cout << "Actualizando nodo " << uId << " con nueva distancia " << alt << endl;
            } else {
                cout << "Nodo " << uId << " ya tiene una distancia menor o igual a " << alt << endl;
            }
        }

        delete minNode;
        cout << "Tamaño del heap después de procesar nodo " << v << ": " << Q.nodeCount << endl;
        if (iteration++ > graph.adjList.size() * 2) {
            cout << "Error: Número de iteraciones excedido, posible ciclo o problema en el heap." << endl;
            break;
        }
    }

    for (auto& pair : graph.nodePointers) {
        delete pair.second;
    }

    return {distancias, previos};
}

void imprimirResultadosDijkstraEnArchivo(const pair<vector<double>, vector<int>>& resultado, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "Distancias:\n";
        for (size_t i = 0; i < resultado.first.size(); ++i) {
            outFile << "Nodo " << i << ": " << resultado.first[i] << "\n";
        }

        outFile << "Previos:\n";
        for (size_t i = 0; i < resultado.second.size(); ++i) {
            outFile << "Nodo " << i << ": " << resultado.second[i] << "\n";
        }
        outFile.close();
    } else {
        cerr << "No se pudo abrir el archivo " << filename << " para escritura.\n";
    }
}

void testDijkstraWithFibonacci() {
    cout << "PRUEBA 1: 2^10 nodos, 2^12 aristas \n";
    Graph grafo1 = construirGrafo(10, 12); // 10 nodos, 12 aristas
    auto resultado = dijkstraWithFibonacciHeap(grafo1, 0);
    imprimirResultadosDijkstraEnArchivo(resultado, "resultados_dijkstra_fibonacci.txt");
}

int main() {
    testDijkstraWithFibonacci(); // Llamar a la función de prueba
    cout << "FIN";
    return 0;
}