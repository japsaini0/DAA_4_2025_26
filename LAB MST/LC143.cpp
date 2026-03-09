
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    void reorderList(ListNode* head) {

        if(!head || !head->next) return;
        
        ListNode* temp = head;
        ListNode* last = head;
        int count = 0;

        while(temp->next != nullptr)
        {
            temp = temp->next;
            count++;
        }

        last = temp;
        count++;

        for(int i = 0; i < count/2; i++)
        {     
            temp = head;   
        
            while(temp->next != last)
            {
                temp = temp->next;
            }

            temp->next = nullptr;

            last->next = head->next;
            head->next = last;

            last = temp;
        }
    }
};