// Japjit Singh
// 24BCS10785
// Section - 24_KRG-711A



#include <iostream>
using namespace std;

class node
{
    public :
    int data;
    node *next;

    node(int x)
    {
        data = x;
        next = NULL;
    }
};

class Nop
{
    public :
    void InsertStart(node * &head, int x)
    {
        node * newnode = new node(x);
        newnode->next = head;
        head = newnode;
    }

    void InsertEnd(node *&head, int x)
    {
        node * newnode = new node(x);

        if(head == NULL)
        {
            head = newnode;
            return;
        }

        node *temp = head;

        while(temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newnode;
    }
    
    void print(node * &head)
    {
        node * temp = head;
        
        while(temp != NULL)
        {
            cout<<temp->data<<endl;
            temp = temp->next;
        }
    }

    void deleteEnd(node * &head)
    {
        node * temp = head;

        if(head == NULL) return;

        while(temp->next->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = NULL;
    }

    void deleteStart(node * &head)
    {
        if(head == NULL) return;

        node * temp = head;
        head = head->next;
        delete temp;
    }

    void Insertlocation(node * &head, int x, int pos)
    {
        node * temp = head;
        int count = 1;

        if(head == NULL)
        {
            InsertStart(head, x);
            return;
        }

        while(count < pos - 1)
        {
            temp = temp->next;
            count++;
        }

        node * newnode = new node(x);
        newnode->next = temp->next;
        temp->next = newnode;
    }

    void deletelocation(node * &head, int pos)
    {
        node * prev = NULL;
        node * curr = head;
        int loc = 1;

        if(head == NULL) return;

        if(loc == pos)
        {
            deleteStart(head);
        }

        while(loc < pos)
        {
            prev = curr;
            curr =  curr->next;
            loc++;
        }

        prev->next = curr->next;
        delete curr; 
    }
};


int main()
{
    node * head = NULL;
    Nop s;

    s.InsertEnd(head, 27);
    s.InsertStart(head, 9);
    s.InsertStart(head, 99);
    s.InsertStart(head, 81);
    s.InsertStart(head, 111);
    s.InsertStart(head, 227);
    s.InsertStart(head, 7);
    s.InsertStart(head, 29);
    s.Insertlocation(head,54, 1);
    s.deleteStart(head);
    s.deletelocation(head, 3);
    s.print(head);

}
