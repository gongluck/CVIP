/*
 * @lc app=leetcode.cn id=863 lang=cpp
 *
 * [863] 二叉树中所有距离为 K 的结点
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution
{
public:
    void makeparents(std::unordered_map<int, TreeNode *>& parents, TreeNode *root)
    {
        if (root == nullptr)
        {
            return;
        }

        if (root->left != nullptr)
        {
            parents[root->left->val] = root;
            makeparents(parents, root->left);
        }
        if (root->right != nullptr)
        {
            parents[root->right->val] = root;
            makeparents(parents, root->right);
        }

        return;
    }

    void search(std::unordered_map<int, TreeNode *> parents, TreeNode *from, TreeNode *root, int k, int step, std::vector<int> &result)
    {
        if (root == nullptr)
        {
            return;
        }

        if (k == step)
        {
            result.push_back(root->val);
            return;
        }

        if (from != root->left)
        {
            search(parents, root, root->left, k, step + 1, result);
        }
        if (from != root->right)
        {
            search(parents, root, root->right, k, step + 1, result);
        }
        if (from != parents[root->val])
        {
            search(parents, root, parents[root->val], k, step + 1, result);
        }

        return;
    }

    vector<int> distanceK(TreeNode *root, TreeNode *target, int k)
    {
        std::unordered_map<int, TreeNode *> parents;
        makeparents(parents, root);

        std::vector<int> result;
        search(parents, nullptr, target, k, 0, result);

        return result;
    }
};
// @lc code=end
