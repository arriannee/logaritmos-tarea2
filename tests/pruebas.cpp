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

    for(auto elemento : Q.pares){
        double precio = get<0>(elemento);
        cout << "elemento:" << precio << endl;
    }

    /*
    std::vector<int> vec = {10, 20, 30, 40, 50};
    
    // Obtener un puntero a uno de los elementos del vector
    int* ptr = &vec[2];  // ptr apunta al elemento 30

    // Mostrar el valor original
    std::cout << "Valor original en vec[2]: " << vec[2] << std::endl;

    // Editar el valor al que apunta ptr
    *ptr = 123123;

    // Mostrar el vector modificado
    for(int i=0; i<vec.size(); i++){
        std::cout << "Valor " << i << " del vector es " << vec[i] << std::endl;
    }
    */

    return 0;
}