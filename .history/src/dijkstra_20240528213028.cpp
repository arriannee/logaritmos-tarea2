#include <vector>
#include <map>
#include <iostream>
#include <limits>
#include <cmath>

#include "fibonacci.cpp"

using namespace std;


// Implementación de Dijkstra utilizando la cola de Fibonacci
vector<double> dijkstra(Graph& graph, int startNode) {
    unordered_map<int, FibonacciNode*> nodes;
    FibonacciHeap heap;
    vector<double> dist(graph.nodes.size(), numeric_limits<double>::infinity());
    dist[startNode] = 0;

    for (const auto& pair : graph.nodes) {
        int nodeId = pair.first;
        double distance = (nodeId == startNode) ? 0 : numeric_limits<double>::infinity();
        FibonacciNode* node = new FibonacciNode(nodeId, distance);
        nodes[nodeId] = node;
        heap.insert(node);
    }

    while (!heap.empty()) {
        FibonacciNode* minNode = heap.extractMin();
        int u = minNode->id;
        for (const Edge& edge : graph.getNeighbors(u)) {
            int v = edge.end;
            double weight = edge.weight;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                heap.decreaseKey(nodes[v], dist[v]);
            }
        }
    }

    for (auto& pair : nodes) {
        delete pair.second;
    }

    return dist;
}

int main() {
    Graph graph;
    graph.addNode(0);
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);

    graph.addEdge(0, 1, 1.0);
    graph.addEdge(0, 2, 2.0);
    graph.addEdge(1, 2, 1.0);
    graph.addEdge(1, 3, 3.0);
    graph.addEdge(2, 3, 1.0);
    graph.addEdge(3, 4, 2.0);

    vector<double> distances = dijkstra(graph, 0);
    for (int i = 0; i < distances.size(); ++i) {
        cout << "Distance from node 0 to node " << i << " is " << distances[i] << endl;
    }

    return 0;
}