/*
 * @lc app=leetcode.cn id=155 lang=cpp
 *
 * [155] 最小栈
 */

// @lc code=start
class MinStack
{
public:
    void push(int val)
    {
        data.push(val);
        if (!sort.empty() && sort.top() <= val)
        {
            sort.push(sort.top());
        }
        else
        {
            sort.push(val);
        }
    }

    void pop()
    {
        if (!data.empty())
        {
            data.pop();
            sort.pop();
        }
    }

    int top()
    {
        if (data.empty())
        {
            return -1;
        }
        else
        {
            return data.top();
        }
    }

    int getMin()
    {
        if (sort.empty())
        {
            return -1;
        }
        else
        {
            return sort.top();
        }
    }

private:
    std::stack<int> data;
    std::stack<int> sort;
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */
// @lc code=end
