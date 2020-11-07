// 简单变形示例——区别对待观察者
/*
1：范例需求
这是一个实际系统的简化需求：在一个水质监测系统中有这样一个功能，当水中的杂质为正常的时候，只是通知监测人员做记录；
当为轻度污染的时候，除了通知监测人员做记录外，还要通知预警人员，判断是否需要预警；当为中度或者高度污染的时候，
除了通知监测人员做记录外，还要通知预警人员，判断是否需要预警，同时还要通知监测部门领导做相应的处理。
*/
#include <iostream>
#include <list>
using namespace std;

class WaterQualitySubject;

// 观察者的接口
/**
 * 水质观察者接口定义
 */
class WatcherObserver
{
public:
    WatcherObserver() {}
    virtual ~WatcherObserver() {}
    /**
     * 被通知的方法
     * @param subject 传入被观察的目标对象
     */
    virtual void update(WaterQualitySubject *subject) = 0;

    // 和普通观察者模式， 增加了角色
    /**
     * 设置观察人员的职务
     * @param job 观察人员的职务
     */
    virtual void setJob(string job) = 0;

    /**
     * 获取观察人员的职务
     * @return 观察人员的职务
     */
    virtual string getJob() = 0;
};

/**
* 定义水质监测的目标对象
*/
class WaterQualitySubject
{
public:
    WaterQualitySubject() {}
    virtual ~WaterQualitySubject() {}
    /**
    * 注册观察者对象
     * @param observer 观察者对象
     */
    void attach(WatcherObserver *observer)
    {
        observers.push_back(observer);
    }

    /**
     * 删除观察者对象
     * @param observer 观察者对象
     */
    void detach(WatcherObserver *observer)
    {
        observers.remove(observer);
    }

    /**
     * 通知相应的观察者对象
     */
    virtual void notifyWatchers() = 0;

    /**
     * 获取水质污染的级别
     * @return 水质污染的级别
     */
    virtual int getPolluteLevel() = 0;

protected:
    /**
     * 用来保存注册的观察者对象
     */
    list<WatcherObserver *> observers;
};

/**
 * 具体的观察者实现
 */
class Watcher : public WatcherObserver
{
public:
    Watcher() {}
    virtual ~Watcher() {}
    string getJob()
    {
        return m_job;
    }

    void setJob(string job)
    {
        m_job = job;
    }

    virtual void update(WaterQualitySubject *subject)
    {
        //这里采用的是拉的方式
        cout << m_job << " 获取到通知，当前污染级别为：" << subject->getPolluteLevel() << endl;
    }

private:
    /**
     * 职务
     */
    string m_job;
};

/**
 * 具体的水质监测对象
 */
class WaterQuality : public WaterQualitySubject
{
public:
    WaterQuality() {}
    virtual ~WaterQuality() {}
    /**
     * 获取水质污染的级别
     * @return 水质污染的级别
     */
    int getPolluteLevel()
    {
        return m_polluteLevel;
    }

    /**
     * 当监测水质情况后，设置水质污染的级别
     * @param polluteLevel 水质污染的级别
     */
    virtual void setPolluteLevel(int polluteLevel)
    {
        m_polluteLevel = polluteLevel;
        //通知相应的观察者
        notifyWatchers();
    }

    /**
     * 通知相应的观察者对象
     */

    virtual void notifyWatchers()
    {
        //循环所有注册的观察者
        for (WatcherObserver *watcher : observers)
        {
            //开始根据污染级别判断是否需要通知，由这里总控
            if (m_polluteLevel >= 0)
            {
                //通知监测员做记录
                if (watcher->getJob().compare("监测人员") == 0)
                {
                    watcher->update(this);
                }
            }

            if (m_polluteLevel >= 1)
            {
                //通知预警人员
                if (watcher->getJob().compare("预警人员") == 0)
                {
                    watcher->update(this);
                }
            }

            if (m_polluteLevel >= 2)
            {
                //通知监测部门领导
                if (watcher->getJob().compare("监测部门领导") == 0)
                {
                    watcher->update(this);
                }
            }
        }
    }

private:
    /**
     * 污染的级别，0表示正常，1表示轻度污染，2表示中度污染，3表示高度污染
     */
    int m_polluteLevel = 0;
};

int main()
{
    //创建水质主题对象
    WaterQuality *subject = new WaterQuality();
    //创建几个观察者, 观察者分了不同角色
    WatcherObserver *watcher1 = new Watcher();
    watcher1->setJob("监测人员");
    WatcherObserver *watcher2 = new Watcher();
    watcher2->setJob("预警人员");
    WatcherObserver *watcher3 = new Watcher();
    watcher3->setJob("监测部门领导");

    //注册观察者
    subject->attach(watcher1);
    subject->attach(watcher2);
    subject->attach(watcher3);

    //填写水质报告
    cout << "当水质为正常的时候------------------〉" << endl;
    subject->setPolluteLevel(0);

    cout << "\n当水质为轻度污染的时候---------------〉" << endl;
    subject->setPolluteLevel(1);

    cout << "\n当水质为中度污染的时候---------------〉" << endl;
    subject->setPolluteLevel(2);

    // 释放观察者
    subject->detach(watcher1);
    subject->detach(watcher2);
    subject->detach(watcher3);

    delete watcher1;
    delete watcher2;
    delete watcher3;

    delete subject;

    return 0;
}
