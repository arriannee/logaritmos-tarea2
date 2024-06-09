#include <iostream>
#include <cassert>

#include "../src/graph.cpp" 

void testAddNode() {
    Graph graph;
    graph.addNode(1);
    assert(graph.nodes.size() == 1);
    assert(graph.nodes[1].id == 1);
    std::cout << "testAddNode OK! \n";
}

void testAddEdge() {
    Graph graph;
    graph.addNode(1);
    graph.addNode(2);
    graph.addEdge(1, 2, 5.0);
    assert(graph.nodes[1].matrix[2].size() == 1);
    assert(graph.nodes[1].matrix[2][0].weight == 5.0);
    std::cout << "testAddEdge OK! \n";
}

// Agregar las funciones de tests aquí:
int main() {
    testAddNode();
    testAddEdge();
    std::cout << "Tests OK! \n";
    return 0;
}
