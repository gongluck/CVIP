/*
 * @lc app=leetcode.cn id=380 lang=cpp
 *
 * [380] O(1) 时间插入、删除和获取随机元素
 */

// @lc code=start
class RandomizedSet
{
public:
    RandomizedSet()
    {
        srand((unsigned)time(nullptr));
    }

    bool insert(int val)
    {
        if (maps.count(val) != 0)
        {
            return false;
        }
        int index = nums.size();
        nums.push_back(val);
        maps[val] = index;
        return true;
    }

    bool remove(int val)
    {
        if (maps.count(val) == 0)
        {
            return false;
        }
        int index = maps[val];
        // std::swap(nums[nums.size() - 1], nums[index]);
        // maps[nums[index]] = index;
        int last = nums.back();
        nums[index] = last;
        maps[last] = index;
        nums.pop_back();
        maps.erase(val);
        return true;
    }

    int getRandom()
    {
        int randomIndex = rand() % nums.size();
        return nums[randomIndex];
    }

private:
    std::vector<int> nums;
    std::unordered_map<int, int> maps;
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */
// @lc code=end
