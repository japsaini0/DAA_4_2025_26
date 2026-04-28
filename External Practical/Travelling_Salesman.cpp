#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int ans;

void tspBacktrack(vector<vector<int>>& cost, vector<bool>& visited, int curr, int n, int count, int currCost)
{
    if(count == n)
    {
        ans = min(ans, currCost + cost[curr][0]);
        return;
    }

    for(int i = 0; i < n; i++)
    {
        if(!visited[i] && currCost + cost[curr][i] < ans)
        {
            visited[i] = true;
            tspBacktrack(cost, visited, i, n, count + 1, currCost + cost[curr][i]);
            visited[i] = false;
        }
    }
}

int tsp(vector<vector<int>>& cost)
{
    int n = cost.size();
    vector<bool> visited(n, false);
    ans = INT_MAX;

    visited[0] = true;
    tspBacktrack(cost, visited, 0, n, 1, 0);

    return ans;
}

int main()
{
    vector<vector<int>> cost = {{0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}};

    int res = tsp(cost);
    cout << res << endl;

    return 0;
}
