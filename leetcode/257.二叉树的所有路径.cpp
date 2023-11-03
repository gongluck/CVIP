/*
 * @lc app=leetcode.cn id=257 lang=cpp
 *
 * [257] 二叉树的所有路径
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
    std::string makestr(const std::vector<TreeNode *> &nodes)
    {
        std::string str;
        for (int i = 0; i < nodes.size(); ++i)
        {
            if (i != 0)
            {
                str.append("->");
            }
            str.append(std::to_string(nodes[i]->val));
        }

        return str;
    }

    void travel(TreeNode *root, std::vector<TreeNode *> &nodes, std::vector<std::string> &result)
    {
        nodes.push_back(root);

        if (root->left == nullptr && root->right == nullptr)
        {
            result.push_back(makestr(nodes));
        }
        
        if (root->left != nullptr)
        {
            travel(root->left, nodes, result);
        }
        
        if (root->right != nullptr)
        {
            travel(root->right, nodes, result);
        }

        nodes.pop_back();
    }

    vector<string> binaryTreePaths(TreeNode *root)
    {
        std::vector<std::string> result;
        std::vector<TreeNode *> nodes;

        travel(root, nodes, result);

        return result;
    }
};
// @lc code=end
