/*
 * @lc app=leetcode.cn id=104 lang=cpp
 *
 * [104] 二叉树的最大深度
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
    int maxDepth(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        // return 1 + std::max(maxDepth(root->left), maxDepth(root->right));

        std::queue<TreeNode *> qu;
        qu.push(root);
        int h = 0;

        while (!qu.empty())
        {
            ++h;
            int n = qu.size();
            while (n-- > 0)
            {
                auto node = qu.front();
                qu.pop();
                if (node->left != nullptr)
                {
                    qu.push(node->left);
                }
                if (node->right != nullptr)
                {
                    qu.push(node->right);
                }
            }
        }

        return h;
    }
};
// @lc code=end
