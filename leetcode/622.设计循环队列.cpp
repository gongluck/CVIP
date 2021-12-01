/*
 * @lc app=leetcode.cn id=622 lang=cpp
 *
 * [622] 设计循环队列
 */

// @lc code=start
class MyCircularQueue
{
public:
    MyCircularQueue(int k)
    {
        data.resize(k);
    }

    bool enQueue(int value)
    {
        if (isFull())
        {
            return false;
        }
        used += 1;
        data[rear] = value;
        rear = (rear + 1 + data.size()) % data.size();
        return true;
    }

    bool deQueue()
    {
        if (isEmpty())
        {
            return false;
        }
        used -= 1;
        front = (front + 1 + data.size()) % data.size();
        return true;
    }

    int Front()
    {
        if (isEmpty())
        {
            return -1;
        }
        return data[(front + data.size()) % data.size()];
    }

    int Rear()
    {
        if (isEmpty())
        {
            return -1;
        }
        return data[(rear - 1 + data.size()) % data.size()];
    }

    bool isEmpty()
    {
        return used == 0;
    }

    bool isFull()
    {
        return used >= data.size();
    }

private:
    int used = 0;
    int front = 0;
    int rear = 0;
    std::vector<int> data;
};

/**
 * Your MyCircularQueue object will be instantiated and called as such:
 * MyCircularQueue* obj = new MyCircularQueue(k);
 * bool param_1 = obj->enQueue(value);
 * bool param_2 = obj->deQueue();
 * int param_3 = obj->Front();
 * int param_4 = obj->Rear();
 * bool param_5 = obj->isEmpty();
 * bool param_6 = obj->isFull();
 */
// @lc code=end
