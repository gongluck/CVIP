/*
 * @lc app=leetcode.cn id=12 lang=cpp
 *
 * [12] 整数转罗马数字
 */

// @lc code=start
class Solution
{
public:
    string intToRoman(int num)
    {
        const std::pair<int, std::string> syms[] = {
            {1000, "M"},
            {900, "CM"},
            {500, "D"},
            {400, "CD"},
            {100, "C"},
            {90, "XC"},
            {50, "L"},
            {40, "XL"},
            {10, "X"},
            {9, "IX"},
            {5, "V"},
            {4, "IV"},
            {1, "I"},
        };
        std::string result;

        for (const auto &[value, symbol] : syms)
        {
            while (num >= value)
            {
                num -= value;
                result += symbol;
            }
        }
        return result;
    }
};
// @lc code=end
