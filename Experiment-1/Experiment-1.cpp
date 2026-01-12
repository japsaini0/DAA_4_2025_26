//#include <bits/stdc++.h>


//UID - 24BCS10785
//Name - Japjit Singh
//Section - 24KRG_711-A

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

void complexRec(int n, int &operations, int depth, int &maxDepth) {

    maxDepth = max(depth, maxDepth);
    operations++;

    //O(1)
   if (n <= 2) {
       return;
    } 
    operations++;


    //O(nlogn)
   int p = n;
   while (p > 0) {
       vector<int> temp(n);
       for (int i = 0; i < n; i++) {
           temp[i] = i ^ p;
           operations++;
       }
       p >>= 1;
       operations++;
   } 

   //O(n)
   vector<int> small(n);
   for (int i = 0; i < n; i++) {
       small[i] = i * i;
       operations++;
   } 

   //O(n/2)
   if (n % 3 == 0) {
       reverse(small.begin(), small.end());
       operations++;
   } else {
       reverse(small.begin(), small.end());
       operations++;
   } 


   complexRec(n / 2, operations, depth + 1, maxDepth); //T(n/2)
   complexRec(n / 2, operations, depth + 1, maxDepth); //T(n/2)
   complexRec(n / 2, operations, depth + 1, maxDepth); //T(n/2)
}

int main()
{
    auto st = high_resolution_clock::now();

    int operations = 0;
    int depth = 1;
    int maxDepth = 1;

    int size;
    
    cout<<"Enter size : ";
    cin>>size;

    complexRec(size, operations, depth, maxDepth);
    
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - st);

    cout<<"Duration : "<<duration.count()<<"ms"<<endl;
    cout<<"Number of Operations :"<<operations<<endl;
    cout<<"Depth : "<<depth<<endl;

    cout<<"Recurrance Relation T(n) = 3T(n/2) + nlogn";

    return 0;

}



//Cost = 1 + nlogn + n + n/2 = O(nlogn)
//Recurrance Relation of above code is
//T(n) = 3T(n/2) + nlogn

//Using Master Theorem Case 1 : a > b^k
//O(n^(log base b of a))

//Time Complexity = O(n^(log base 2 of 3))