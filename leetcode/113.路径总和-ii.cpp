/*
 * @lc app=leetcode.cn id=113 lang=cpp
 *
 * [113] 路径总和 II
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
    vector<vector<int>> res;
    void track(TreeNode *root, int sum, int targetSum, std::vector<int>& tracks)
    {
        if (root == nullptr)
        {
            return;
        }

        sum += root->val;
        tracks.push_back(root->val);

        if (root->left == nullptr && root->right == nullptr)
        {
            if (sum == targetSum)
            {
                res.push_back(tracks);
            }
        }
        else
        {
            track(root->left, sum, targetSum, tracks);
            track(root->right, sum, targetSum, tracks);
        }

        tracks.pop_back();
    }
    vector<vector<int>> pathSum(TreeNode *root, int targetSum)
    {
        vector<int> tracks;
        track(root, 0, targetSum, tracks);
        return res;
    }
};
// @lc code=end
