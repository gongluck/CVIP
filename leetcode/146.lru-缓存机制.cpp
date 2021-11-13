/*
 * @lc app=leetcode.cn id=146 lang=cpp
 *
 * [146] LRU 缓存机制
 */

// @lc code=start
class LRUCache
{
private:
    int cap;
    std::list<std::pair<int, int>> NodeList;
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> NodeMap;

public:
    LRUCache(int capacity)
    {
        cap = capacity;
    }

    int get(int key)
    {
        auto it = NodeMap.find(key);
        if (it == NodeMap.end())
        {
            return -1;
        }

        auto kv = (*it->second);
        NodeList.erase(it->second);
        NodeList.push_front(kv);
        it->second = NodeList.begin();

        return kv.second;
    }

    void put(int key, int value)
    {
        auto it = NodeMap.find(key);
        if (it != NodeMap.end())
        {
            NodeList.erase(it->second);
        }
        else if (NodeMap.size() >= cap)
        {
            auto itt = NodeList.rbegin();
            NodeMap.erase(itt->first);
            NodeList.pop_back();
        }
        NodeList.push_front({key, value});
        NodeMap[key] = NodeList.begin();
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
// @lc code=end
