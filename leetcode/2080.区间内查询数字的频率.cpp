/*
 * @lc app=leetcode.cn id=2080 lang=cpp
 *
 * [2080] 区间内查询数字的频率
 */

// @lc code=start
class RangeFreqQuery
{
public:
    RangeFreqQuery(vector<int> &arr) //: arr_(arr)
    {
        for (int i = 0; i < arr.size(); ++i)
        {
            hash_[arr[i]].push_back(i);
        }
    }

    int query(int left, int right, int value)
    {
        //二分查找！
        auto l = lower_bound(hash_[value].begin(), hash_[value].end(), left);
        auto r = upper_bound(l, hash_[value].end(), right);
        return r - l;
    }

private:
    //std::vector<int> arr_;
    std::unordered_map<int, std::vector<int>> hash_;
};

/**
 * Your RangeFreqQuery object will be instantiated and called as such:
 * RangeFreqQuery* obj = new RangeFreqQuery(arr);
 * int param_1 = obj->query(left,right,value);
 */
// @lc code=end
