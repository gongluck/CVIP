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
        auto fast = head;
        auto slow = head;

        while (fast != nullptr && fast->next != nullptr)
        {
            fast = fast->next->next;
            slow = slow->next;
            if (fast == slow)
            {
                slow = head;
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

        return nullptr;
    }
};
// @lc code=end
