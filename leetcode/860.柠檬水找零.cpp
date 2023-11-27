/*
 * @lc app=leetcode.cn id=860 lang=cpp
 *
 * [860] 柠檬水找零
 */

// @lc code=start
class Solution
{
public:
    bool lemonadeChange(vector<int> &bills)
    {
        int fives = 0;
        int tens = 0;

        for (int i = 0; i < bills.size(); ++i)
        {
            switch (bills[i])
            {
            case 20:
                if (tens > 0 && fives > 0)
                {
                    --tens;
                    --fives;
                }
                else if (fives >= 3)
                {
                    fives -= 3;
                }
                else
                {
                    return false;
                }
                break;
            case 10:
                if (fives > 0)
                {
                    --fives;
                    ++tens;
                }
                else
                {
                    return false;
                }
                break;
            case 5:
                ++fives;
                break;
            }
        }

        return true;
    }
};
// @lc code=end
