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
        //常规解法
        // while (head && head->val == val)
        // {
        //     auto tmp = head;
        //     head = head->next;
        //     delete (tmp);
        // }

        // auto pre = head;
        // while (pre && pre->next)
        // {
        //     if (pre->next->val == val)
        //     {
        //         auto tmp = pre->next;
        //         pre->next = pre->next->next;
        //         delete (tmp);
        //         continue;
        //     }
        //     else
        //     {
        //         pre = pre->next;
        //         continue;
        //     }
        // }

        //虚拟头节点
        ListNode dummy(0);
        dummy.next = head;

        auto pre = &dummy;
        while (pre && pre->next)
        {
            if (pre->next->val == val)
            {
                auto tmp = pre->next;
                pre->next = pre->next->next;
                delete tmp;
            }
            else
            {
                pre = pre->next;
            }
        }

        return dummy.next;
    }
};
// @lc code=end
