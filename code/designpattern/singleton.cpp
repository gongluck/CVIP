/*单例类
* (1) 单例类保证全局只有一个唯一的实例对象。
* (2) 单例类保证只有唯一的接口获取这唯一实例。
*/
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

#define SINGLETON_INDEX 6 // 开关，不同模式的开关

/*
 * 补充：
 * =default: 用于显式要求编译器提供合成版本的四大函数(构造、拷贝、析构、赋值)
 * =delete: 用于定义删除函数，在旧标准下，我们如果希望阻止拷贝可以通过显式声明拷贝构造函数和拷贝赋值函数为private，但新标准下允许我们定义删除函数
 */

// 反汇编举例 objdump -S -d 4-singleton-c++11 > 4-singleton-c++11.txt
// 直接汇编：g++ -S -o main2-2.s main2.cpp -std=c++11

//1、原始懒汉式单例模式 懒汉式单例就是需要使用这个单例对象的时候才去创建这个单例对象。
#if SINGLETON_INDEX == 1
class Singleton
{
private:
    static Singleton *m_singleton;
    Singleton() = default;                             // 自动生成默认构造函数
    Singleton(const Singleton &s) = delete;            // 禁用拷贝构造函数
    Singleton &operator=(const Singleton &s) = delete; // 禁用拷贝赋值操作符

    class GarbageCollector
    {
    public:
        ~GarbageCollector()
        {
            cout << "~GarbageCollector\n";
            if (Singleton::m_singleton)
            {
                cout << "free m_singleton\n";
                delete Singleton::m_singleton;
                Singleton::m_singleton = nullptr;
            }
        }
    };
    static GarbageCollector m_gc;

public:
    static Singleton *getInstance()
    {
        if (Singleton::m_singleton == nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); //休眠，模拟创建实例的时间
            m_singleton = new Singleton();
        }
        return m_singleton;
    }
};
// 必须在类外初始化
Singleton *Singleton::m_singleton = nullptr;
Singleton::GarbageCollector Singleton::m_gc;
#elif SINGLETON_INDEX == 2
// 2 线程安全的懒汉式单例模式
//线程安全的懒汉式单例

