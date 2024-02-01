/*
 * @lc app=leetcode.cn id=92 lang=cpp
 *
 * [92] 反转链表 II
 */

// @lc code=start
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
class Solution
{
public:
    ListNode *reverseBetween(ListNode *head, int left, int right)
    {
        ListNode dummy;
        dummy.next = head;

        auto cur = &dummy;
        ListNode *betweenpre = nullptr;
        ListNode *betweenend = nullptr;
        ListNode *pre = nullptr;
        for (int i = 0; i < right; ++i)
        {
            if (i == left - 1)
            {
                betweenpre = cur;
                betweenend = cur->next;
            }
            if (i > left)
            {
                auto next = cur->next;
                cur->next = pre;
                pre = cur;
                cur = next;
            }
            else
            {
                pre = cur;
                cur = cur->next;
            }
        }

        auto next = cur->next;
        cur->next = pre;
        betweenpre->next = cur;
        betweenend->next = next;

        return dummy.next;
    }
};
// @lc code=end
