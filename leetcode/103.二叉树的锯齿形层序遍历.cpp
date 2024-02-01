/*
 * @lc app=leetcode.cn id=103 lang=cpp
 *
 * [103] 二叉树的锯齿形层序遍历
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
    vector<vector<int>> zigzagLevelOrder(TreeNode *root)
    {
        std::queue<TreeNode *> que;
        bool forward = true;
        std::vector<std::vector<int>> result;

        if (root != nullptr)
        {
            que.push(root);
        }
        while (!que.empty())
        {
            auto n = que.size();
            std::vector<int> nums;
            while (n--)
            {
                auto node = que.front();
                que.pop();
                nums.push_back(node->val);

                if (node->left != nullptr)
                {
                    que.push(node->left);
                }
                if (node->right != nullptr)
                {
                    que.push(node->right);
                }
            }

            if (!forward)
            {
                std::reverse(nums.begin(), nums.end());
            }
            result.push_back(nums);
            forward = !forward;
        }

        return result;
    }
};
// @lc code=end