class Singleton
{
private:
    static Singleton *m_singleton;
    static mutex m_mutex;
    Singleton() = default;
    Singleton(const Singleton &s) = delete;            // 禁用拷贝构造函数
    Singleton &operator=(const Singleton &s) = delete; // 禁用拷贝赋值操作符
    class GarbageCollector
    {
    public:
        ~GarbageCollector()
        {
            cout << "~GarbageCollector\n";
            if (Singleton::m_singleton)
            {
                cout << "free m_singleton\n";
                delete Singleton::m_singleton;
                Singleton::m_singleton = nullptr;
            }
        }
    };
    static GarbageCollector m_gc;

public:
    static Singleton *getInstance()
    {                   // 加锁的粒度大，效率较低， 对高并发的访问
        m_mutex.lock(); // 加锁，保证只有一个线程在访问下面的语句
        if (Singleton::m_singleton == nullptr)
        {
            //std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //休眠，模拟创建实例的时间
            m_singleton = new Singleton();
        }
        m_mutex.unlock(); //解锁
        return m_singleton;
    }
};
Singleton *Singleton::m_singleton = nullptr;
mutex Singleton::m_mutex;
Singleton::GarbageCollector Singleton::m_gc;
#elif SINGLETON_INDEX == 3
// 3、锁住初始化实例语句之后再次检查实例是否被创建
/* 双检查锁，但由于内存读写reorder不安全 因为C++创建对象时，会执行1、分配内存，2 调用构造，3 赋值操作三步操作，
然而现代CPU和编译器高并发下可能会进行乱序重排操作，因而创建对象new CSingleton的第2步可能会晚于第3步进行指令调用，
因而导致出现未定义的的行为。*/
class Singleton
{
private:
    static Singleton *m_singleton;
    static mutex m_mutex;
    Singleton() = default;
    Singleton(const Singleton &s) = default;
    Singleton &operator=(const Singleton &s) = default;
    class GarbageCollector
    {
    public:
        ~GarbageCollector()
        {
            cout << "~GarbageCollector\n";
            if (Singleton::m_singleton)
            {
                cout << "free m_singleton\n";
                delete Singleton::m_singleton;
                Singleton::m_singleton = nullptr;
            }
        }
    };
    static GarbageCollector m_gc;

public:
    void *getSingletonAddress()
    {
        return m_singleton;
    }
    static Singleton *getInstance()
    {
        if (Singleton::m_singleton == nullptr)
        {
            m_mutex.lock(); // 加锁，保证只有一个线程在访问线程内的代码
            if (Singleton::m_singleton == nullptr)
            {                                  //再次检查
                m_singleton = new Singleton(); // 对象的new不是原子操作 1、分配内存，2 调用构造，3 赋值操作，到第3步的时候才是m_singleton非空
                                               //  1、分配内存，2 赋值操作 3 调用构造，到第2步的时候才是m_singleton非空
            }
            m_mutex.unlock(); //解锁
        }
        return m_singleton;
    }
};
Singleton *Singleton::m_singleton = nullptr;
mutex Singleton::m_mutex;
Singleton::GarbageCollector Singleton::m_gc;
#elif SINGLETON_INDEX == 4
// 4、C++ 11版本之后的跨平台实现
class Singleton
{
private:
    static std::atomic<Singleton *> m_instance;
    static std::mutex m_mutex;
    Singleton() = default;
    Singleton(const Singleton &s) = default;
    Singleton &operator=(const Singleton &s) = default;
    class GarbageCollector
    {
    public:
        ~GarbageCollector()
        {
            cout << "~GarbageCollector\n";
            Singleton *tmp = m_instance.load(std::memory_order_relaxed);
            if (tmp)
            {
                cout << "free m_singleton: " << tmp << endl;
                delete tmp;
            }
        }
    };
    static GarbageCollector m_gc;

public:
    void *getSingletonAddress()
    {
        return m_instance;
    }
    static Singleton *getInstance()
    {
        Singleton *tmp = m_instance.load(std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_acquire); //获取内存fence
        if (tmp == nullptr)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            tmp = m_instance.load(std::memory_order_relaxed);
            if (tmp == nullptr)
            {
                tmp = new Singleton();                               // 1、分配内存，2 调用构造，3 赋值操作
                std::atomic_thread_fence(std::memory_order_release); //释放内存fence
                m_instance.store(tmp, std::memory_order_relaxed);
            }
        }
        return tmp;
    }
};
std::atomic<Singleton *> Singleton::m_instance;
std::mutex Singleton::m_mutex;
Singleton::GarbageCollector Singleton::m_gc;
#elif SINGLETON_INDEX == 5
// 懒汉式
class Singleton
{
private:
    //Singleton() = default;  // 自动生成默认构造函数
    Singleton()
    { // 构造函数会影响局部静态变量，不能用隐式的构造函数
        cout << "Singleton construct\n";
    }
    Singleton(const Singleton &s) = delete;            // 禁用拷贝构造函数
    Singleton &operator=(const Singleton &s) = delete; // 禁用拷贝赋值操作符
public:
    static Singleton *getInstance()
    {
        static Singleton s_singleton; // C++11线程安全，C++11之前不是线程安全  __cxa_guard_acquire 和 __cxa_guard_release
        return &s_singleton;
    }
};
#elif SINGLETON_INDEX == 6
// 饿汉式，在main函数运行前初始化，绝对安全
class Singleton
{
private:
    //Singleton() = default;   //自动生成默认构造函数
    Singleton()
    {
        cout << "Singleton construct\n";
    }
    Singleton(const Singleton &s) = delete;            // 禁用拷贝构造函数
    Singleton &operator=(const Singleton &s) = delete; // 禁用拷贝赋值操作符
    static Singleton m_singleton;

public:
    static Singleton *getInstance()
    {
        return &m_singleton;
    }
};
Singleton Singleton::m_singleton;
#endif

//// 定义一个互斥锁
static mutex s_m;

void print_address()
{
    // 获取实例
    Singleton *singleton = Singleton::getInstance();
    // 打印singleton地址
    s_m.lock(); // 锁住，保证只有一个线程在打印地址
    cout << singleton << endl;
    //cout<<"Address:" << singleton->getSingletonAddress() << endl; // 只有双检查锁
    s_m.unlock(); // 解锁
}

void test_Singleton()
{
    thread threads[10];
    // 创建10个线程
    for (auto &t : threads)
        t = thread(print_address);
    // 对每个线程调用join，主线程等待子线程完成运行
    for (auto &t : threads)
        t.join();
}

int main()
{
    //Singleton *s = new Singleton();
    cout << "main\n";

    test_Singleton();

    cout << "test finish! " << Singleton::getInstance() << endl;
    return 0;
}
