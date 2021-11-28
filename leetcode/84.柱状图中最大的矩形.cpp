/*
 * @lc app=leetcode.cn id=84 lang=cpp
 *
 * [84] 柱状图中最大的矩形
 */

// @lc code=start
class Solution
{
public:
    int largestRectangleArea(vector<int> &heights)
    {
        stack<int> s;
        int leftborder;
        int rightborder;
        int area;
        int ans = heights[0];
        heights.push_back(0); //末尾添加高度为0的柱子,这样可以计算最后柱子他的最大面积
        s.push(0);

        for (int i = 0; i + 1 < heights.size(); i++)
        {
            while (!s.empty() && heights[i + 1] < heights[s.top()]) //其后面柱子i+1比当前栈顶元素矮,说明找到栈顶元素右边界
            {
                rightborder = i + 1;
                int curval = heights[s.top()]; //一点细节要先记录再pop,curval的值是当前栈顶的值,举例栈中现在为1,5,6,遇见2后,6出栈算其面积,遇见2时相当于5也找到了右边界,栈顶元素是5,要计算以5为矩形高的面积
                s.pop();
                if (!s.empty()) //有左边界
                    leftborder = s.top();
                else
                    leftborder = -1;                            //若没有左边界,说明其数组中左侧都是比当前元素矮的,如2,2,2,1,0 计算以!为高的矩形,但是栈中只有1,不能说左边界就是自身,其左边界为第一个元素
                area = (rightborder - leftborder - 1) * curval; //注意乘高度要用curval而不是s.top,有可能栈为空,对于求面积这个式子的-1,手动模拟一下就明白了
                ans = max(ans, area);
            }
            //i+1不比i矮,将i+1入栈继续寻找i的右边界,相当于也要找i+1的右边界
            s.push(i + 1);
        }
        return ans;
    }
};
// @lc code=end
