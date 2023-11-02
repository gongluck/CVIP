/*
 * @lc app=leetcode.cn id=144 lang=cpp
 *
 * [144] 二叉树的前序遍历
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
    void traversal(std::vector<int> &result, TreeNode *root)
    {
        if (root == nullptr)
        {
            return;
        }

        result.emplace_back(root->val);
        traversal(result, root->left);
        traversal(result, root->right);
    }

    vector<int> preorderTraversal(TreeNode *root)
    {
        std::vector<int> result;

        // traversal(result, root);

        std::stack<TreeNode *> st;
        if (root != nullptr)
        {
            st.push(root);
        }

        while (!st.empty())
        {
            auto node = st.top();
            st.pop();

            result.emplace_back(node->val);
            if (node->right != nullptr)
            {
                st.push(node->right);
            }
            if (node->left != nullptr)
            {
                st.push(node->left);
            }
        }

        return result;
    }
};
// @lc code=end
