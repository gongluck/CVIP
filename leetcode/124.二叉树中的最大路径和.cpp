/*
 * @lc app=leetcode.cn id=124 lang=cpp
 *
 * [124] 二叉树中的最大路径和
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
    int res = INT_MIN;
    //深度遍历
    int dfs(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        //左右分支只取有正增长部分
        auto left = std::max(0, dfs(root->left));
        auto right = std::max(0, dfs(root->right));

        res = std::max(res, left + root->val + right);

        //root到任一分支的最大值
        return std::max(left, right) + root->val;
    }
    int maxPathSum(TreeNode *root)
    {
        dfs(root);
        return res;
    }
};
// @lc code=end
