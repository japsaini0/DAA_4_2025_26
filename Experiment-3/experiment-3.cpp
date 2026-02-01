#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<string>

using namespace std;

int main() {
    int N;
    cin >> N;


    //Taking Input

    vector<char> attendanceWindowVector(N);
    for (int i = 0; i < N; i++) {
        cin >> attendanceWindowVector[i];
    }

    unordered_map<int, int> mp; //used unordered_map
    
    mp[0] = -1; //I Used this to handle Edge Case


    //Variables needed for Calculation
    int currentSum = 0;
    int maxLength = 0;


    //Finding Prefix Sum and Maximum Length [added 1 for 'P' and -1 for 'A']

    for (int i = 0; i < N; i++) {
        if (attendanceWindowVector[i] == 'P') {
            currentSum += 1;
        } else {
            currentSum -= 1;
        }


        //checking if currentSum is already in map
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



// USED TEST CASE
// Input:
// 6
//P    
//P
//A
//A
//P
//P

// OUTPUT

// 4   