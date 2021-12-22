/*
 * @lc app=leetcode.cn id=225 lang=cpp
 *
 * [225] 用队列实现栈
 */

// @lc code=start
class MyStack
{
private:
    std::queue<int> que1;
    std::queue<int> que2;

public:
    MyStack()
    {
    }

    void push(int x)
    {
        que1.push(x);
    }

    int pop()
    {
        // 1
        //  while (que1.size() > 1)
        //  {
        //      que2.push(que1.front());
        //      que1.pop();
        //  }
        //  auto res = que1.front();
        //  que1.pop();
        //  que1 = que2;
        //  while (!que2.empty())
        //  {
        //      que2.pop();
        //  }
        //  return res;

        // 2
        int size = que1.size();
        size--;
        while (size--)
        { // 将队列头部的元素（除了最后一个元素外） 重新添加到队列尾部
            que1.push(que1.front());
            que1.pop();
        }
        auto res = que1.front();
        que1.pop();
        return res;
    }

    int top()
    {
        return que1.back();
    }

    bool empty()
    {
        return que1.size() == 0;
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
