#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <limits>

class FibNode {
public:
    int key;
    double value;
    FibNode* parent;
    FibNode* child;
    FibNode* left;
    FibNode* right;
    int degree;
    bool mark;

    FibNode(int key, double value) {
        this->key = key;
        this->value = value;
        parent = child = nullptr;
        left = right = this;
        degree = 0;
        mark = false;
    }
};

class FibHeap {
private:
    FibNode* minNode;
    int nodeCount;

    void link(FibNode* y, FibNode* x) {
        y->left->right = y->right;
        y->right->left = y->left;
        y->parent = x;

        if (x->child == nullptr) {
            x->child = y;
            y->right = y->left = y;
        } else {
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }

        x->degree++;
        y->mark = false;
    }

    void consolidate() {
        int D = static_cast<int>(std::log2(nodeCount)) + 1;
        std::vector<FibNode*> A(D, nullptr);

        std::vector<FibNode*> nodes;
        FibNode* x = minNode;
        if (x != nullptr) {
            do {
                nodes.push_back(x);
                x = x->right;
            } while (x != minNode);
        }

        for (FibNode* w : nodes) {
            x = w;
            int d = x->degree;
            while (A[d] != nullptr) {
                FibNode* y = A[d];
                if (x->value > y->value) {
                    std::swap(x, y);
                }
                link(y, x);
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
        }

        minNode = nullptr;
        for (FibNode* y : A) {
            if (y != nullptr) {
                if (minNode == nullptr) {
                    minNode = y;
                    minNode->left = minNode->right = minNode;
                } else {
                    y->left = minNode;
                    y->right = minNode->right;
                    minNode->right->left = y;
                    minNode->right = y;
                    if (y->value < minNode->value) {
                        minNode = y;
                    }
                }
            }
        }
    }

public:
    FibHeap() {
        minNode = nullptr;
        nodeCount = 0;
    }

    FibNode* insert(int key, double value) {
        FibNode* node = new FibNode(key, value);
        if (minNode == nullptr) {
            minNode = node;
        } else {
            node->left = minNode;
            node->right = minNode->right;
            minNode->right->left = node;
            minNode->right = node;
            if (node->value < minNode->value) {
                minNode = node;
            }
        }
        nodeCount++;
        return node;
    }

    FibNode* extractMin() {
        FibNode* z = minNode;
        if (z != nullptr) {
            if (z->child != nullptr) {
                FibNode* x = z->child;
                do {
                    FibNode* next = x->right;
                    x->left->right = x->right;
                    x->right->left = x->left;

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
            nodeCount--;
        }
        return z;
    }

    void decreaseKey(FibNode* x, double k) {
        if (k > x->value) {
            std::cerr << "New key is greater than current key" << std::endl;
            return;
        }
        x->value = k;
        FibNode* y = x->parent;
        if (y != nullptr && x->value < y->value) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->value < minNode->value) {
            minNode = x;
        }
    }

    void cut(FibNode* x, FibNode* y) {
        if (x->right == x) {
            y->child = nullptr;
        } else {
            x->left->right = x->right;
            x->right->left = x->left;
            if (y->child == x) {
                y->child = x->right;
            }
        }
        y->degree--;

        x->left = minNode;
        x->right = minNode->right;
        minNode->right->left = x;
        minNode->right = x;
        x->parent = nullptr;
        x->mark = false;
    }

    void cascadingCut(FibNode* y) {
        FibNode* z = y->parent;
        if (z != nullptr) {
            if (!y->mark) {
                y->mark = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

    bool isEmpty() {
        return minNode == nullptr;
    }
};



#include <iostream>
#include <vector>
#include <list>
#include <utility> // Para std::pair
#include <unordered_map>
#include <limits>
#include <queue>

// Definición de la clase Grafo
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

    // Algoritmo de Dijkstra utilizando una cola de Fibonacci
    void dijkstra(int src) {
        std::vector<double> dist(V, std::numeric_limits<double>::infinity());
        std::vector<int> prev(V, -1);
        FibHeap pq;
        std::unordered_map<int, FibNode*> node_map;

        dist[src] = 0;
        pq.insert(src, 0);
        node_map[src] = pq.minNode;

        for (int i = 0; i < V; ++i) {
            if (i != src) {
                pq.insert(i, std::numeric_limits<double>::infinity());
                node_map[i] = pq.minNode;
            }
        }

        while (!pq.isEmpty()) {
            FibNode* u = pq.extractMin();
            int u_key = u->key;
            double u_value = u->value;

            for (const auto& neighbor : adj[u_key]) {
                int v = neighbor.first;
                double weight = neighbor.second;

                if (dist[u_key] + weight < dist[v]) {
                    dist[v] = dist[u_key] + weight;
                    prev[v] = u_key;
                    pq.decreaseKey(node_map[v], dist[v]);
                }
            }
        }

        // Imprimir las distancias
        for (int i = 0; i < V; ++i) {
            std::cout << "Distancia del nodo " << src << " al nodo " << i << " es " << dist[i] << std::endl;
        }
    }
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

    // Ejecutar Dijkstra desde el nodo 0
    g.dijkstra(0);

    return 0;
}



