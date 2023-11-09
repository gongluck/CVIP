/*
 * @lc app=leetcode.cn id=235 lang=cpp
 *
 * [235] 二叉搜索树的最近公共祖先
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

class Solution
{
public:
    TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
    {
        auto min = std::min(p->val, q->val);
        auto max = std::max(p->val, q->val);

        while (root != nullptr)
        {
            if ((root->val > min && root->val < max) || root->val == min || root->val == max)
            {
                return root;
            }
            else if (root->val < min)
            {
                root = root->right;
            }
            else
            {
                root = root->left;
            }
        }

        return root;
    }
};
// @lc code=end
