#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>
#include <memory>
#include <fstream>

struct FibonacciNode {
    int id;
    double key;
    FibonacciNode* parent;
    std::unique_ptr<FibonacciNode> child;
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

    void insert(std::unique_ptr<FibonacciNode>& node) {
        if (minNode == nullptr) {
            minNode = node.release();
        } else {
            node->left = minNode;
            node->right = minNode->right;
            minNode->right->left = node.get();
            minNode->right = node.get();
            if (node->key < minNode->key) {
                minNode = node.get();
            }
        }
        ++nodeCount;
    }

    std::unique_ptr<FibonacciNode> extractMin() {
        std::unique_ptr<FibonacciNode> z(minNode);
        if (z) {
            if (z->child) {
                FibonacciNode* x = z->child.release();
                do {
                    FibonacciNode* next = x->right;
                    x->left = minNode;
                    x->right = minNode->right;
                    minNode->right->left = x;
                    minNode->right = x;
                    x->parent = nullptr;
                    x = next;
                } while (x != z->child.get());
            }
            z->left->right = z->right;
            z->right->left = z->left;
            if (z.get() == z->right) {
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
        int D = static_cast<int>(std::log2(nodeCount)) + 1;
        std::vector<FibonacciNode*> A(D, nullptr);
        std::vector<FibonacciNode*> rootNodes;
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
                        std::swap(x, y);
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
    }

    void link(FibonacciNode* y, FibonacciNode* x) {
        y->left->right = y->right;
        y->right->left = y->left;
        y->parent = x;
        if (!x->child) {
            x->child.reset(y);
            y->right = y;
            y->left = y;
        } else {
            y->left = x->child.get();
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }
        ++x->degree;
        y->mark = false;
    }

    void cut(FibonacciNode* x, FibonacciNode* y) {
        if (x->right == x) {
            y->child.reset();
        } else {
            x->left->right = x->right;
            x->right->left = x->left;
            if (y->child.get() == x) {
                y->child.reset(x->right);
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
    }
};
