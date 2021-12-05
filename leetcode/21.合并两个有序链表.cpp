/*
 * @lc app=leetcode.cn id=21 lang=cpp
 *
 * [21] 合并两个有序链表
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
    ListNode *mergeTwoLists(ListNode *list1, ListNode *list2)
    {
        ListNode dummy;
        ListNode *tail = &dummy;

        while (list1 || list2)
        {
            if (!list2)
            {
                tail->next = list1;
                list1 = nullptr;
            }
            else if (!list1)
            {
                tail->next = list2;
                list2 = nullptr;
            }
            else
            {
                if (list1->val < list2->val)
                {
                    tail->next = list1;
                    tail = tail->next;
                    list1 = list1->next;
                }
                else
                {
                    tail->next = list2;
                    tail = tail->next;
                    list2 = list2->next;
                }
            }
        }
        return dummy.next;
    }
};
// @lc code=end
