#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

class DisjointSet {
    vector<int> parent;
    vector<int> sz;

public:
    DisjointSet(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
        sz.assign(n + 1, 1);
    }

    int findRoot(int x) {
        if (parent[x] == x) return x;
        return parent[x] = findRoot(parent[x]);
    }

    void unite(int a, int b) {
        int ra = findRoot(a);
        int rb = findRoot(b);
        if (ra != rb) {
            if (sz[ra] < sz[rb]) swap(ra, rb);
            parent[rb] = ra;
            sz[ra] += sz[rb];
        }
    }
};

int main() {
    int V = 5;
    vector<pair<int, int>> edges = {{0, 1}, {0, 2}, {1, 3}, {3, 4}, {1, 2}};

    DisjointSet dsu(V);
    bool cycle = false;

    for (auto &e : edges) {
        int u = e.first, v = e.second;
        int ru = dsu.findRoot(u);
        int rv = dsu.findRoot(v);
        if (ru == rv) {
            cout << "Cycle detected on edge " << u << " - " << v << endl;
            cycle = true;
        } else {
            dsu.unite(u, v);
        }
    }

    if (!cycle) cout << "No cycle found." << endl;
    return 0;
}