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
        int lenA = 0;
        int lenB = 0;
        auto pA = headA;
        auto pB = headB;
        while (pA)
        {
            ++lenA;
            pA = pA->next;
        }
        while (pB)
        {
            ++lenB;
            pB = pB->next;
        }

        if (lenA < lenB)
        {
            std::swap(lenA, lenB);
            std::swap(headA, headB);
        }
        pA = headA;
        pB = headB;
        int skip = lenA - lenB;
        while (skip-- > 0)
        {
            pA = pA->next;
        }
        while (pA)
        {
            if (pA == pB)
            {
                return pA;
            }
            pA = pA->next;
            pB = pB->next;
        }

        return nullptr;
    }
};
// @lc code=end
