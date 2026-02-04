// We need to find the K-th highest score as numbers keep coming.  
// Basically we need to maintain the Top K largest numbers seen so far.  
// The smallest number in this "Top K" group is the K-th highest overall.  

// So I used a Min-Heap of size K:  
// 1. Min heap keeps the smallest element at the top.  
// 2. If heap has < K elements -> just add the new score.  
// 3. If heap is full (size K) -> compare new score with top (smallest).  
//    - If new score > top, remove top and add new score.  
//    - If new score is small, ignore it.  
// 4. If heap size < K output is -1, else output the top element.  

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main()
{
    int K, N;

    cout << "Enter K: ";
    cin >> K;

    cout << "Enter N: ";
    cin >> N;

    priority_queue<int, vector<int>, greater<int>> pq;

    cout << "Enter scores:" << endl;
    for (int i = 0; i < N; i++)
    {
        int val;
        cin >> val;

        if (pq.size() < K)
        {
            pq.push(val);
        } 
        
        else
        {      
            if(val > pq.top())
            {
                pq.pop();
                pq.push(val);
            }
        }

        if (pq.size() < K)
        {
            cout << "-1" << endl;
        }
        
        else
        {
            cout << pq.top() << endl;
        }
    }

    return 0;
}