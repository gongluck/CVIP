/*
 * @lc app=leetcode.cn id=206 lang=cpp
 *
 * [206] 反转链表
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
    ListNode *reverse(ListNode *pre, ListNode *cur)
    {
        if (cur == nullptr)
        {
            return pre;
        }

        auto tmp = cur->next;
        cur->next = pre;
        return reverse(cur, tmp);
    }

    ListNode *reverseList(ListNode *head)
    {
        // ListNode *pre = nullptr;
        // ListNode *cur = head;

        // while (cur != nullptr)
        // {
        //     auto tmp = cur->next;
        //     cur->next = pre;
        //     pre = cur;
        //     cur = tmp;
        // }

        // return pre;

        return reverse(nullptr, head);
    }
};
// @lc code=end
