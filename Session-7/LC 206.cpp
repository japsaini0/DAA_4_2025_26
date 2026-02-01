<<<<<<< HEAD
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

//  Q 206   
class Solution {
public:
    ListNode* reverseList(ListNode* head) {

        ListNode * prev = NULL;
        ListNode * curr = head;
        ListNode * next = NULL;

        while(curr != NULL)
        {
            next = curr->next;
            curr->next = prev;

            prev = curr;
            curr = next;
        }           

        return prev;
    }
=======
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

//  Q 206   
class Solution {
public:
    ListNode* reverseList(ListNode* head) {

        ListNode * prev = NULL;
        ListNode * curr = head;
        ListNode * next = NULL;

        while(curr != NULL)
        {
            next = curr->next;
            curr->next = prev;

            prev = curr;
            curr = next;
        }           

        return prev;
    }
>>>>>>> f74c3c0cd43c0f31635588bf67b0a980e9022093
};