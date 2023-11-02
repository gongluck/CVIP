/*
 * @lc app=leetcode.cn id=226 lang=cpp
 *
 * [226] 翻转二叉树
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
    TreeNode *invertTree(TreeNode *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        // std::swap(root->left, root->right);
        // invertTree(root->left);
        // invertTree(root->right);

        std::stack<TreeNode *> st;
        st.push(root);

        while (!st.empty())
        {
            auto node = st.top();
            st.pop();
            std::swap(node->left, node->right);
            if (node->left != nullptr)
            {
                st.push(node->left);
            }
            if (node->right != nullptr)
            {
                st.push(node->right);
            }
        }

        return root;
    }
};
// @lc code=end
