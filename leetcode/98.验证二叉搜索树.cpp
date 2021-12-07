/*
 * @lc app=leetcode.cn id=98 lang=cpp
 *
 * [98] 验证二叉搜索树
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
    bool track(TreeNode *root, long long min, long long max)
    {
        if (root == nullptr)
        {
            return true;
        }

        if (root->val >= max || root->val <= min)
        {
            return false;
        }

        if (root->left && !track(root->left, min, root->val))
        {
            return false;
        }

        if (root->right && !track(root->right, root->val, max))
        {
            return false;
        }

        return true;
    }
    bool isValidBST(TreeNode *root)
    {
        return track(root, LONG_MIN, LONG_MAX);
    }
};
// @lc code=end
