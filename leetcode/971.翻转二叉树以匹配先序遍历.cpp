/*
 * @lc app=leetcode.cn id=971 lang=cpp
 *
 * [971] 翻转二叉树以匹配先序遍历
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
    bool flipMatch(std::vector<int> &result, TreeNode *root, const vector<int> &voyage, int &index)
    {
        if (root == nullptr)
        {
            return true;
        }

        if (root->val != voyage[index])
        {
            return false;
        }

        ++index;

        if (root->left != nullptr && root->left->val != voyage[index])
        {
            result.push_back(root->val);
            std::swap(root->left, root->right);
        }

        return flipMatch(result, root->left, voyage, index) && flipMatch(result, root->right, voyage, index);
    }

    vector<int> flipMatchVoyage(TreeNode *root, vector<int> &voyage)
    {
        std::vector<int> result;
        int index = 0;

        return flipMatch(result, root, voyage, index) ? result : std::vector<int>{-1};
    }
};
// @lc code=end
