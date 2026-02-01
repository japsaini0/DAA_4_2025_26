#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<string>

using namespace std;

int main() {
    int n;
    cin >> n;


    //Input here

    vector<char> attendanceVector(n);
    for (int i = 0; i < n; i++) {
        cin >> attendanceVector[i];
    }

    unordered_map<int, int> mp; //used unordered_map
    
    mp[0] = -1; 

    int currentSum = 0;
    int maxLength = 0;


    for (int i = 0; i < n; i++) {
        if (attendanceVector[i] == 'P') {
            currentSum += 1;
        } else {
            currentSum -= 1;
        }


        if (mp.find(currentSum) != mp.end()) {
            int windowLength = i - mp[currentSum];
            maxLength = max(maxLength, windowLength);
        } else {
            mp[currentSum] = i;
        }
    }

    cout << maxLength << endl; //printing result

    return 0;
}



// Sample Input
//9
//P 
//P
//P
//A
//A
//A
//P
//A
//P
// OUTPUT

//8   