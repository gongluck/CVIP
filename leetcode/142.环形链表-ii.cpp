/*
 * @lc app=leetcode.cn id=142 lang=cpp
 *
 * [142] 环形链表 II
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution
{
public:
    ListNode *detectCycle(ListNode *head)
    {
        ListNode *fast = head;
        ListNode *slow = head;

        while (true)
        {
            if (fast == nullptr || fast->next == nullptr)
            {
                return nullptr;
            }
            fast = fast->next->next;
            slow = slow->next;
            if (fast == slow)
            {
                fast = head;
                while (true)
                {
                    if (fast == slow)
                    {
                        return fast;
                    }
                    fast = fast->next;
                    slow = slow->next;
                }
            }
        }
    }
};
// @lc code=end
