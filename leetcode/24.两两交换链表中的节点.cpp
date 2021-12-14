/*
 * @lc app=leetcode.cn id=24 lang=cpp
 *
 * [24] 两两交换链表中的节点
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
    ListNode *swapPairs(ListNode *head)
    {
        ListNode dummy;
        dummy.next = head;
        auto p = &dummy;
        while (p->next && p->next->next)
        {
            auto tmp1 = p->next;
            auto tmp2 = p->next->next;
            tmp1->next = tmp2->next;
            tmp2->next = tmp1;
            p->next = tmp2;
            p = p->next->next;
        }
        return dummy.next;
    }
};
// @lc code=end
