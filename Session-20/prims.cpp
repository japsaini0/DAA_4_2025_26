#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

class Graph {
    int vertices;
    vector<vector<pair<int, int>>> adjacencyList;

public:
    Graph(int vertices) {
        this->vertices = vertices;
        adjacencyList.resize(vertices);
    }

    void addEdge(int nodeA, int nodeB, int weight) {
        adjacencyList[nodeA].push_back({nodeB, weight});
        adjacencyList[nodeB].push_back({nodeA, weight});
    }

    void primMST() {
        vector<int> minCost(vertices, INT_MAX);
        vector<int> parentNode(vertices, -1);
        vector<bool> visitedNodes(vertices, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        pq.push({0, 0});
        minCost[0] = 0;

        while (!pq.empty()) {
            int currentNode = pq.top().second;
            pq.pop();

            if (visitedNodes[currentNode]) {
                continue;
            }

            visitedNodes[currentNode] = true;

            for (auto it : adjacencyList[currentNode]) {
                int nextNode = it.first;
                int edgeWeight = it.second;
                if (!visitedNodes[nextNode] && edgeWeight < minCost[nextNode]) {
                    minCost[nextNode] = edgeWeight;
                    pq.push({minCost[nextNode], nextNode});
                    parentNode[nextNode] = currentNode;
                }
            }
        }

        cout << "Edge   Weight\n";
        for (int i = 1; i < vertices; ++i) {
            cout << parentNode[i] << " - " << i << "    " << minCost[i] << "\n";
        }
    }
};

int main() {
    int vertices = 9;
    Graph graph(vertices);

    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 7, 5);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 7, 6);
    graph.addEdge(2, 3, 12);
    graph.addEdge(2, 8, 9);
    graph.addEdge(2, 5, 7);
    graph.addEdge(3, 4, 2);
    graph.addEdge(3, 5, 8);
    graph.addEdge(4, 5, 15);
    graph.addEdge(5, 6, 11);
    graph.addEdge(6, 7, 4);
    graph.addEdge(6, 8, 13);
    graph.addEdge(7, 8, 20);

    graph.primMST();
    
    return 0;
}