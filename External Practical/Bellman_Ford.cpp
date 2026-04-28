#include <bits/stdc++.h>
using namespace std;

void bellmanFord(int V, int E, int src, vector<vector<int>>& edges) {
    
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    for (int i = 0; i < V - 1; i++)
    {
        for (int j = 0; j < E; j++)
        {
            int u = edges[j][0];
            int v = edges[j][1];
            int wt = edges[j][2];

            if(dist[u] != INT_MAX && dist[u] + wt < dist[v])
            {
                dist[v] = dist[u] + wt;
            }
        }
    }

    for (int i = 0; i < V; i++)
    {
        cout<<dist[i]<< endl;
    }
}

int main()
{
    int V = 4, E = 4;

    vector<vector<int>> edges = {{0, 1, 5}, {1, 2, -2}, {2, 3, 3}, {0, 3, 10}};

    bellmanFord(V, E, 0, edges);
    return 0;
}