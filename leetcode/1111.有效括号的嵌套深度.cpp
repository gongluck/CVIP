/*
 * @lc app=leetcode.cn id=1111 lang=cpp
 *
 * [1111] 有效括号的嵌套深度
 */

// @lc code=start
class Solution
{
public:
    vector<int> maxDepthAfterSplit(string seq)
    {
        std::vector<int> res(seq.size(), 0);
        int count = 0;
        for (int i = 0; i < seq.size(); ++i)
        {
            if (seq[i] == '(')
            {
                res[i] = count % 2;
                ++count;
            }
            else if (seq[i] == ')')
            {
                --count;
                res[i] = count % 2;
            }
        }
        return res;
    }
};
// @lc code=end
