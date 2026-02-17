#include <iostream>
using namespace std;

int bSearch(int arr[], int n, int key)
{
    int st = 0;
    int end = n - 1;

    while(st <= end)
    {
        int mid = st + (end - st)/2;

        if(arr[mid] > key)
        {
            end = mid - 1;
        }

        else if(arr[mid] < key)
        {
            st = mid + 1;
        }

        else
        {
            return mid;
        }
    }

    return -1;
}

int main()
{
    int n, pos, key;

    int arr[7] = {11, 3, 5, 7, 9, 11, 13};
    
    n = sizeof(arr)/sizeof(arr[0]);
    
    cout<<"Enter the element you want to find : ";
    cin>>key;

    pos = bSearch(arr, n, key);

    if(pos > -1)
    {
        cout<<"Element found at position : "<<(pos + 1);
    }

    else
    {
        cout << "Element not found in the array.";
    }

    return 0;
}