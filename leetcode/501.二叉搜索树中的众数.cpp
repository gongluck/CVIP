/*
 * @lc app=leetcode.cn id=501 lang=cpp
 *
 * [501] 二叉搜索树中的众数
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
    void find(TreeNode *&pre, TreeNode *root, std::vector<int> &nums, int &maxcount, int &count)
    {
        if (root == nullptr)
        {
            return;
        }

        find(pre, root->left, nums, maxcount, count);
        if (pre != nullptr && pre->val == root->val)
        {
            ++count;
        }
        else
        {
            count = 1;
        }

        if (count > maxcount)
        {
            maxcount = count;
            nums.clear();
            nums.push_back(root->val);
        }
        else if (count == maxcount)
        {
            nums.push_back(root->val);
        }

        pre = root;
        find(pre, root->right, nums, maxcount, count);

        return;
    }

    vector<int> findMode(TreeNode *root)
    {
        TreeNode *pre = nullptr;
        std::vector<int> nums;
        int maxcount = INT_MIN;
        int count = 0;

        find(pre, root, nums, maxcount, count);

        return nums;
    }
};
// @lc code=end
