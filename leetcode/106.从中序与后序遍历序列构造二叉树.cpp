/*
 * @lc app=leetcode.cn id=106 lang=cpp
 *
 * [106] 从中序与后序遍历序列构造二叉树
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
    TreeNode *build(std::unordered_map<int, int> &inindexs, std::vector<int> &inorder, int instart, int inend,
                    std::vector<int> &postorder, int poststart, int postend)
    {
        if (instart >= inend)
        {
            return nullptr;
        }

        auto val = postorder[postend - 1];
        auto node = new TreeNode(val);
        auto leftcount = inindexs[val] - instart;

        node->left = build(inindexs, inorder, instart, inindexs[val], postorder, poststart, poststart + leftcount);
        node->right = build(inindexs, inorder, inindexs[val] + 1, inend, postorder, poststart + leftcount, postend - 1);

        return node;
    }

    TreeNode *buildTree(vector<int> &inorder, vector<int> &postorder)
    {
        int n = inorder.size();
        std::unordered_map<int, int> inindexs;

        for (int i = 0; i < n; ++i)
        {
            inindexs[inorder[i]] = i;
        }

        return build(inindexs, inorder, 0, n, postorder, 0, n);
    }
};
// @lc code=end
