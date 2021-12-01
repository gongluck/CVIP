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
        vector<vector<int>> res;
        if (root == nullptr)
        {
            return res;
        }
        std::queue<struct TreeNode *> q;
        q.push(root);

        while (!q.empty())
        {
            auto n = q.size();
            std::vector<int> tmp;
            for (int i = 0; i < n; ++i)
            {
                auto cur = q.front();
                q.pop();
                tmp.push_back(cur->val);
                if (cur->left != nullptr)
                {
                    q.push(cur->left);
                }
                if (cur->right != nullptr)
                {
                    q.push(cur->right);
                }
            }
            res.push_back(tmp);
        }

        return res;
    }
};
// @lc code=end
