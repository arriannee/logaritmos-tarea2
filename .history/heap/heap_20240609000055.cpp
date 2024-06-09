#include "graph.cpp"
#include <tuple>
#include <limits>


// Definición de la estructura Heap
struct Heap{
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
    void heapify1(Graph& grafo, int i){
        int l = left(i);
        int r = right(i);
        int min = i;
        // Si el hijo izquierdo está dentro del heap y es menor que el actual
        if((l < pares.size()) && (get<0>(pares[l]) < get<0>(pares[min]))){
            // El hijo izquierdo es más pequeño
            min = l;
        }
        // Si el hijo derecho está dentro del heap y es menor que el actual
        if ((r < pares.size()) && (get<0>(pares[r]) < get<0>(pares[min]))){
            min = r;
        }
        // Si el minimo es distinto que al inicio
        if (min != i){    
            // Se debe intercambiar par[i] con el nuevo mínimo
            swap(pares[i], pares[min]);      
            // Actualizamos las Qpos de los nodos
            int idi = get<1>(pares[i]).id;
            int idmin = get<1>(pares[min]).id; 
            grafo.nodes[idi].Qpos = i;
            grafo.nodes[idmin].Qpos = min;
            // Y llamamos a heapify de su hijo por si esto causó conflicto
            heapify1(grafo, min);
        }      
    }

    // El siguiente es un heapify que revisa hacia arriba
    // i: índice del nodo donde se hará heapify
    void heapify2(Graph& grafo, int i){
        int p = parent(i);
        // Si el padre está dentro del vector
        if(p >= 0) {
            // Si el valor actual es menor que el del padre
            if(get<0>(pares[i]) < get<0>(pares[p])){  
                // Se debe intercambiar el actual por el padre
                swap(pares[i], pares[p]);
                // Actualizamos las Qpos de los nodos
                int idi = get<1>(pares[i]).id;
                int idp = get<1>(pares[p]).id;
                grafo.nodes[idi].Qpos = i;
                grafo.nodes[idp].Qpos = p;
                // Llamamos a heapify de su padre por si esto causó conflicto
                heapify2(grafo, p);
            }
        }
    }

    void insertHeap(Graph& grafo, tuple<double, Node> nuevoPar){
        // Se inserta el nuevo par al final
        pares.push_back(nuevoPar);
        int lastPair = pares.size() - 1;
        // La posición del nuevo par en Q será el final del vector de pares
        grafo.nodes[get<1>(nuevoPar).id].Qpos = lastPair;
        // Como lo anterior puede romper la estructura, se hace heapify
        heapify2(grafo, lastPair);
    }

    tuple<double, Node> extractMin(Graph& grafo){
        // El mínimo es el primer elemento en la cola de prioridad
        tuple<double, Node> min = pares[0];
        // Se elimina la posición del elemento que se va a extraer
        int idMin = get<1>(min).id;
        grafo.nodes[idMin].Qpos = -1;
        // Intercambiamos el primer por el último elemento
        swap(pares[0], pares[pares.size()-1]);
        // Eliminamos el elemento que queremos extraer
        pares.pop_back();    
        if (!pares.empty()) {
            // Actualizamos la posición del que ahora quedó primero
            int idNew = get<1>(pares[0]).id;
            grafo.nodes[idNew].Qpos = 0;
            // Lo anterior puede haber roto la condición del Heap, así que se llama a Heapify sobre su primer elemento
            heapify1(grafo, 0);
        }
        // Retornamos el elemento menor
        return min;
    }

    // Recibe el índice y el valor que se desea colocar en él
    void decreaseKey(Graph& grafo, int i, double k){
        // Se cambia el valor del índice i por k
        pares[i] = make_tuple(k, get<1>(pares[i])); 
        // Lo anterior puede haber roto la condición del Heap, así que se revisará toda la rama hacia arriba    
        // Mientras el elemento i tenga un padre y el valor del padre sea mayor que el de i
        while ((i >= 1) && (get<0>(pares[parent(i)]) > get<0>(pares[i]))){
            int parentIndex = parent(i);
            // Se intercambia la posición del elemento i y su padre
            swap(pares[i], pares[parentIndex]);    
            // Se actualizan las posiciones de los pares intercambiados  
            int iId = get<1>(pares[i]).id;
            int idparentIndex = get<1>(pares[parentIndex]).id;
            grafo.nodes[iId].Qpos = i;
            grafo.nodes[idparentIndex].Qpos = parentIndex;
            // Se revisa nuevamente hacia arriba
            i = parentIndex;
        }
    }
};
