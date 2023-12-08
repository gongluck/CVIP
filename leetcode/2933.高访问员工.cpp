/*
 * @lc app=leetcode.cn id=2933 lang=cpp
 *
 * [2933] 高访问员工
 */

// @lc code=start
class Solution
{
public:
    vector<string> findHighAccessEmployees(vector<vector<string>> &access_times)
    {
        std::vector<string> result;
        std::unordered_map<std::string, std::vector<int>> maps;

        for (const auto &access : access_times)
        {
            maps[access[0]].push_back(std::stoi(access[1].substr(0, 2)) * 60 + std::stoi(access[1].substr(2)));
        }
        for (auto &person : maps)
        {
            std::sort(person.second.begin(), person.second.end());
            for (int i = 1; i < person.second.size() - 1; ++i)
            {
                if (person.second[i + 1] < person.second[i - 1] + 60)
                {
                    result.push_back(person.first);
                    break;
                }
            }
        }

        return result;
    }
};
// @lc code=end
