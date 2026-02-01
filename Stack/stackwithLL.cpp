#include <iostream>
using namespace std;

class node
{
    public :
    int data;
    node * next;

    node(int x)
    {
        data = x;
        next = nullptr;
    }
};

class Stack
{
    public : 
    node * head = nullptr;
    int count = 0;

    void insert(int x)
    {
        if(head == nullptr)
        {
            node * newnode = new node(x);
            head = newnode;
            count++;

            return;
        }

        node * newnode = new node(x);
        newnode->next = head;
        head = newnode;
        count++;
    }

    void pop()
    {
        if(head == nullptr) return;

        node * temp = head;
        head = head->next;

        delete temp;

        if(count > 1)
        {
            count--;
        }
    }

    void traverse()
    {
        node * temp = head;

        cout<<"Elements in Stack : ";

        while(temp != nullptr)
        {
            cout<<temp->data<<" ";
            temp = temp->next;
        }

        cout<<endl;
    }

    void peek()
    {
        cout<<"Top element : "<<head->data<<endl;
    }

    bool isempty()
    {
        if(head == nullptr)
        {
            return true;
        }

        return false;
    }

};

int main()
{
    Stack jp;
    jp.insert(9);
    jp.insert(27);
    jp.insert(81);
    jp.pop();

    jp.traverse();
    jp.peek();
   
    if(!jp.isempty())
    {
        cout<<"Stack is not empty!";
    }   

    else
    {
        cout<<"Stack is Empty";
    }
    
    cout<<endl<<"Total elements in Stack : "<<jp.count;

    return 0;
}