#include <iostream>
#include <vector>
#include "heapT.cpp"

int main() {
    // Nodos
    Node nodo0(0);
    Node nodo1(1);
    Node nodo2(2);
    Node nodo3(3);
    Node nodo4(4);
    Node nodo5(5);

    // Pares
    tuple<double, Node> par0(2012, nodo0);
    tuple<double, Node> par1(1012, nodo1);
    tuple<double, Node> par2(3032, nodo2);
    tuple<double, Node> par3(1231, nodo3);
    tuple<double, Node> par4(9382, nodo4);
    tuple<double, Node> par5(2331, nodo5);

    // Estructura Q
    Heap Q;

    // Insertar los elementos en la estructura Q
    Q.insertHeap(par0);
    Q.insertHeap(par1);
    Q.insertHeap(par2);
    Q.insertHeap(par3);
    Q.insertHeap(par4);
    Q.insertHeap(par5);

    cout << "Valores originales:" << endl;

    for(auto elemento : Q.pares){
        double precio = get<0>(elemento);
        cout << "elemento:" << precio << endl;
    }

    /*
    cout << "Se realiza extractMin" << endl;

    Q.extractMin();

    cout << "Valores después extractMin" << endl;

    for(auto elemento : Q.pares){
        double precio = get<0>(elemento);
        cout << "elemento:" << precio << endl;
    }
    */
    
    // Puntero a algún nodo
    tuple<double, Node>* ptr = &Q.pares[3];

    ptrdiff_t index = ptr - &Q.pares[0];

    cout << "El índice debería ser 3 y es: " << index << endl;

    cout << "Valores después de decreaseKey" << endl;

    Q.decreaseKey(index, 123);

    for(auto elemento : Q.pares){
        double precio = get<0>(elemento);
        cout << "elemento:" << precio << endl;
    }
    

    return 0;
}