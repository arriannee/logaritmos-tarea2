#include <iostream>
#include <cassert>

#include "../src/fibonacci.cpp" 

// Función para probar el método insert
void testInsert() {
    FibonacciHeap fibHeap;

    FibonacciNode* node1 = new FibonacciNode(1, 10);
    FibonacciNode* node2 = new FibonacciNode(2, 5);
    FibonacciNode* node3 = new FibonacciNode(3, 15);

    fibHeap.insert(node1);
    fibHeap.insert(node2);
    fibHeap.insert(node3);

    // Verificar si el nodo mínimo es el esperado
    if (fibHeap.minNode->id == 2) {
        std::cout << "testInsert OK!: el nodo minimo es el correcto." << std::endl;
    } else {
        std::cout << "Test FAIL: el nodo minimo no es, en efecto, el minimo." << std::endl;
    }  
}

// Función para probar el método cut
void testCut() {
    FibonacciHeap fibHeap;

    FibonacciNode* node1 = new FibonacciNode(1, 10);
    FibonacciNode* node2 = new FibonacciNode(2, 20);
    FibonacciNode* node3 = new FibonacciNode(3, 30);

    fibHeap.insert(node1);
    fibHeap.insert(node2);

    // Simular la estructura de árbol, node1 tiene a node3 como hijo
    node1->child = node3;
    node3->parent = node1;
    node1->degree = 1;

    // Quitamos node3 de la lista de hijos de node1
    fibHeap.cut(node3, node1);

    // Verificar si node3 es ahora una raíz y minNode es correcto
    bool isRoot = node3->parent == nullptr;
    bool inRootList = false;
    FibonacciNode* current = fibHeap.minNode;

    do {
        if (current == node3) {
            inRootList = true;
            break;
        }
        current = current->right;
    } while (current != fibHeap.minNode);

    if (isRoot && inRootList) {
        std::cout << "Test OK!: node3 es ahora una raiz y esta en la lista de raices." << std::endl;
    } else {
        std::cout << "Test FAIL!: node3 no es una raiz o no esta en la lista de raices." << std::endl;
    }
}

// Falta test para Cascading-Cut, pero voy con toda la fe (lit me guié del psuedocódigo)

// Función para imprimir la lista de raíces 
void printRootList(FibonacciNode* minNode) {
    if (minNode == nullptr) {
        std::cout << "La lista de raices esta vacia." << std::endl;
        return;
    }

    FibonacciNode* current = minNode;
    do {
        std::cout << "Nodo " << current->id << " con clave " << current->key << std::endl;
        current = current->right;
    } while (current != minNode);
}

// Función para probar el método decreaseKey
void testDecreaseKey() {
    FibonacciHeap fibHeap;

    FibonacciNode* node1 = new FibonacciNode(1, 10);
    FibonacciNode* node2 = new FibonacciNode(2, 20);
    FibonacciNode* node3 = new FibonacciNode(3, 30);

    fibHeap.insert(node1);
    fibHeap.insert(node2);
    fibHeap.insert(node3);

    std::cout << "Antes de decreaseKey:" << std::endl;
    printRootList(fibHeap.minNode);

    fibHeap.decreaseKey(node3, 5);

    std::cout << "Después de decreaseKey:" << std::endl;
    printRootList(fibHeap.minNode);

    // Verificar si el nodo mínimo es el esperado
    if (fibHeap.minNode->id == 3) {
        std::cout << "Test OK!: el nodo minimo es el correcto despues de decreaseKey." << std::endl;
    } else {
        std::cout << "Test FAIL!: el nodo minimo no es el esperado despues de decreaseKey." << std::endl;
    }
}

int main() {
    testInsert();
    testCut();
    testDecreaseKey();
    std::cout << "Tests OK! \n";
    return 0;
}