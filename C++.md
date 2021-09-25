# C++

- [C++](#c)
	- [基础](#基础)
		- [编译过程](#编译过程)
		- [链接](#链接)
			- [二者的优缺点](#二者的优缺点)
		- [内存管理](#内存管理)
		- [堆和栈的区别](#堆和栈的区别)
		- [全局变量、局部变量、静态全局变量、静态局部变量的区别](#全局变量局部变量静态全局变量静态局部变量的区别)
		- [限制类的对象只能在堆上创建和限制对象只能在栈上创建](#限制类的对象只能在堆上创建和限制对象只能在栈上创建)
		- [内存对齐](#内存对齐)
		- [类大小的计算](#类大小的计算)
		- [内存泄漏](#内存泄漏)
		- [智能指针](#智能指针)
		- [``C++ 11`` 新特性](#c-11-新特性)
			- [``auto`` 类型推导](#auto-类型推导)
			- [``decltype`` 类型推导](#decltype-类型推导)
			- [``lambda`` 表达式](#lambda-表达式)
			- [范围 ``for`` 语句](#范围-for-语句)
			- [右值引用](#右值引用)
			- [标准库 ``move()`` 函数](#标准库-move-函数)
			- [智能指针](#智能指针-1)
			- [``delete`` 函数和 ``default`` 函数](#delete-函数和-default-函数)
		- [面向对象](#面向对象)
		- [重载、重写、隐藏的区别](#重载重写隐藏的区别)
		- [`i++`与`++i`](#i与i)
		- [不用中间变量，交换`a`和`b`的值](#不用中间变量交换a和b的值)
		- [`main`函数执行完之后还会执行其他语句吗？](#main函数执行完之后还会执行其他语句吗)
		- [宏参数的连接](#宏参数的连接)
		- [`const`和`#define`的特点和区别](#const和define的特点和区别)
		- [`const`的作用](#const的作用)
		- [`static`的作用](#static的作用)
		- [字节对齐](#字节对齐)
		- [`C++`中4种指针转化运算符](#c中4种指针转化运算符)
	- [内联函数](#内联函数)
		- [宏函数和内联函数](#宏函数和内联函数)
	- [友元](#友元)
	- [构造函数](#构造函数)
		- [构造函数放在`private`区](#构造函数放在private区)
	- [析构函数](#析构函数)
	- [虚函数](#虚函数)
		- [虚函数的实现](#虚函数的实现)
		- [虚析构函数](#虚析构函数)
		- [构造函数中调用虚函数](#构造函数中调用虚函数)
	- [模板](#模板)

## 基础

### 编译过程

编译（编译预处理、编译、优化），汇编，链接。

![编译过程](https://github.com/gongluck/images/blob/main/编译过程.png)

- 编译预处理：处理以 `#` 开头的指令；
- 编译、优化：将源码 `.cpp` 文件翻译成 `.s` 汇编代码；
- 汇编：将汇编代码 `.s` 翻译成机器指令 `.o` 文件；
- 链接：汇编程序生成的目标文件，即 `.o` 文件，并不会立即执行，因为可能会出现：`.cpp` 文件中的函数引用了另一个 `.cpp` 文件中定义的符号或者调用了某个库文件中的函数。那链接的目的就是将这些文件对应的目标文件连接成一个整体，从而生成可执行的程序 `.exe` 文件。

### 链接

- 静态链接：代码从其所在的静态链接库中拷贝到最终的可执行程序中，在该程序被执行时，这些代码会被装入到该进程的虚拟地址空间中。
- 动态链接：代码被放到动态链接库或共享对象的某个目标文件中，链接程序只是在最终的可执行程序中记录了共享对象的名字等一些信息。在程序执行时，动态链接库的全部内容会被映射到运行时相应进行的虚拟地址的空间。

#### 二者的优缺点

- 静态链接：浪费空间，每个可执行程序都会有目标文件的一个副本，这样如果目标文件进行了更新操作，就需要重新进行编译链接生成可执行程序（更新困难）；优点就是执行的时候运行速度快，因为可执行程序具备了程序运行的所有内容。
- 动态链接：节省内存、更新方便，但是动态链接是在程序运行时，每次执行都需要链接，相比静态链接会有一定的性能损失。

### 内存管理

`C++` 内存分区：栈、堆、全局/静态存储区、常量存储区、代码区。

- 栈：存放函数的局部变量、函数参数、返回地址等，由编译器自动分配和释放。
- 堆：动态申请的内存空间，就是由 malloc 分配的内存块，由程序员控制它的分配和释放，如果程序执行结束还没有释放，操作系统会自动回收。
- 全局区/静态存储区（`.bss` 段和 `.data` 段）：存放全局变量和静态变量，程序运行结束操作系统自动释放，在 `C` 语言中，未初始化的放在 `.bss` 段中，初始化的放在 `.data` 段中，`C++` 中不再区分了。
- 常量存储区（`.data` 段）：存放的是常量，不允许修改，程序运行结束自动释放。
- 代码区（`.text` 段）：存放代码，不允许修改，但可以执行。编译后的二进制文件存放在这里。

从操作系统的本身来讲，以上存储区在内存中的分布是如下形式(从低地址到高地址)：
`.text` 段 --> `.data` 段 --> `.bss` 段 --> 堆 --> `unused` --> 栈 --> `env`

### 堆和栈的区别

- 申请方式：栈是系统自动分配，堆是程序员主动申请。
- 申请后系统响应：分配栈空间，如果剩余空间大于申请空间则分配成功，否则分配失败栈溢出；申请堆空间，堆在内存中呈现的方式类似于链表（记录空闲地址空间的链表），在链表上寻找第一个大于申请空间的节点分配给程序，将该节点从链表中删除，大多数系统中该块空间的首地址存放的是本次分配空间的大小，便于释放，将该块空间上的剩余空间再次连接在空闲链表上。
栈在内存中是连续的一块空间（向低地址扩展）最大容量是系统预定好的，堆在内存中的空间（向高地址扩展）是不连续的。
- 申请效率：栈是有系统自动分配，申请效率高，但程序员无法控制；堆是由程序员主动申请，效率低，使用起来方便但是容易产生碎片。
- 存放的内容：栈中存放的是局部变量，函数的参数；堆中存放的内容由程序员控制。

### 全局变量、局部变量、静态全局变量、静态局部变量的区别

`C++` 变量根据定义的位置的不同的生命周期，具有不同的作用域，作用域可分为 `6` 种：全局作用域，局部作用域，语句作用域，类作用域，命名空间作用域和文件作用域。

- 全局变量：具有全局作用域。全局变量只需在一个源文件中定义，就可以作用于所有的源文件。当然，其他不包含全局变量定义的源文件需要用 `extern` 关键字再次声明这个全局变量。
- 静态全局变量：具有文件作用域。它与全局变量的区别在于如果程序包含多个文件的话，它作用于定义它的文件里，不能作用到其它文件里，即被 `static` 关键字修饰过的变量具有文件作用域。这样即使两个不同的源文件都定义了相同名字的静态全局变量，它们也是不同的变量。
- 局部变量：具有局部作用域。它是自动对象（`auto`），在程序运行期间不是一直存在，而是只在函数执行期间存在，函数的一次调用执行结束后，变量被撤销，其所占用的内存也被收回。
- 静态局部变量：具有局部作用域。它只被初始化一次，自从第一次被初始化直到程序运行结束都一直存在，它和全局变量的区别在于全局变量对所有的函数都是可见的，而静态局部变量只对定义自己的函数体始终可见。

从分配内存空间看，变量可以分为：

- 静态存储区：全局变量，静态局部变量，静态全局变量；
- 栈：局部变量。

静态变量和栈变量（存储在栈中的变量）、堆变量（存储在堆中的变量）的区别：

- 静态变量会被放在程序的静态数据存储区（`.data` 段）中（静态变量会自动初始化），这样可以在下一次调用的时候还可以保持原来的赋值。
- 而栈变量或堆变量不能保证在下一次调用的时候依然保持原来的值。

静态变量和全局变量的区别：

- 静态变量用 `static` 告知编译器，自己仅仅在变量的作用范围内可见。

### 限制类的对象只能在堆上创建和限制对象只能在栈上创建

`C++` 中的类的对象的建立分为两种：静态建立、动态建立。

- 静态建立：由编译器为对象在栈空间上分配内存，直接调用类的构造函数创建对象。例如：`A a;`
- 动态建立：使用 `new` 关键字在堆空间上创建对象，底层首先调用 `operator new()` 函数，在堆空间上寻找合适的内存并分配；然后，调用类的构造函数创建对象。例如：`A *p = new A();`

限制对象只能建立在堆上：

- 最直观的思想：避免直接调用类的构造函数，因为对象静态建立时，会调用类的构造函数创建对象。但是直接将类的构造函数设为私有并不可行，因为当构造函数设置为私有后，不能在类的外部调用构造函数来构造对象，只能用 `new` 来建立对象。但是由于 `new` 创建对象时，底层也会调用类的构造函数，将构造函数设置为私有后，那就无法在类的外部使用 `new` 创建对象了。因此，这种方法不可行。

解决方法 1：

- 将析构函数设置为私有。原因：静态对象建立在栈上，是由编译器分配和释放内存空间，编译器为对象分配内存空间时，会对类的非静态函数进行检查，即编译器会检查析构函数的访问性。当析构函数设为私有时，编译器创建的对象就无法通过访问析构函数来释放对象的内存空间，因此，编译器不会在栈上为对象分配内存。

```C++
class A
{
public:
    A() {}
    void destory()
    {
        delete this;
    }
private:
    ~A()
    {
    }
};
```

该方法存在的问题：

- 用 `new` 创建的对象，通常会使用 `delete` 释放该对象的内存空间，但此时类的外部无法调用析构函数，因此类内必须定义一个 `destory()` 函数，用来释放 `new` 创建的对象。
- 无法解决继承问题，因为如果这个类作为基类，析构函数要设置成 `virtual`，然后在派生类中重写该函数，来实现多态。但此时，析构函数是私有的，派生类中无法访问。

解决方法 2：

- 构造函数设置为 `protected`，并提供一个 `public` 的静态函数来完成构造，而不是在类的外部使用 `new` 构造；将析构函数设置为 `protected`。原因：类似于单例模式，也保证了在派生类中能够访问析构函数。通过调用 `create()` 函数在堆上创建对象。

```C++
class A
{
protected:
    A() {}
    ~A() {}
public:
    static A *create()
    {
        return new A();
    }
    void destory()
    {
        delete this;
    }
};
```

限制对象只能建立在栈上：

- 解决方法：将 ``operator new()`` 设置为私有。原因：当对象建立在堆上时，是采用 ``new`` 的方式进行建立，其底层会调用 ``operator new()`` 函数，因此只要对该函数加以限制，就能够防止对象建立在堆上。
  
```C++
class A
{
private:
    void *operator new(size_t t) {}    // 注意函数的第一个参数和返回值都是固定的
    void operator delete(void *ptr) {} // 重载了 new 就需要重载 delete
public:
    A() {}
    ~A() {}
};
```

### 内存对齐

内存对齐：编译器将程序中的每个“数据单元”安排在字的整数倍的地址指向的内存之中。

内存对齐的原则：

- 结构体变量的首地址能够被其最宽基本类型成员大小与对齐基数中的较小者所整除；
- 结构体每个成员相对于结构体首地址的偏移量 （``offset``） 都是该成员大小与对齐基数中的较小者的整数倍，如有需要编译器会在成员之间加上填充字节 （``internal padding``）；
- 结构体的总大小为结构体最宽基本类型成员大小与对齐基数中的较小者的整数倍，如有需要编译器会在最末一个成员之后加上填充字节 （``trailing padding``）。
- 
实例：

```C++
/*
说明：程序是在 64 位编译器下测试的
*/
#include <iostream>
using namespace std;
struct A
{
    short var; // 2 字节
    int var1;  // 8 字节 （内存对齐原则：填充 2 个字节） 2 (short) + 2 (填充) + 4 (int)= 8
    long var2; // 12 字节 8 + 4 (long) = 12
    char var3; // 16 字节 （内存对齐原则：填充 3 个字节）12 + 1 (char) + 3 (填充) = 16
    string s;  // 48 字节 16 + 32 (string) = 48
};
int main()
{
    short var;
    int var1;
    long var2;
    char var3;
    string s;
    A ex1;
    cout << sizeof(var) << endl;  // 2 short
    cout << sizeof(var1) << endl; // 4 int
    cout << sizeof(var2) << endl; // 4 long
    cout << sizeof(var3) << endl; // 1 char
    cout << sizeof(s) << endl;    // 32 string
    cout << sizeof(ex1) << endl;  // 48 struct
    return 0;
}
```

进行内存对齐的原因：（主要是硬件设备方面的问题）

- 某些硬件设备只能存取对齐数据，存取非对齐的数据可能会引发异常；
- 某些硬件设备不能保证在存取非对齐数据的时候的操作是原子操作；
- 相比于存取对齐的数据，存取非对齐的数据需要花费更多的时间；
- 某些处理器虽然支持非对齐数据的访问，但会引发对齐陷阱（``alignment trap``）；
- 某些硬件设备只支持简单数据指令非对齐存取，不支持复杂数据指令的非对齐存取。

内存对齐的优点：

- 便于在不同的平台之间进行移植，因为有些硬件平台不能够支持任意地址的数据访问，只能在某些地址处取某些特定的数据，否则会抛出异常；
- 提高内存的访问效率，因为 ``CPU`` 在读取内存时，是一块一块的读取。

### 类大小的计算

说明：类的大小是指类的实例化对象的大小，用 ``sizeof`` 对类型名操作时，结果是该类型的对象的大小。

计算原则：

- 遵循结构体的对齐原则。
- 与普通成员变量有关，与成员函数和静态成员无关。即普通成员函数，静态成员函数，静态数据成员，静态常量数据成员均对类的大小无影响。因为静态数据成员被类的对象共享，并不属于哪个具体的对象。
- 虚函数对类的大小有影响，是因为虚函数表指针的影响。
- 虚继承对类的大小有影响，是因为虚基表指针带来的影响。
- 空类的大小是一个特殊情况，空类的大小为 ``1``，当用 ``new`` 来创建一个空类的对象时，为了保证不同对象的地址不同，空类也占用存储空间。

实例：简单情况和空类情况

```C++
/*
说明：程序是在 64 位编译器下测试的
*/
#include <iostream>
using namespace std;
class A
{
private:
    static int s_var; // 不影响类的大小
    const int c_var;  // 4 字节
    int var;          // 8 字节 4 + 4 (int) = 8
    char var1;        // 12 字节 8 + 1 (char) + 3 (填充) = 12
public:
    A(int temp) : c_var(temp) {} // 不影响类的大小
    ~A() {}                    // 不影响类的大小
};
class B
{
};
int main()
{
    A ex1(4);
    B ex2;
    cout << sizeof(ex1) << endl; // 12 字节
    cout << sizeof(ex2) << endl; // 1 字节
    return 0;
}
```

带有虚函数的情况：（注意：虚函数的个数并不影响所占内存的大小，因为类对象的内存中只保存了指向虚函数表的指针。）

```C++
/*
说明：程序是在 64 位编译器下测试的
*/
#include <iostream>
using namespace std;
class A
{
private:
    static int s_var; // 不影响类的大小
    const int c_var;  // 4 字节
    int var;          // 8 字节 4 + 4 (int) = 8
    char var1;        // 12 字节 8 + 1 (char) + 3 (填充) = 12
public:
    A(int temp) : c_var(temp) {} // 不影响类的大小
    ~A() {}                      // 不影响类的大小
    virtual void f() { cout << "A::f" << endl; }
    virtual void g() { cout << "A::g" << endl; }
    virtual void h() { cout << "A::h" << endl; } // 24 字节 12 + 4 (填充) + 8 (指向虚函数的指针) = 24
};
int main()
{
    A ex1(4);
    A *p;
    cout << sizeof(p) << endl;   // 8 字节 注意：指针所占的空间和指针指向的数据类型无关
    cout << sizeof(ex1) << endl; // 24 字节
    return 0;
}
```

### 内存泄漏

- 由于疏忽或错误导致的程序未能释放已经不再使用的内存。并非指内存从物理上消失，而是指程序在运行过程中，由于疏忽或错误而失去了对该内存的控制，从而造成了内存的浪费。
- 常指堆内存泄漏，因为堆是动态分配的，而且是用户来控制的，如果使用不当，会产生内存泄漏。
- 使用 ``malloc、calloc、realloc、new`` 等分配内存时，使用完后要调用相应的 ``free`` 或 ``delete`` 释放内存，否则这块内存就会造成内存泄漏。

指针重新赋值

```C++
char *p = (char *)malloc(10);
char *p1 = (char *)malloc(10);
p = np;
```

开始时，指针 ``p`` 和 ``p1`` 分别指向一块内存空间，但指针 ``p`` 被重新赋值，导致 ``p`` 初始时指向的那块内存空间无法找到，从而发生了内存泄漏。

防止内存泄漏的方法：

- 内部封装：将内存的分配和释放封装到类中，在构造的时候申请内存，析构的时候释放内存。

```C++
#include <iostream>
#include <cstring>
using namespace std;
class A
{
private:
    char *p;
    unsigned int p_size;
public:
    A(unsigned int n = 1) // 构造函数中分配内存空间
    {
        p = new char[n];
        p_size = n;
    };
    ~A() // 析构函数中释放内存空间
    {
        if (p != NULL)
        {
            delete[] p; // 删除字符数组
            p = NULL;   // 防止出现野指针
        }
    };
    char *GetPointer()
    {
        return p;
    };
};
void fun()
{
    A ex(100);
    char *p = ex.GetPointer();
    strcpy(p, "Test");
    cout << p << endl;
}
int main()
{
    fun();
    return 0;
}
```

说明：但这样做并不是最佳的做法，在类的对象复制时，程序会出现同一块内存空间释放两次的情况，请看如下程序：

```C++
void fun1()
{
    A ex(100);
    A ex1 = ex; 
    char *p = ex.GetPointer();
    strcpy(p, "Test");
    cout << p << endl;
}
```

简单解释：对于 ``fun1`` 这个函数中定义的两个类的对象而言，在离开该函数的作用域时，会两次调用析构函数来释放空间，但是这两个对象指向的是同一块内存空间，所以导致同一块内存空间被释放两次，可以通过增加计数机制来避免这种情况，看如下程序：

```C++
#include <iostream>
#include <cstring>
using namespace std;
class A
{
private:
    char *p;
    unsigned int p_size;
    int *p_count; // 计数变量
public:
    A(unsigned int n = 1) // 在构造函数中申请内存
    {
        p = new char[n];
        p_size = n;
        p_count = new int;
        *p_count = 1;
        cout << "count is : " << *p_count << endl;
    };
    A(const A &temp)
    {
        p = temp.p;
        p_size = temp.p_size;
        p_count = temp.p_count;
        (*p_count)++; // 复制时，计数变量 +1
        cout << "count is : " << *p_count << endl;
    }
    ~A()
    {
        (*p_count)--; // 析构时，计数变量 -1
        cout << "count is : " << *p_count << endl; 
		if (*p_count == 0) // 只有当计数变量为 0 的时候才会释放该块内存空间
        {
            cout << "buf is deleted" << endl;
            if (p != NULL) 
            {
                delete[] p; // 删除字符数组
                p = NULL;   // 防止出现野指针
                if (p_count != NULL)
                {
                    delete p_count;
                    p_count = NULL;
                }
            }
        }
    };
    char *GetPointer()
    {
        return p;
    };
};
void fun()
{
    A ex(100);
    char *p = ex.GetPointer();
    strcpy(p, "Test");
    cout << p << endl;
    A ex1 = ex; // 此时计数变量会 +1
    cout << "ex1.p = " << ex1.GetPointer() << endl;
}
int main()
{
    fun();
    return 0;
}
```

程序运行结果：

```Shell
count is : 1
Test
count is : 2
ex1.p = Test
count is : 1
count is : 0
buf is deleted
```

解释下：程序运行结果的倒数 ``2、3`` 行是调用两次析构函数时进行的操作，在第二次调用析构函数时，进行内存空间的释放，从而会有倒数第 ``1`` 行的输出结果。

- 智能指针是 ``C++`` 中已经对内存泄漏封装好了一个工具，可以直接拿来使用。

内存泄漏检测工具的实现原理：

``valgrind`` 是一套 ``Linux`` 下，开放源代码（``GPL V2``）的仿真调试工具的集合，包括以下工具：

- ``Memcheck``：内存检查器（``valgrind`` 应用最广泛的工具），能够发现开发中绝大多数内存错误的使用情况，比如：使用未初始化的内存，使用已经释放了的内存，内存访问越界等。
- ``Callgrind``：检查程序中函数调用过程中出现的问题。
- ``Cachegrind``：检查程序中缓存使用出现的问题。
- ``Helgrind``：检查多线程程序中出现的竞争问题。
- ``Massif``：检查程序中堆栈使用中出现的问题。
- ``Extension``：可以利用 ``core`` 提供的功能，自己编写特定的内存调试工具。
- ``Memcheck`` 能够检测出内存问题，关键在于其建立了两个全局表：
  - ``Valid-Value`` 表：对于进程的整个地址空间中的每一个字节（``byte``），都有与之对应的 ``8`` 个 ``bits`` ；对于 ``CPU`` 的每个寄存器，也有一个与之对应的 ``bit`` 向量。这些 ``bits`` 负责记录该字节或者寄存器值是否具有有效的、已初始化的值。
  - ``Valid-Address`` 表：对于进程整个地址空间中的每一个字节（``byte``），还有与之对应的 ``1`` 个 ``bit``，负责记录该地址是否能够被读写。
  
检测原理：

- 当要读写内存中某个字节时，首先检查这个字节对应的 ``Valid-Address`` 表中对应的 ``bit``。如果该 ``bit`` 显示该位置是无效位置，``Memcheck`` 则报告读写错误。
- 内核（``core``）类似于一个虚拟的 ``CPU`` 环境，这样当内存中的某个字节被加载到真实的 ``CPU`` 中时，该字节在 ``Valid-Value`` 表对应的 ``bits`` 也被加载到虚拟的 ``CPU`` 环境中。一旦寄存器中的值，被用来产生内存地址，或者该值能够影响程序输出，则 ``Memcheck`` 会检查 ``Valid-Value`` 表对应的 ``bits``，如果该值尚未初始化，则会报告使用未初始化内存错误。

### 智能指针

智能指针是为了解决动态内存分配时带来的内存泄漏以及多次释放同一块内存空间而提出的。``C++11`` 中封装在了 ``<memory>`` 头文件中。

``C++11`` 中智能指针包括以下三种：

- 共享指针（``shared_ptr``）：资源可以被多个指针共享，使用计数机制表明资源被几个指针共享。通过 ``use_count()`` 查看资源的所有者的个数，可以通过 ``unique_ptr``、``weak_ptr`` 来构造，调用 ``release()`` 释放资源的所有权，计数减``1``，当计数减为 ``0`` 时，会自动释放内存空间，从而避免了内存泄漏。
- 独占指针（``unique_ptr``）：独享所有权的智能指针，资源只能被一个指针占有，该指针不能拷贝构造和赋值。但可以进行移动构造和移动赋值构造（调用 ``move()`` 函数），即一个 ``unique_ptr`` 对象赋值给另一个 ``unique_ptr`` 对象，可以通过该方法进行赋值。
- 弱指针（``weak_ptr``）：指向 ``share_ptr`` 指向的对象，能够解决由``shared_ptr``带来的循环引用问题。
  
智能指针的实现原理： 计数原理。

```C++
#include <iostream>
#include <memory>
template <typename T>
class SmartPtr
{
private : 
	T *_ptr;
	size_t *_count;
public:
	SmartPtr(T *ptr = nullptr) : _ptr(ptr)
	{
		if (_ptr)
		{
			_count = new size_t(1);
		}
		else
		{
			_count = new size_t(0);
		}
	}
	~SmartPtr()
	{
		(*this->_count)--;
		if (*this->_count == 0)
		{
			delete this->_ptr;
			delete this->_count;
		}
	}
	SmartPtr(const SmartPtr &ptr) // 拷贝构造：计数 +1
	{
		if (this != &ptr)
		{
			this->_ptr = ptr._ptr;
			this->_count = ptr._count;
			(*this->_count)++;
		}
	}
	SmartPtr &operator=(const SmartPtr &ptr) // 赋值运算符重载 
	{
		if (this->_ptr == ptr._ptr)
		{
			return *this;
		}
		if (this->_ptr) // 将当前的 ptr 指向的原来的空间的计数 -1
		{
			(*this->_count)--;
			if (this->_count == 0)
			{
				delete this->_ptr;
				delete this->_count;
			}
		}
		this->_ptr = ptr._ptr;
		this->_count = ptr._count;
		(*this->_count)++; // 此时 ptr 指向了新赋值的空间，该空间的计数 +1
		return *this;
	}
	T &operator*()
	{
		assert(this->_ptr == nullptr);
		return *(this->_ptr);
	}
	T *operator->()
	{
		assert(this->_ptr == nullptr);
		return this->_ptr;
	}
	size_t use_count()
	{
		return *this->count;
	}
};
```

一个 ``unique_ptr`` 怎么赋值给另一个 ``unique_ptr`` 对象？

- ``std::move()`` 可以实现将一个 ``unique_ptr`` 对象赋值给另一个 ``unique_ptr`` 对象，其目的是实现所有权的转移。

```C++
// A 作为一个类 
std::unique_ptr<A> ptr1(new A());
std::unique_ptr<A> ptr2 = std::move(ptr1);
```

智能指针可能出现的问题：循环引用

在如下例子中定义了两个类 ``Parent``、``Child``，在两个类中分别定义另一个类的对象的共享指针，由于在程序结束后，两个指针相互指向对方的内存空间，导致内存无法释放。

```C++
#include <iostream>
#include <memory>
using namespace std;
class Child;
class Parent;
class Parent {
private:
    shared_ptr<Child> ChildPtr;
public:
    void setChild(shared_ptr<Child> child) {
        this->ChildPtr = child;
    }
    void doSomething() {
        if (this->ChildPtr.use_count()) {
        }
    }
    ~Parent() {
    }
};
class Child {
private:
    shared_ptr<Parent> ParentPtr;
public:
    void setPartent(shared_ptr<Parent> parent) {
        this->ParentPtr = parent;
    }
    void doSomething() {
        if (this->ParentPtr.use_count()) {
        }
    }
    ~Child() {
    }
};
int main() {
    weak_ptr<Parent> wpp;
    weak_ptr<Child> wpc;
    {
        shared_ptr<Parent> p(new Parent);
        shared_ptr<Child> c(new Child);
        p->setChild(c);
        c->setPartent(p);
        wpp = p;
        wpc = c;
        cout << p.use_count() << endl; // 2
        cout << c.use_count() << endl; // 2
    }
    cout << wpp.use_count() << endl;  // 1
    cout << wpc.use_count() << endl;  // 1
    return 0;
}
```

循环引用的解决方法： ``weak_ptr``

循环引用：该被调用的析构函数没有被调用，从而出现了内存泄漏。

- ``weak_ptr`` 对被 ``shared_ptr`` 管理的对象存在 非拥有性（弱）引用，在访问所引用的对象前必须先转化为 ``shared_ptr``；
- ``weak_ptr`` 用来打断 ``shared_ptr`` 所管理对象的循环引用问题，若这种环被孤立（没有指向环中的外部共享指针），``shared_ptr`` 引用计数无法抵达 ``0``，内存被泄露；令环中的指针之一为弱指针可以避免该情况；
- ``weak_ptr`` 用来表达临时所有权的概念，当某个对象只有存在时才需要被访问，而且随时可能被他人删除，可以用 ``weak_ptr`` 跟踪该对象；需要获得所有权时将其转化为 ``shared_ptr``，此时如果原来的 ``shared_ptr`` 被销毁，则该对象的生命期被延长至这个临时的 ``shared_ptr`` 同样被销毁。

```C++
#include <iostream>
#include <memory>
using namespace std;
class Child;
class Parent;
class Parent {
private:
    //shared_ptr<Child> ChildPtr;
    weak_ptr<Child> ChildPtr;
public:
    void setChild(shared_ptr<Child> child) {
        this->ChildPtr = child;
    }
    void doSomething() {
        //new shared_ptr
        if (this->ChildPtr.lock()) {
        }
    }
    ~Parent() {
    }
};
class Child {
private:
    shared_ptr<Parent> ParentPtr;
public:
    void setPartent(shared_ptr<Parent> parent) {
        this->ParentPtr = parent;
    }
    void doSomething() {
        if (this->ParentPtr.use_count()) {
        }
    }
    ~Child() {
    }
};
int main() {
    weak_ptr<Parent> wpp;
    weak_ptr<Child> wpc;
    {
        shared_ptr<Parent> p(new Parent);
        shared_ptr<Child> c(new Child);
        p->setChild(c);
        c->setPartent(p);
        wpp = p;
        wpc = c;
        cout << p.use_count() << endl; // 2
        cout << c.use_count() << endl; // 1
    }
    cout << wpp.use_count() << endl;  // 0
    cout << wpc.use_count() << endl;  // 0
    return 0;
}
```

### ``C++ 11`` 新特性

#### ``auto`` 类型推导

``auto`` 关键字：自动类型推导，编译器会在 编译期间 通过初始值推导出变量的类型，通过 ``auto`` 定义的变量必须有初始值。

``auto`` 关键字基本的使用语法如下：

```C++
auto var = val1 + val2; // 根据 val1 和 val2 相加的结果推断出 var 的类型
```

注意：编译器推导出来的类型和初始值的类型并不完全一样，编译器会适当地改变结果类型使其更符合初始化规则。

#### ``decltype`` 类型推导

``decltype`` 关键字：``decltype`` 是“``declare type``”的缩写，译为“声明类型”。和 ``auto`` 的功能一样，都用来在编译时期进行自动类型推导。如果希望从表达式中推断出要定义的变量的类型，但是不想用该表达式的值初始化变量，这时就不能再用 ``auto``。``decltype`` 作用是选择并返回操作数的数据类型。

区别：

```C++
auto var = val1 + val2; 
decltype(val1 + val2) var1 = 0; 
```

- ``auto`` 根据 = 右边的初始值 ``val1 + val2`` 推导出变量的类型，并将该初始值赋值给变量 ``var``；``decltype`` 根据 ``val1 + val2`` 表达式推导出变量的类型，变量的初始值和与表达式的值无关。
- ``auto`` 要求变量必须初始化，因为它是根据初始化的值推导出变量的类型，而 ``decltype`` 不要求，定义变量的时候可初始化也可以不初始化。

#### ``lambda`` 表达式

``lambda``匿名函数的定义:

```C++
[capture list] (parameter list) -> return type
{
   function body;
};
```

``capture list``：捕获列表，指 ``lambda`` 所在函数中定义的局部变量的列表，通常为空。
``return type``、``parameter list``、``function body``：分别表示返回值类型、参数列表、函数体，和普通函数一样。

```C++
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
    int arr[4] = {4, 2, 3, 1};
    //对 a 数组中的元素进行升序排序
    sort(arr, arr+4, [=](int x, int y) -> bool{ return x < y; } );
    for(int n : arr){
        cout << n << " ";
    }
    return 0;
}
```

#### 范围 ``for`` 语句

```C++
for (declaration : expression){
    statement
}
```

``expression``：必须是一个序列，例如用花括号括起来的初始值列表、数组、``vector`` ，``string`` 等，这些类型的共同特点是拥有能返回迭代器的 ``beign``、``end`` 成员。
``declaration``：此处定义一个变量，序列中的每一个元素都能转化成该变量的类型，常用 ``auto`` 类型说明符。

```C++
#include <iostream>
#include <vector>
using namespace std;
int main() {
    char arr[] = "hello world!";
    for (char c : arr) {
        cout << c;
    }  
    return 0;
}
/*
程序执行结果为：
hello world!
*/
```

#### 右值引用

右值引用：绑定到右值的引用，用 ``&&`` 来获得右值引用，右值引用只能绑定到要销毁的对象。为了和右值引用区分开，常规的引用称为左值引用。

```C++
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    int var = 42;
    int &l_var = var;
    int &&r_var = var; // error: cannot bind rvalue reference of type 'int&&' to lvalue of type 'int' 错误：不能将右值引用绑定到左值上
    int &&r_var2 = var + 40; // 正确：将 r_var2 绑定到求和结果上
    return 0;
}
```

#### 标准库 ``move()`` 函数

``move()`` 函数：通过该函数可获得绑定到左值上的右值引用，该函数包括在 ``utility`` 头文件中。

#### [智能指针](#智能指针)

#### ``delete`` 函数和 ``default`` 函数

``delete`` 函数：``= delete`` 表示该函数不能被调用。
``default`` 函数：``= default`` 表示编译器生成默认的函数，例如：生成默认的构造函数。

```C++
#include <iostream>
using namespace std;
class A
{
public:
	A() = default; // 表示使用默认的构造函数
	~A() = default;	// 表示使用默认的析构函数
	A(const A &) = delete; // 表示类的对象禁止拷贝构造
	A &operator=(const A &) = delete; // 表示类的对象禁止拷贝赋值
};
int main()
{
	A ex1;
	A ex2 = ex1; // error: use of deleted function 'A::A(const A&)'
	A ex3;
	ex3 = ex1; // error: use of deleted function 'A& A::operator=(const A&)'
	return 0;
}
```

### 面向对象

面向对象：对象是指具体的某一个事物，这些事物的抽象就是类，类中包含数据（成员变量）和动作（成员方法）。

面向对象的三大特性：

- 封装：将具体的实现过程和数据封装成一个函数，只能通过接口进行访问，降低耦合性。
- 继承：子类继承父类的特征和行为，子类有父类的非 ``private`` 方法或成员变量，子类可以对父类的方法进行重写，增强了类之间的耦合性，但是当父类中的成员变量、成员函数或者类本身被 ``final`` 关键字修饰时，修饰的类不能继承，修饰的成员不能重写或修改。
- 多态：多态就是不同继承类的对象，对同一消息做出不同的响应，基类的指针指向或绑定到派生类的对象，使得基类指针呈现不同的表现方式。
  - 实现方法：多态是通过虚函数实现的，虚函数的地址保存在虚函数表中，虚函数表的地址保存在含有虚函数的类的实例对象的内存空间中。
  - 实现过程：在类中用 ``virtual`` 关键字声明的函数叫做虚函数；存在虚函数的类都有一个虚函数表，当创建一个该类的对象时，该对象有一个指向虚函数表的虚表指针（虚函数表和类对应的，虚表指针是和对象对应）；当基类指针指向派生类对象，基类指针调用虚函数时，基类指针指向派生类的虚表指针，由于该虚表指针指向派生类虚函数表，通过遍历虚表，寻找相应的虚函数。

### 重载、重写、隐藏的区别

重载：是指同一可访问区内被声明几个具有不同参数列（参数的类型、个数、顺序）的同名函数，根据参数列表确定调用哪个函数，重载不关心函数返回类型。

```C++
class A
{
public:
    void fun(int tmp);
    void fun(float tmp);        // 重载 参数类型不同（相对于上一个函数）
    void fun(int tmp, float tmp1); // 重载 参数个数不同（相对于上一个函数）
    void fun(float tmp, int tmp1); // 重载 参数顺序不同（相对于上一个函数）
    int fun(int tmp);            // error: 'int A::fun(int)' cannot be overloaded 错误：注意重载不关心函数返回类型
};
```

隐藏：是指派生类的函数屏蔽了与其同名的基类函数，主要只要同名函数，不管参数列表是否相同，基类函数都会被隐藏。

```C++
#include <iostream>
using namespace std;
class Base
{
public:
    void fun(int tmp, float tmp1) { cout << "Base::fun(int tmp, float tmp1)" << endl; }
};
class Derive : public Base
{
public:
    void fun(int tmp) { cout << "Derive::fun(int tmp)" << endl; } // 隐藏基类中的同名函数
};
int main()
{
    Derive ex;
    ex.fun(1);       // Derive::fun(int tmp)
    ex.fun(1, 0.01); // error: candidate expects 1 argument, 2 provided
    return 0;
}
```

上述代码中 ``ex.fun(1, 0.01);`` 出现错误，说明派生类中将基类的同名函数隐藏了。若是想调用基类中的同名函数，可以加上类型名指明 ``ex.Base::fun(1, 0.01);``，这样就可以调用基类中的同名函数。

重写(覆盖)：是指派生类中存在重新定义的函数。函数名、参数列表、返回值类型都必须同基类中被重写的函数一致，只有函数体不同。派生类调用时会调用派生类的重写函数，不会调用被重写函数。重写的基类中被重写的函数必须有 ``virtual`` 修饰。

```C++
#include <iostream>
using namespace std;
class Base
{
public:
    virtual void fun(int tmp) { cout << "Base::fun(int tmp) : " << tmp << endl; }
};
class Derived : public Base
{
public:
    virtual void fun(int tmp) { cout << "Derived::fun(int tmp) : " << tmp << endl; } // 重写基类中的 fun 函数
};
int main()
{
    Base *p = new Derived();
    p->fun(3); // Derived::fun(int) : 3
    return 0;
}
```

重写和重载的区别：

- 范围区别：对于类中函数的重载或者重写而言，重载发生在同一个类的内部，重写发生在不同的类之间（子类和父类之间）。
- 参数区别：重载的函数需要与原函数有相同的函数名、不同的参数列表，不关注函数的返回值类型；重写的函数的函数名、参数列表和返回值类型都需要和原函数相同，父类中被重写的函数需要有 ``virtual`` 修饰。
- ``virtual`` 关键字：重写的函数基类中必须有 ``virtual`` 关键字的修饰，重载的函数可以有 ``virtual`` 关键字的修饰也可以没有。
- 
隐藏和重写、重载的区别：

- 范围区别：隐藏与重载范围不同，隐藏发生在不同类中。
- 参数区别：隐藏函数和被隐藏函数参数列表可以相同，也可以不同，但函数名一定相同；当参数不同时，无论基类中的函数是否被 ``virtual`` 修饰，基类函数都是被隐藏，而不是重写。

### `i++`与`++i`

- 内建数据类型的情况，效率没有区别。
- **自定义数据类型的情况，`++i`效率更高。`++i`可以返回对象的引用，`i++`必须返回对象的值。**

### 不用中间变量，交换`a`和`b`的值

```C++
void swap(int& a, int& b)
{
    //1 可能产生数据溢出
    a = a + b;
    b = a - b;
    a = a - b;
    //2
    a ^= b;
    b ^= a;
    a ^= b;
}
```

### `main`函数执行完之后还会执行其他语句吗？

可以用`atexit()`函数注册程序正常终止时要被调用的函数，在`main`函数结束时，按注册时的顺序**反序**调用这些函数。

```C++
#include <stdlib.h>
int atexit(void (*)void);
```

### 宏参数的连接

```C++
// #把宏参数变成一个字符串
#define FUNC(a) #a
const char* str = FUNC(把宏参数变成一个字符串);//const char* str = "把宏参数变成一个字符串";

// ##把两个宏参数粘合在一起
#define FUNC(a, b) a##b
const char* str = FUNC("把两个宏参数", "粘合在一起");//const char* str = "把两个宏参数""粘合在一起";
```

### `const`和`#define`的特点和区别

- `#define`常量没有数据类型，生命周期止于编译期，它存在于程序的代码段。
- `const`常量有数据类型，存在于程序的数据段，并在堆栈中分配了空间。

### `const`的作用

- 定义**常量**，编译器可以对其进行数据**静态类型安全检查**。
- 修饰**函数引用类型形参**，节省临时对象的构造、复制和析构过程的消耗，避免引用对象被修改。
- 修饰**函数返回值**，避免返回值被直接修改。
- 修饰**类成员函数**，避免修改数据成员。

### `static`的作用

- **函数体中的静态变量**，在函数被调用的过程中维持值不变。
- **模块内的全局静态变量**，可以被模块内所有函数访问，但不能被其他模块访问。
- **模块内的静态函数**，只能被模块内的其他函数调用。

### 字节对齐

- 结构体变量的首地址能被其**最宽基本类型成员的大小和编译器默认对齐较小值**所整除。
- 结构体每个成员相对于结构体首地址的偏移量都是**成员大小和编译器默认对齐较小值**的整数倍。
- 结构体总大小为**结构体最宽基本类型成员大小和编译器默认对齐较小值**的整数倍。

### `C++`中4种指针转化运算符

- `const_cast`:特定情况下，将const限制解除。
- `dynamic_cast`:可以在运行时将一个指向派生类的基类指针还原成原来的派生类指针。
- `reinterpret_cast`:指针间强行转化。
- `static_cast`:安全类型转化，转换定义了相关构造函数、类型转换函数或者有继承关系的类，或者将数域宽度大的类型转换到较小的类型。

## 内联函数

### 宏函数和内联函数

- 宏函数使用预处理器实现，没有参数压栈、代码生成等操作，效率高。但是只是简单的文本替换，没有参数有效性检测和类型检查，返回值也不能被强制转换为合适的类型。
- 内联函数在编译时展开，可以直接镶嵌到目标代码中。内联函数是函数，可以完成类型检查功能。

## 友元

- 相同`class`的各个`objects`互为友元

## 构造函数

### 构造函数放在`private`区

```C++
class A
{
public:
	static A& getInstance();
private:
	A();
	A(const A& rhs);
}
A& A::getInstance()
{
	static A a;
	return a;
}
```

## 析构函数

## 虚函数

### 虚函数的实现

```C++
#include <iostream>

class A
{
public:virtual void AA() {}
};
class B
{
public:virtual void BB() {}
};
class C : public A
{
public:virtual void CC() {}
};
class D : public A, public B
{
	virtual void DD() {}
};
class E : public D
{
	virtual void EE() {}
};
class F : public A, public E
{
	virtual void FF() {}
};
int main()
{
	A a; B b; C c; D d; E e; F f;
	return 0;
}
```

![C++虚函数表](https://github.com/gongluck/images/blob/main/C++虚函数表.png)

- 如果类中有虚函数，编译器会至少为这个类分配一个成员指针指向一张虚函数表，表中每一项指向一个虚函数地址。
- 多重继承情况下，为每个含有虚函数的基类分配一个虚表指针。
- 每个派生类的虚函数表继承它各个基类的虚函数表。
- **如果派生类覆盖了基类对应的一个虚函数，则派生类的虚函数表的该项指向重载后的虚函数。**
- 在类对象的内存布局中，首先是该类的虚函数表指针，然后才是对象数据。
- 在通过对象指针调用一个虚函数时，编译器生成的代码将先获取对象类的虚函数表指针，然后调用虚函数表中对应的一项。
- 如果派生类自身新增虚函数的话，那么这个函数地址会被添加到复制基类的虚函表最后一个位置中，如果是多继承的情况下，会被添加到继承顺序最先继承的基类虚函数表中。

### 虚析构函数

- 只有**当一个类被用来作为基类时**，才会把析构函数写成虚析构函数，以按正确完整的顺序调用基类和派生类的析构函数。
- 基类的析构函数声明为虚函数，所以子类的析构函数也是虚函数，即使子类实例被当作基类来使用，虚函数表中指向的是具体子类的析构函数。

### 构造函数中调用虚函数

```C++
#include<iostream>

class A
{
public:
	A()
	{ 
		func();
	}
	virtual void func() 
	{
		std::cout << "A";
	}
};
class B : public A
{
public:
	B()
	{ 
		func();
	};
	virtual void func()
	{
		std::cout << "B";
	}
};
int main(int argc, char* argv[])
{
	A* p = new B();//输出AB
	p->func();//输出B
	return 0;
}
```

- **基类的构造函数在派生类构造函数之前执行，当基类构造函数运行时，派生类数据成员还没有被初始化。**
- 在构造子类时调用父类的构造函数，而父类的构造函数中又调用了虚成员函数，这个虚成员函数即使被子类重写，也**不允许发生多态的行为**。所以使用的是**静态绑定**，调用了当前构造的父类的函数。

## 模板

- 不当地使用模板会导致代码膨胀，严重影响程序运行效率，**应该把模板与参数无关的代码分离出来**。
- 当模板需要对某些类型进行特别处理时，使用**特化**。
