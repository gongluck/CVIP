/*
 * @lc app=leetcode.cn id=2081 lang=cpp
 *
 * [2081] k 镜像数字的和
 */

// @lc code=start

// 获取下一个对称的十进制数
long long nextInt(long long num)
{
    string s = to_string(num);
    int width = s.size();
    for (int i = width / 2; i >= 0; i--)
    {
        if (s[i] != '9')
        {
            s[i]++;
            if (width - 1 - i != i)
            {
                s[width - 1 - i]++;
            }
            for (int j = i + 1; j <= width / 2; j++)
            {
                s[j] = '0';
                s[width - 1 - j] = '0';
            }
            return stoll(s);
        }
    }
    long long ans = 1;
    for (int i = 0; i < width; i++)
    {
        ans *= 10;
    }
    ans += 1;
    return ans;
}

// 判断一个字符串是否是对称的
bool isGood(string &s)
{
    int n = s.size();
    for (int i = 0; i < n / 2; i++)
    {
        if (s[i] != s[n - 1 - i])
        {
            return false;
        }
    }
    return true;
}

// 将十进制数转换为k进制字符串
string tokstring(long long num, int k)
{
    string ans = "";
    while (num != 0)
    {
        ans += char(num % k + '0');
        num /= k;
    }
    return ans;
}

class Solution
{
public:
    long long kMirror(int k, int n)
    {
        long long ans = 0, num = 0;
        while (n != 0)
        {
            num = nextInt(num);
            string s = tokstring(num, k);
            if (isGood(s))
            {
                ans += num;
                n--;
            }
        }
        return ans;
    }
};
// @lc code=end
