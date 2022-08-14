/*
 * @lc app=leetcode.cn id=919 lang=cpp
 *
 * [919] 完全二叉树插入器
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
class CBTInserter
{
public:
    CBTInserter(TreeNode *root)
    {
        if (root != nullptr)
        {
            root_ = root;
        }

        if (root_ != nullptr)
        {
            //层序遍历每个节点
            std::queue<TreeNode *> queue;
            queue.push(root);
            while (!queue.empty())
            {
                auto node = queue.front();
                queue.pop();

                if (node->left != nullptr)
                {
                    queue.push(node->left);
                }
                if (node->right != nullptr)
                {
                    queue.push(node->right);
                }
                else
                {
                    que_.push(node); //只保留左右子树不完整的节点
                }
            }
        }
    }

    int insert(int val)
    {
        auto node = que_.front();
        auto inode = new TreeNode(val);
        if (node->left == nullptr)
        {
            node->left = inode;
        }
        else
        {
            node->right = inode;
            que_.pop(); //左右子树完整的节点弹出
        }
        que_.push(inode);

        return node->val;
    }

    TreeNode *get_root()
    {
        return root_;
    }

private:
    TreeNode *root_ = nullptr;
    std::queue<TreeNode *> que_;
};

/**
 * Your CBTInserter object will be instantiated and called as such:
 * CBTInserter* obj = new CBTInserter(root);
 * int param_1 = obj->insert(val);
 * TreeNode* param_2 = obj->get_root();
 */
// @lc code=end
