#include<iostream>
using namespace std;

int main()
{
    int arr[] = {1,2,3,4,5,6,6,6,7,7,8,9};

    int n =sizeof(arr)/sizeof(arr[0]);

    int key = 6;
  
    int st = 0;
    int end = n;

    while(st < end)
    {
        int mid = st + (end - st)/2;

        if(arr[mid] < key)
            st = mid + 1;

        else
            end = mid;
    }
    
    cout<<"Lower bound of "<<key<<" is at index: "<<st<<endl;
    
    st = 0;
    end = n;

    while(st < end)
    {
        int mid = st + (end - st) / 2;
        if(arr[mid] <= key)
            st = mid + 1;

        else
            end = mid;
    }
    cout<<"Upper bound of "<<key<<" is at index: "<<end<<endl;
}