/*
 * @lc app=leetcode.cn id=394 lang=cpp
 *
 * [394] 字符串解码
 */

// @lc code=start
class Solution
{
public:
    string decodeString(string s)
    {
        std::stack<int> nums;
        std::stack<std::string> strs;
        std::string result;
        int num = 0;
        int n = s.size();

        for (int i = 0; i < n; ++i)
        {
            if (s[i] >= '0' && s[i] <= '9')
            {
                num = num * 10 + s[i] - '0';
            }
            else if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
            {
                result += s[i];
            }
            else if (s[i] == '[')
            {
                nums.push(num);
                num = 0;
                strs.push(result);
                result.clear();
            }
            else if (s[i] == ']')
            {
                auto times = nums.top();
                nums.pop();
                auto str = strs.top();
                strs.pop();
                for (int j = 0; j < times; ++j)
                {
                    str += result;
                }
                result = str;
            }
        }

        return result;
    }
};
// @lc code=end
