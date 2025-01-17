/*
 * @lc app=leetcode.cn id=111 lang=cpp
 *
 * [111] 二叉树的最小深度
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
    int minDepth(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        // if (root->left == nullptr && root->right == nullptr)
        // {
        //     return 1;
        // }
        // else if (root->left == nullptr)
        // {
        //     return 1 + minDepth(root->right);
        // }
        // else if (root->right == nullptr)
        // {
        //     return 1 + minDepth(root->left);
        // }
        // else
        // {
        //     return 1 + std::min(minDepth(root->left), minDepth(root->right));
        // }

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

                if (node->left == nullptr && node->right == nullptr)
                {
                    return h;
                }

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
