# Tarea 2: Dijkstra y Análisis Amortizado
Este proyecto implementa dos versiones del algoritmo de Dijkstra para encontrar caminos más cortos en un grafo no dirigido con pesos positivos en sus aristas. Las dos implementaciones utilizan diferentes estructuras de datos para la cola de prioridad: un Heap y una Cola de Fibonacci. El objetivo principal es analizar empíricamente la diferencia en las complejidades globales entre estos dos algoritmos.

# Estructura del Repositorio
```
tarea2-dijkstra/
│
├── fib_csv/                    # Archivos CSV para los tiempos de Fibonacci
│   ├── fibtocsv.py
│   ├── tiempos_dijkstra_fibonacci.csv
│   ├── tiempos_dijkstra_fibonacci_2e10.csv
│   ├── tiempos_fibonacci_2e12.csv
│   ├── tiempos_fibonacci_2e14.csv
│
├── fibonacciHeap/              # Código Dijkstra con Cola de Fibonacci
│   ├── output/
│   ├── dijkstraFibonacci.exe
│   ├── tiempos_dijkstra_fibonacci.txt
│   ├── dijkstraFibonacci.cpp
│   ├── fibonacciHeaps.pdf
│   └── fibonacci.cpp
│
├── heap/                       # Código Dijkstra con Heap
│   ├── output/
│   ├── dijkstraHeap.exe
│   ├── tiempos_dijkstra_heap.txt
│   ├── dijkstraHeap.cpp
│   └── heap.cpp
│
├── heap_csv/                   # Archivos CSV para los tiempos de Heap
│   ├── heaptocsv.py
│   ├── tiempos_dijkstra_heap.txt
│   ├── tiempos_heap_2e10.csv
│   ├── tiempos_heap_2e12.csv
│   └── tiempos_heap_2e14.csv
│
├── PlotsTarea2.ipynb           # Jupyter Notebook para los gráficos de la tarea
├── README.md                   # Instrucciones del proyecto
├── Tarea_2.pdf                 # Enunciado
└── graph.cpp                   # Implementación del grafo

```

# Requisitos
Para compilar y ejecutar este proyecto, se necesita un compilador de C++ que soporte C++11 o superior. No se requieren bibliotecas externas adicionales.

# Compilación y Ejecución
Se recomienda utilizar un entorno de desarrollo integrado (IDE) como Visual Studio Code, CLion, o cualquier otro IDE que soporte C++.

Una vez abierto el proyecto en el IDE, navegue la estructura deseada y ejecute el archivo ```dijkstraFibonacci.cpp``` para ejecutar Dijsktra con Colas de Fibonacci y ```dijkstraHeap.cpp``` para Heaps, los resultados serán guardados en ```tiempos_dijkstra_fibonacci.txt``` y ```tiempos_dijkstra_heap.txt``` respectivamente.
