/*
 * @lc app=leetcode.cn id=117 lang=cpp
 *
 * [117] 填充每个节点的下一个右侧节点指针 II
 */

// @lc code=start
/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/

class Solution
{
public:
    Node *connect(Node *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        std::queue<Node *> q;
        q.push(root);
        while (!q.empty())
        {
            auto n = q.size();
            Node *last = nullptr;
            while (n-- > 0)
            {
                auto cur = q.front();
                if (last != nullptr)
                {
                    last->next = cur;
                }
                if (cur->left != nullptr)
                {
                    q.push(cur->left);
                }
                if (cur->right != nullptr)
                {
                    q.push(cur->right);
                }
                q.pop();
                cur->next = nullptr;
                last = cur;
            }
            last = nullptr;
        }
        return root;
    }
};
// @lc code=end
