/*
 * @lc app=leetcode.cn id=110 lang=cpp
 *
 * [110] 平衡二叉树
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
    int getheight(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        auto hleft = getheight(root->left);
        auto hright = getheight(root->right);

        if (hleft == -1 || hright == -1 || std::abs(hleft - hright) > 1)
        {
            return -1;
        }
        else
        {
            return 1 + std::max(hleft, hright);
        }
    }

    bool isBalanced(TreeNode *root)
    {
        return getheight(root) != -1;
    }
};
// @lc code=end
