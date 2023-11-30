/*
 * @lc app=leetcode.cn id=22 lang=cpp
 *
 * [22] 括号生成
 */

// @lc code=start
class Solution
{
public:
    void generate(std::vector<std::string> &result, int index, int n, std::string &steps, int leftused, int rightused)
    {
        if (leftused > n || rightused > leftused)
        {
            return;
        }

        if (rightused == n)
        {
            result.push_back(steps);
            return;
        }

        steps[index] = '(';
        generate(result, index + 1, n, steps, leftused + 1, rightused);

        steps[index] = ')';
        generate(result, index + 1, n, steps, leftused, rightused + 1);
    }

    vector<string> generateParenthesis(int n)
    {
        std::vector<std::string> result;
        std::string steps(2 * n, '.');
        steps[0] = '(';
        steps[2 * n - 1] = ')';

        generate(result, 1, n, steps, 1, 0);

        return result;
    }
};
// @lc code=end
