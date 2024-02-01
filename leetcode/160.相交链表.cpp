/*
 * @lc app=leetcode.cn id=160 lang=cpp
 *
 * [160] 相交链表
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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB)
    {
        auto a = headA;
        auto b = headB;
        while (a != b)
        {
            a = (a ? a->next : headB);
            b = (b ? b->next : headA);
        }

        return a;
    }
};
// @lc code=end
