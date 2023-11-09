/*
 * @lc app=leetcode.cn id=654 lang=cpp
 *
 * [654] 最大二叉树
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution
{
public:
    TreeNode *construct(const std::vector<int> &nums, int start, int end)
    {
        if (start >= end)
        {
            return nullptr;
        }

        int index = start;
        for (int i = start + 1; i < end; ++i)
        {
            if (nums[i] > nums[index])
            {
                index = i;
            }
        }

        auto node = new TreeNode(nums[index]);
        node->left = construct(nums, start, index);
        node->right = construct(nums, index + 1, end);

        return node;
    }

    TreeNode *constructMaximumBinaryTree(vector<int> &nums)
    {
        return construct(nums, 0, nums.size());
    }
};
// @lc code=end
