/*
 * @lc app=leetcode.cn id=102 lang=cpp
 *
 * [102] 二叉树的层序遍历
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
    vector<vector<int>> levelOrder(TreeNode *root)
    {
        std::vector<std::vector<int>> result;
        std::queue<TreeNode *> qu;

        if (root != nullptr)
        {
            qu.push(root);
        }

        while (!qu.empty())
        {
            int n = qu.size();
            std::vector<int> ret(n);
            for (int i = 0; i < n; ++i)
            {
                auto node = qu.front();
                if (node->left != nullptr)
                {
                    qu.push(node->left);
                }
                if (node->right != nullptr)
                {
                    qu.push(node->right);
                }
                ret[i] = node->val;
                qu.pop();
            }
            result.emplace_back(ret);
        }

        return result;
    }
};
// @lc code=end
