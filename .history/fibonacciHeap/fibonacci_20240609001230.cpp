#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>

#include "../graph.cpp"

using namespace std;

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