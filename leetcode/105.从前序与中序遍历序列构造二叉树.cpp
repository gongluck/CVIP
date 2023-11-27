/*
 * @lc app=leetcode.cn id=105 lang=cpp
 *
 * [105] 从前序与中序遍历序列构造二叉树
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

#include <unordered_map>

class Solution
{
public:
    TreeNode *build(std::unordered_map<int, int> &maps,
                    const std::vector<int> &preorder, int pstart, int pend,
                    const std::vector<int> &inorder, int istart, int iend)
    {
        if (pstart >= pend)
        {
            return nullptr;
        }

        auto node = new TreeNode(preorder[pstart]);
        auto index = maps[node->val];
        auto leftlen = index - istart;
        node->left = build(maps, preorder, pstart + 1, pstart + 1 + leftlen, inorder, istart, index);
        node->right = build(maps, preorder, pstart + 1 + leftlen, pend, inorder, index + 1, iend);

        return node;
    }

    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
    {
        std::unordered_map<int, int> maps;
        for (int i = 0; i < inorder.size(); ++i)
        {
            maps[inorder[i]] = i;
        }

        return build(maps, preorder, 0, preorder.size(), inorder, 0, inorder.size());
    }
};
// @lc code=end
