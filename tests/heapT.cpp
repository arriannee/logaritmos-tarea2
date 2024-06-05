#include "graphT.cpp"
#include <tuple>
#include <limits>


struct Heap{
    // Par (distancia, nodo)
    vector<tuple<double, Node>> pares;

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
    
    // El siguiente es un heapify que revisa hacia abajo
    // i: índice del nodo donde se hará heapify
    void heapify1(int i){
        int l = left(i);
        int r = right(i);
        int min = i;
        
        // Si el hijo izquierdo está dentro del heap y es menor que el actual
        if((l < pares.size()) && (get<0>(pares[l]) < get<0>(pares[min]))){
            // El hijo izquierdo es más pequeño
            min = l;
        } else {
            // Si no, el actual es más pequeño
            min = i;
        }       
        // Si el hijo derecho está dentro del heap y es menor que el actual
        if ((r < pares.size()) && (get<0>(pares[r]) < get<0>(pares[min]))){
            min = r;
        }     
        // Si el minimo es distinto que al inicio
        if (min != i){
            // Se debe intercambiar par[i] con el nuevo mínimo
            swap(pares[i], pares[min]);
            // Y llamamos a heapify de su hijo por si esto causó conflicto
            heapify1(min); 
        }    
    }

    // El siguiente es un heapify que revisa hacia arriba
    // i: índice del nodo donde se hará heapify
    void heapify2(int i){
        int p = parent(i);
        // Si el padre está dentro del vector
        if(p >= 0) {
            // Si el valor actual es menor que el del padre
            if(get<0>(pares[i]) < get<0>(pares[p])){
                // Se debe intercambiar el actual por el padre
                swap(pares[i], pares[p]);
                // Llamamos a heapify de su padre por si esto causó conflicto
                heapify2(p);
            }
        }
    }

    void insertHeap(tuple<double, Node> nuevoPar){
        // Se inserta el nuevo par al final
        pares.push_back(nuevoPar);
        // Como lo anterior puede romper la estructura, se hace heapify
        heapify2(get<1>(nuevoPar).id);
    }

    tuple<double, Node> extractMin(){
        // El mínimo es el primer elemento en la cola de prioridad
        tuple<double, Node> min = pares[0];
        // Intercambiamos el primer por el último elemento
        pares[0] = pares.back();
        // Eliminamos el elemento que queremos extraer
        pares.pop_back();
        // Lo anterior puede haber roto la condición del Heap, así que se llama a Heapify sobre su primer elemento
        heapify1(0);
        // Retornamos el elemento menor
        return min;
    }

    // Recibe el índice y el valor que se desea colocar en él
    void decreaseKey(int i, double k){
        // Se cambia el valor del índice i por k
        get<0>(pares[i]) = k;
        // Lo anterior puede haber roto la condición del Heap, así que se revisará toda la rama hacia arriba
        // Mientras el elemento i tenga un padre y el valor del padre sea mayor que el de i
        while ((i > 1) && (get<0>(pares[parent(i)]) > get<0>(pares[i]))){
            // Se intercambia la posición del elemento i y su padre
            swap(pares[i], pares[parent(i)]);
            // Se revisa nuevamente hacia arriba
            i = parent(i);
        }
    }
};