/*
 * @lc app=leetcode.cn id=146 lang=cpp
 *
 * [146] LRU 缓存
 */

// @lc code=start
class LRUCache
{
    typedef struct NODE
    {
        int key = 0;
        int value = 0;
        NODE *prev = nullptr;
        NODE *next = nullptr;
    } Node;

public:
    LRUCache(int capacity) : cap_(capacity)
    {
        dummy_.prev = &dummy_;
        dummy_.next = &dummy_;
    }

    void remove(Node *node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void push_front(Node *node)
    {
        node->prev = &dummy_;
        node->next = dummy_.next;
        dummy_.next->prev = node;
        dummy_.next = node;
    }

    Node *get_node(int key)
    {
        auto it = nodes_.find(key);
        if (it == nodes_.end())
        {
            return nullptr;
        }
        auto node = it->second;
        remove(node);
        push_front(node);
        return node;
    }

    int get(int key)
    {
        auto node = get_node(key);
        return node == nullptr ? -1 : node->value;
    }

    void put(int key, int value)
    {
        auto node = get_node(key);
        if (node != nullptr)
        {
            node->value = value;
            return;
        }
        node = new Node;
        node->key = key;
        node->value = value;
        nodes_[key] = node;
        push_front(node);
        if (nodes_.size() > cap_)
        {
            node = dummy_.prev;
            nodes_.erase(node->key);
            remove(node);
            delete node;
        }
    }

private:
    int cap_ = 0;
    Node dummy_;
    std::unordered_map<int, Node *> nodes_;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
// @lc code=end
