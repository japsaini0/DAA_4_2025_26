#include <iostream>
using namespace std;

const int MAX = 100;   // maximum size of stack
int stack[MAX], t = -1;

void push(int x)
{
    if (t == MAX - 1) 
    {
        cout << "Stack Overflow! Cannot push " << x << endl;
        return;
    }
    t++;
    stack[t] = x;
}

void pop()
{
    if (t == -1)
    {
        cout << "Stack Underflow! Cannot pop." << endl;
        return;
    }

    t--;
}

void display()
{
    if (t == -1) {
        cout << "Stack is empty!" << endl;
        return;
    }
   
    for (int i = 0; i <= t; i++) {
        cout << stack[i] << " ";
    }
    cout << endl;
}

int main()
{
    push(3);
    push(7);
    push(9);
    push(13);
    push(17);
    pop();
    push(21);
    display();

    return 0;
}
