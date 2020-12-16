#include <unistd.h>
#include <iostream>
#include <vector>
#include <list>
using namespace std;

#if defined(__APPLE__)
#include <AvailabilityMacros.h>
#include <sys/time.h>
#include <mach/task.h>
#include <mach/mach.h>
#else
#include <time.h>
#endif

class CTimerNode
{
public:
    CTimerNode(int fd) : id(fd), ref(0) {}

    void Offline()
    {
        this->ref = 0;
    }

    bool TryKill()
    {
        if (this->ref == 0)
            return true;
        DecrRef();
        if (this->ref == 0)
        {
            cout << id << " is killed down" << endl;
            return true;
        }
        cout << id << " ref is " << ref << endl;
        return false;
    }

    void IncrRef()
    {
        this->ref++;
    }

protected:
    void DecrRef()
    {
        this->ref--;
    }

private:
    int ref;
    int id;
};

const int TW_SIZE = 16;
const int EXPIRE = 10;
const int TW_MASK = TW_SIZE - 1;
static size_t iRealTick = 0;
typedef list<CTimerNode *> TimeList;
typedef TimeList::iterator TimeListIter;
typedef vector<TimeList> TimeWheel;

void AddTimeout(TimeWheel &tw, CTimerNode *p)
{
    if (p)
    {
        p->IncrRef();
        TimeList &le = tw[(iRealTick + EXPIRE) & TW_MASK];
        le.push_back(p);
    }
}

// 用来表示delay时间后调用 AddTimeout
void AddTimeoutDelay(TimeWheel &tw, CTimerNode *p, size_t delay)
{
    if (p)
    {
        p->IncrRef();
        TimeList &le = tw[(iRealTick + EXPIRE + delay) & TW_MASK];
        le.push_back(p);
    }
}

void TimerShift(TimeWheel &tw)
{
    size_t tick = iRealTick;
    iRealTick++;
    TimeList &le = tw[tick & TW_MASK];
    TimeListIter iter = le.begin();
    for (; iter != le.end(); iter++)
    {
        CTimerNode *p = *iter;
        if (p && p->TryKill())
        {
            delete p;
        }
    }
    le.clear();
}

static time_t
current_time()
{
    time_t t;
#if !defined(__APPLE__) || defined(AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER)
    struct timespec ti;
    clock_gettime(CLOCK_MONOTONIC, &ti);
    t = (time_t)ti.tv_sec;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    t = (time_t)tv.tv_sec;
#endif
    return t;
}

int main()
{
    TimeWheel tw(TW_SIZE);

    CTimerNode *p = new CTimerNode(10001);

    AddTimeout(tw, p);

    AddTimeoutDelay(tw, p, 5);

    time_t start = current_time();
    for (;;)
    {
        time_t now = current_time();
        if (now - start > 0)
        {
            for (int i = 0; i < now - start; i++)
                TimerShift(tw);
            start = now;
            cout << "check timer shift " << iRealTick << endl;
        }
        usleep(2500);
    }

    return 0;
}
