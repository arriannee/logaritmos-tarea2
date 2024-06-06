#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>

#include "graph.cpp"

// Nodo de la cola de Fibonacci
struct NodoFibonacci {
    int id;
    double key;
    NodoFibonacci* parent;
    NodoFibonacci* child;
    NodoFibonacci* left;
    NodoFibonacci* right;
    int degree;
    bool mark;

    NodoFibonacci(int id, double key)
        : id(id), key(key), parent(nullptr), child(nullptr),
          left(this), right(this), degree(0), mark(false) {}
};

// Cola de Fibonacci (H)
struct ColaFibonacci {
    NodoFibonacci* minNode;
    int nodeCount;

    ColaFibonacci() : minNode(nullptr), nodeCount(0) {}

    bool empty() const {
        return minNode == nullptr;
    }

    void insert(NodoFibonacci* node) {
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

    void cut(NodoFibonacci* x, NodoFibonacci* y) {
        // Remover x de la lista de hijos de y
        if (x->right == x) {
            y->child = nullptr;
        } else {
            x->left->right = x->right;
            x->right->left = x->left;
            if (y->child == x) {
                y->child = x->right;
            }
        }
        // Decrementando y.degree
        --y->degree;

        // Agregar x a la lista de raíces de H
        x->parent = nullptr;
        x->left = minNode;
        x->right = minNode->right;
        minNode->right->left = x;
        minNode->right = x;

        // Resetear el marcador de x
        x->mark = false;
    }

    void cascadingCut(NodoFibonacci* y) {
        NodoFibonacci* z = y->parent;

        if (z != nullptr) {
            if (!y->mark) {
                y->mark = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

    void decreaseKey(NodoFibonacci* x, int k) {
        if (k > x->key) {
            // error “new key is greater than current key”
            std::cout << "New key is greater than current key. \n" << std::endl;
        }
        x->key = k;

        NodoFibonacci* y = x->parent;

        if (y != nullptr && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->key < minNode->key) {
            minNode = x;
        }
    }

    NodoFibonacci* extractMin() {
        NodoFibonacci* z = minNode;
        if (z != nullptr) {
            if (z->child != nullptr) {
                NodoFibonacci* x = z->child;
                do {
                    NodoFibonacci* next = x->right;
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
        int maxDegree = static_cast<int>(std::log2(nodeCount)) + 1;
        std::vector<NodoFibonacci*> A(maxDegree, nullptr);

        std::vector<NodoFibonacci*> rootNodes;
        NodoFibonacci* x = minNode;
        if (x != nullptr) {
            do {
                rootNodes.push_back(x);
                x = x->right;
            } while (x != minNode);

            for (NodoFibonacci* w : rootNodes) {
                x = w;
                int d = x->degree;
                while (A[d] != nullptr) {
                    NodoFibonacci* y = A[d];
                    if (x->key > y->key) {
                        std::swap(x, y);
                    }
                    link(y, x);
                    A[d] = nullptr;
                    ++d;
                }
                A[d] = x;
            }

            minNode = nullptr;
            for (NodoFibonacci* y : A) {
                if (y != nullptr) {
                    if (minNode == nullptr) {
                        minNode = y;
                    } else {
                        y->left->right = y->right;
                        y->right->left = y->left;
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
    }

    void link(NodoFibonacci* y, NodoFibonacci* x) {
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