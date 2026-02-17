#include <iostream>
#include <vector>

using namespace std;

int countPairs(vector <int> &arr1, vector <int> &arr2, int key)
{
    int count = 0;

    int n = arr1.size();

    int i = 0;
    int j = n - 1;

    while(i < n && j >= 0)
    {
        int sum = arr1[i] + arr2[j];

        if(sum == key)
        {
            count++;
            j--;
        }

        else if(sum > key)
        {
            j--;
        }

        else
        {
            i++;
        }
    }

    return count;
}

int main()
{
    vector <int> arr1 = {1, 3, 5, 6, 9, 11};
    vector <int> arr2 = {1, 1, 2, 3, 4, 5};

    int key = 7;

    cout<<"Count of pairs with sum as "<<key<<" are : "<<countPairs(arr1, arr2, key);

    return 0;
}