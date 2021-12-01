/*
 * @lc app=leetcode.cn id=400 lang=cpp
 *
 * [400] 第 N 位数字
 */

// @lc code=start
class Solution
{
public:
    int findNthDigit(int n)
    {
        long long N = n;
        int digits = 1;       // 位数
        long long border = 9; // digits位数一共产生多少位数字

        while (N)
        {
            if (N <= border * digits)
                break;            // 循环退出时即找到了当前数字的位数
            N -= border * digits; // N 为扣掉所有digits位数字（比如2位数）产生的数字之后还剩多少个数字
            digits++;
            border *= 10;
        }
        // 循环结束后 剩余的N都是由 digits位数 贡献的
        // 每个 digits位数 都产生了 digits个数字； 因而我们求除求余就可以知道n对应的是第几个digits位数
        int offset = (N - 1) / digits;
        int begin = 1; // 用于计算最小的 digits位数
        for (int i = 0; i < digits - 1; i++)
        {
            begin *= 10;
        }
        int mod = (N - 1) % digits;
        string target = to_string(offset + begin);
        int ans = target[mod] - '0';

        return ans;
    }
};
// @lc code=end
