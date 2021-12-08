/*
 * @lc app=leetcode.cn id=2094 lang=cpp
 *
 * [2094] 找出 3 位偶数
 */

// @lc code=start
class Solution
{
public:
    vector<int> findEvenNumbers(vector<int> &digits)
    {
        std::unordered_map<int, int> nums;
        for (const auto &i : digits)
        {
            nums[i]++;
        }

        std::vector<int> res;
        for (int i = 100; i < 999; i += 2)
        {
            //std::cout << "i : " << i << std::endl;
            int n = i;
            int count[10] = {0};
            while (n > 0)
            {
                //std::cout << "n : " << n << std::endl;
                int c = n % 10;
                //std::cout << "c : " << c << std::endl;
                ++count[c];
                if (count[c] > nums[c])
                {
                    //std::cout << c << "++" << count[c] << "++" << nums[c] << std::endl;
                    break;
                }

                n /= 10;
            }
            //std::cout << i << "--" << n << std::endl;
            if (n <= 0)
            {
                res.push_back(i);
            }
        }

        return res;
    }
};
// @lc code=end
