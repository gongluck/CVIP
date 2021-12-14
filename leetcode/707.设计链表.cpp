/*
 * @lc app=leetcode.cn id=707 lang=cpp
 *
 * [707] 设计链表
 */

// @lc code=start
class MyLinkedList
{
private:
    struct Node
    {
        int val = 0;
        Node *next = nullptr;
    };
    Node dummy;
    int size_ = 0;

public:
    MyLinkedList()
    {
    }

    int get(int index)
    {
        if (index < size_)
        {
            auto p = dummy.next;
            while (index-- > 0)
            {
                p = p->next;
            }
            return p->val;
        }
        else
        {
            return -1;
        }
    }

    void addAtHead(int val)
    {
        auto head = new Node;
        head->val = val;
        head->next = dummy.next;
        dummy.next = head;
        ++size_;
    }

    void addAtTail(int val)
    {
        auto tail = &dummy;
        while (tail && tail->next)
        {
            tail = tail->next;
        }
        auto p = new Node;
        p->val = val;
        tail->next = p;
        ++size_;
    }

    void addAtIndex(int index, int val)
    {
        if (index > size_)
        {
            return;
        }
        if (index == size_)
        {
            addAtTail(val);
            return;
        }
        auto pre = &dummy;
        while (index-- > 0)
        {
            pre = pre->next;
        }
        auto p = new Node;
        p->val = val;
        p->next = pre->next;
        pre->next = p;
        ++size_;
    }

    void deleteAtIndex(int index)
    {
        if (index >= size_)
        {
            return;
        }
        auto pre = &dummy;
        while (index-- > 0)
        {
            pre = pre->next;
        }
        auto p = pre->next;
        pre->next = pre->next->next;
        delete p;
        --size_;
    }
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
