/*
 * @lc app=leetcode.cn id=94 lang=cpp
 *
 * [94] 二叉树的中序遍历
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

        traversal(result, root->left);
        result.emplace_back(root->val);
        traversal(result, root->right);
    }

    vector<int> inorderTraversal(TreeNode *root)
    {
        std::vector<int> result;

        // traversal(result, root);

        std::stack<TreeNode *> st;
        auto cur = root;

        while (cur != nullptr || !st.empty())
        {
            while (cur != nullptr)
            {
                st.push(cur);
                cur = cur->left;
            }

            cur = st.top();
            st.pop();
            result.emplace_back(cur->val);

            cur = cur->right;
        }

        return result;
    }
};
// @lc code=end
