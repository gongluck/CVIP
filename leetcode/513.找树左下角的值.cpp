/*
 * @lc app=leetcode.cn id=513 lang=cpp
 *
 * [513] 找树左下角的值
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
    int findBottomLeftValue(TreeNode *root)
    {
        auto leftnode = root;
        std::queue<TreeNode *> qu;

        qu.push(root);

        while (!qu.empty())
        {
            auto n = qu.size();
            for (int i = 0; i < n; ++i)
            {
                auto node = qu.front();
                qu.pop();
                if (i == 0)
                {
                    leftnode = node;
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

        return leftnode->val;
    }
};
// @lc code=end
