/*
 * @lc app=leetcode.cn id=6 lang=cpp
 *
 * [6] N 字形变换
 */

// @lc code=start
class Solution
{
public:
    string convert(string s, int numRows)
    {
        if (numRows <= 1)
        {
            return s;
        }

        int n = s.size();
        std::string result(n, '\0');
        int index = 0;                     // result下标
        int firststep = 2 * (numRows - 1); // 纵向数列的增加下标值

        for (int row = 0; row < numRows; ++row) // 处理结果的每行
        {
            int beginindex = row;                 // 每行第一个数的开始下标
            int secondstep = firststep - 2 * row; // 每行第二步的步长

            for (int i = 0;; ++i)
            {
                int idx = beginindex + i * firststep;
                if (idx >= n)
                {
                    break;
                }
                result[index++] += s[idx];

                if (secondstep != firststep && secondstep != 0) // 第二步不为0并且不等于第一步的步长才使用
                {
                    int idx = beginindex + i * firststep + secondstep;
                    if (idx >= n)
                    {
                        break;
                    }
                    result[index++] += s[idx];
                }
            }
        }

        return result;
    }
};
// @lc code=end
