/*
 * @lc app=leetcode.cn id=199 lang=cpp
 *
 * [199] 二叉树的右视图
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
    vector<int> rightSideView(TreeNode *root)
    {
        std::vector<int> result;
        if (root == nullptr)
        {
            return result;
        }

        std::deque<TreeNode *> deq;

        deq.push_back(root);
        while (!deq.empty())
        {
            int n = deq.size();
            result.push_back(deq.back()->val);
            for (int i = 0; i < n; ++i)
            {
                auto node = deq.front();
                deq.pop_front();
                if (node->left != nullptr)
                {
                    deq.push_back(node->left);
                }
                if (node->right != nullptr)
                {
                    deq.push_back(node->right);
                }
            }
        }

        return result;
    }
};
// @lc code=end
