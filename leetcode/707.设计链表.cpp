/*
 * @lc app=leetcode.cn id=707 lang=cpp
 *
 * [707] 设计链表
 */

// @lc code=start
class MyLinkedList
{
public:
    MyLinkedList()
    {
    }

    int get(int index)
    {
        if (index >= size_)
        {
            return -1;
        }

        Node *cur = dummy_.next;
        while (index--)
        {
            cur = cur->next;
        }

        return cur->val;
    }

    void addAtHead(int val)
    {
        Node *node = new (Node);
        node->val = val;
        node->next = dummy_.next;
        dummy_.next = node;
        ++size_;

        return;
    }

    void addAtTail(int val)
    {
        Node *cur = &dummy_;
        while (cur->next != nullptr)
        {
            cur = cur->next;
        }

        Node *node = new (Node);
        node->val = val;
        node->next = nullptr;
        cur->next = node;
        ++size_;

        return;
    }

    void addAtIndex(int index, int val)
    {
        if (index > size_)
        {
            return;
        }

        Node *cur = &dummy_;
        while (index--)
        {
            cur = cur->next;
        }

        Node *node = new (Node);
        node->val = val;
        node->next = cur->next;
        cur->next = node;
        ++size_;

        return;
    }

    void deleteAtIndex(int index)
    {
        if (index >= size_)
        {
            return;
        }

        Node *cur = &dummy_;
        while (index--)
        {
            cur = cur->next;
        }

        Node *d = cur->next;
        cur->next = cur->next->next;
        delete d;
        --size_;

        return;
    }

private:
    typedef struct NODE
    {
        int val;
        struct NODE *next = nullptr;
    } Node;
    Node dummy_;
    int size_ = 0;
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */
// @lc code=end
