/*
 * @lc app=leetcode.cn id=225 lang=cpp
 *
 * [225] 用队列实现栈
 */

// @lc code=start
class MyStack
{
private:
    std::queue<int> que;

public:
    MyStack()
    {
    }

    void push(int x)
    {
        que.push(x);
    }

    int pop()
    {
        int n = que.size();
        while (n-- > 1)
        {
            push(que.front());
            que.pop();
        }

        int val = que.front();
        que.pop();
        return val;
    }

    int top()
    {
        int val = pop();
        push(val);
        return val;
    }

    bool empty()
    {
        return que.empty();
    }
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */
// @lc code=end
