#include "graph.cpp"
#include <tuple>
#include <limits>
using namespace std;


struct Heap{
    vector<tuple<double, Node>> par;

    // Se tiene en la estructura de un heap, siendo i el índice de un nodo:
    // El padre de un nodo siempre está en (i-1)/2
    int parent(int i){
        return ((i-1)/2);
    }

    // El hijo izquierdo de un nodo siempre está en (2i)+1
    int left(int i){
        return ((2*i)+1);
    }

    // El hijo derecho de un nodo siempre está en (2i)+2
    int right(int i){
        return ((2*i)+2);
    }
    
    // A: array, i: índice
    void heapify(int i){
        int l = left(i);
        int r = right(i);
        int min = i;
        // Si el hijo izquierdo está dentro del heap y es menor que el actual
        if((l >= par.size()) && (get<0>(par[l]) < get<0>(par[min]))){
            // El hijo izquierdo es más pequeño
            min = l;
        } else {
            // Si no, el actual es más pequeño
            min = i;
        }
        // Si el hijo derecho está dentro del heap y es menor que el actual
        if ((r <= par.size()) && (get<0>(par[r]) < get<0>(par[min]))){
            min = r;
        }
        // Si el minimo es distinto que al inicio
        if (min != i){
            // Se debe intercambiar par[i] con el nuevo mínimo
            swap(par[i], par[min]);
            // Y llamamos a heapify de su hijo por si esto causó conflicto
            heapify(min);
        }      
    }

    tuple<double, Node> extractMin(){
        // El mínimo es el primer elemento en la cola de prioridad
        tuple<double, Node> min = par[0];
        // Intercambiamos el primer por el último elemento
        par[0] = par.back();
        // Eliminamos el elemento que queremos extraer
        par.pop_back();
        // Lo anterior puede haber roto la condición del Heap, así que se llama a Heapify sobre su primer elemento
        heapify(0);
        // Retornamos el elemento menor
        return min;
    }

    // Recibe el índice y el valor que se desea colocar en él
    void decreaseKey(int i, double k){
        // Se cambia el valor del índice i por k
        get<0>(par[i]) = k;
        // Lo anterior puede haber roto la condición del Heap, así que se revisará toda la rama hacia arriba
        // Mientras el elemento i tenga un padre y el valor del padre sea mayor que el de i
        while ((i > 1) && (get<0>(par[parent(i)]) > get<0>(par[i]))){
            // Se intercambia la posición del elemento i y su padre
            swap(par[i], par[parent(i)]);
            // Se revisa nuevamente hacia arriba
            i = parent(i);
        }
    }
};
