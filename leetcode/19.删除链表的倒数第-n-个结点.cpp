/*
 * @lc app=leetcode.cn id=19 lang=cpp
 *
 * [19] 删除链表的倒数第 N 个结点
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
    ListNode *removeNthFromEnd(ListNode *head, int n)
    {
        ListNode dummy(0, head);
        auto fast = &dummy;
        auto slow = &dummy;

        while (fast->next != nullptr)
        {
            fast = fast->next;
            if (n-- <= 0)
            {
                slow = slow->next;
            }
        }

        auto tmp = slow->next->next;
        delete slow->next;
        slow->next = tmp;

        return dummy.next;
    }
};
// @lc code=end
