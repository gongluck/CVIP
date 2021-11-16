/*
 * @lc app=leetcode.cn id=22 lang=cpp
 *
 * [22] 括号生成
 */

// @lc code=start
class Solution
{
public:
    vector<string> res;
    void dp(int left, int right, string &track)
    {
        if (left < 0 || right < 0 || left > right)
        {
            return;
        }
        if (left == 0 && right == 0)
        {
            res.push_back(track);
        }

        track.push_back('(');
        dp(left - 1, right, track);
        track.pop_back();
        track.push_back(')');
        dp(left, right - 1, track);
        track.pop_back();
    }
    vector<string> generateParenthesis(int n)
    {
        std::string track;
        dp(n, n, track);
        return res;
    }
};
// @lc code=end
