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
    TreeNode *dfs(std::unordered_map<int, int> &indexmap,
                  vector<int> &preorder, int prestart, int prend,
                  vector<int> &inorder, int instart, int inend)
    {
        //没有root节点
        if (prestart >= prend)
        {
            return nullptr;
        }

        auto root = new TreeNode(preorder[prestart]);
        auto leftsize = indexmap[root->val] - instart;
        root->left = dfs(indexmap, preorder, prestart + 1, prestart + 1 + leftsize, inorder, instart, indexmap[root->val]);
        root->right = dfs(indexmap, preorder, prestart + 1 + leftsize, prend, inorder, indexmap[root->val] + 1, inend);

        return root;
    }
    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
    {
        std::unordered_map<int, int> indexmap;
        for (int i = 0; i < inorder.size(); ++i)
        {
            indexmap[inorder[i]] = i;
        }
        return dfs(indexmap, preorder, 0, preorder.size(), inorder, 0, inorder.size());
    }
};
// @lc code=end
