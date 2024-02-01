/*
 * @lc app=leetcode.cn id=101 lang=cpp
 *
 * [101] 对称二叉树
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
    bool isSymmetric(TreeNode *root)
    {
        std::stack<std::pair<TreeNode *, TreeNode *>> st;
        st.push(std::make_pair(root->left, root->right));

        while (!st.empty())
        {
            auto nodes = st.top();
            st.pop();
            if (nodes.first != nodes.second &&
                (nodes.first == nullptr || nodes.second == nullptr || nodes.first->val != nodes.second->val))
            {
                return false;
            }
            if (nodes.first != nullptr && nodes.second != nullptr)
            {
                st.push(std::make_pair(nodes.first->left, nodes.second->right));
                st.push(std::make_pair(nodes.first->right, nodes.second->left));
            }
        }

        return true;
    }
};
// @lc code=end
