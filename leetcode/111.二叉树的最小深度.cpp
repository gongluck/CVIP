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

        int depth = 1;
        //保存每一层所有节点
        std::queue<TreeNode *> nodes;
        nodes.push(root);

        while (!nodes.empty())
        {
            //获取当前层节点数
            auto n = nodes.size();
            //遍历这一层的节点
            for (int i = 0; i < n; ++i)
            {
                auto node = nodes.front();
                nodes.pop();
                if (node->left == nullptr && node->right == nullptr)
                {
                    return depth;
                }
                if (node->left != nullptr)
                {
                    nodes.push(node->left);
                }
                if (node->right != nullptr)
                {
                    nodes.push(node->right);
                }
            }
            //递进深度
            ++depth;
        }

        return depth;
    }
};
// @lc code=end
