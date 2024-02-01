/*
 * @lc app=leetcode.cn id=143 lang=cpp
 *
 * [143] 重排链表
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
    void reorderList(ListNode *head)
    {
        std::deque<ListNode *> deq;

        ListNode *cur = head;
        while (cur != nullptr)
        {
            deq.push_back(cur);
            cur = cur->next;
        }

        ListNode dummy;
        ListNode *pre = &dummy;
        while (!deq.empty())
        {
            cur = deq.front();
            deq.pop_front();
            pre->next = cur;
            pre = cur;

            if (deq.size() > 0)
            {
                cur = deq.back();
                deq.pop_back();
                pre->next = cur;
                pre = cur;
            }
        }

        cur->next = nullptr;
    }
};
// @lc code=end
