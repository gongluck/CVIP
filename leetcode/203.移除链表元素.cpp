/*
 * @lc app=leetcode.cn id=203 lang=cpp
 *
 * [203] 移除链表元素
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
    ListNode *removeElements(ListNode *head, int val)
    {
        ListNode dummy(0, head);
        ListNode *pcur = &dummy;

        while (pcur->next != nullptr)
        {
            if (pcur->next->val == val)
            {
                pcur->next = pcur->next->next;
            }
            else
            {
                pcur = pcur->next;
            }
        }

        return dummy.next;
    }
};
// @lc code=end
