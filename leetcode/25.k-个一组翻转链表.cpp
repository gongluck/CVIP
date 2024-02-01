/*
 * @lc app=leetcode.cn id=25 lang=cpp
 *
 * [25] K 个一组翻转链表
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
    ListNode *reverseKGroup(ListNode *head, int k)
    {
        int n = 0;
        for (auto node = head; node != nullptr; node = node->next)
        {
            ++n;
        }

        ListNode dummy;
        dummy.next = head;
        auto phead = &dummy;
        ListNode *pre = nullptr;
        auto cur = phead->next;
        for (; n >= k; n -= k)
        {
            for (int i = 0; i < k; ++i)
            {
                auto next = cur->next;
                cur->next = pre;
                pre = cur;
                cur = next;
            }
            auto tail = phead->next;
            tail->next = cur;
            phead->next = pre;
            phead = tail;
        }

        return dummy.next;
    }
};
// @lc code=end
