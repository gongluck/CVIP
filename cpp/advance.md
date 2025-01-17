# C++新特性

- [C++新特性](#c新特性)
  - [编程范式](#编程范式)
  - [nullptr](#nullptr)
  - [auto 类型推导](#auto-类型推导)
  - [decltype 类型推导](#decltype-类型推导)
  - [lambda 表达式](#lambda-表达式)
  - [范围 for 语句](#范围-for-语句)
  - [左值和右值](#左值和右值)
  - [std::move()](#stdmove)
  - [右值引用](#右值引用)
  - [emplace](#emplace)
  - [智能指针](#智能指针)
  - [delete 函数和 default 函数](#delete-函数和-default-函数)
  - [async 函数](#async-函数)

## 编程范式

![编程范式](https://github.com/gongluck/images/blob/main/cpp/programming_paradigm.png)

- 现代 C++ 支持"面向过程"、"面向对象"、"泛型"、"模板元"、"函数式"这五种主要的编程范式。

## nullptr

- C++11 引入了 `nullptr` 关键字，专门用来区分空指针、`0`。
- nullptr 的类型为 `nullptr_t`，能够隐式的转换为任何指针或成员指针的类型，也能和他们进行相等或者不等的比较。

## auto 类型推导

- auto 关键字，自动类型推导。
- 编译器会在编译期间通过初始值推导出变量的类型，通过 auto 定义的变量必须有初始值。
- 编译器推导出来的类型和初始值的类型并不完全一样，编译器会适当地改变结果类型使其更符合初始化规则。

## decltype 类型推导

- decltype 是 declare type 的缩写，译为声明类型。
- 和 auto 的功能一样，都用来在编译时期进行自动类型推导。
- 如果希望从表达式中推断出要定义的变量的类型，但是不想用该表达式的值初始化变量，这时就不能再用 auto。decltype 作用是选择并返回操作数的数据类型。

## lambda 表达式

- 编译器将 lambda 表达式翻译成一个 **未命名类的未命名对象**。该类含有一个重载的函数调用运算符。
- 每个 lambda 表达式都会有一个独特的类型，而这个类型只有编译器才知道，我们是无法直接写出来的，所以必须用 auto。
- 使用 `[=]` 按值捕获的时候，lambda 表达式使用的是变量的独立副本，非常安全。
- 使用 `[&]` 的方式捕获引用就存在风险，引用的变量可能发生了变化，甚至可能会失效，导致难以预料的后果。
- **无捕获的 lambda 表达式可以显式转换成一个函数指针**。

## 范围 for 语句

```c++
for (declaration : expression){
  statement
}
```

## 左值和右值

- **左值，指表达式结束后依然存在的持久对象。**
- **右值，表达式结束就不再存在的临时对象。**
- 左值和右值的区别，**左值持久,右值短暂**。
- `lvalue` 是 `loactor value` 的缩写，可意为存储在内存中、有明确存储地址(可寻址)的数据，而 `rvalue` 译为 `read value`，指的是那些可以提供数据值的数据(不一定可以寻址，例如存储于寄存器中的数据)。
- 左值引用不能绑定到要转换的表达式、字面常量或返回右值的表达式。右值引用恰好相反，可以绑定到这类表达式，但不能绑定到一个左值上。
- 右值引用必须绑定到右值的引用，通过 `&&` 获得。右值引用只能绑定到一个将要销毁的对象上，因此可以自由地移动其资源。
- **类型是右值引用的变量是一个左值！**
- `std::move` 可以将一个左值强制转化为右值，继而可以通过右值引用使用该值，以用于移动语义。

  ```c++
  #include <iostream>
  using namespace std;
  void fun1(int& tmp)
  {
    cout << "fun1(int& tmp):" << tmp << endl;
  }
  void fun2(int&& tmp)
  {
    cout << "fun2(int&& tmp)" << tmp << endl;
  }
  int main()
  {
    int var = 11;
    fun1(12); // error: cannot bind non-const lvalue reference of type 'int&' to an rvalue of type 'int'
    fun1(var);
    fun2(1);
  }
  ```

## std::move()

- 尽量为元素实现转移构造和转移赋值函数，在加入容器的时候使用 `std::move()` 来转移，减少元素复制的成本。

  ```c++
  template <typename T>
  typename remove_reference<T>::type&& move(T&& t)
  {
    return static_cast<typename remove_reference<T>::type &&>(t);
  }
  ```

- 右值传递给上述函数的形参 `T&&` 依然是右值，即 `T&& &&` 相当于 `T&&`。
- 左值传递给上述函数的形参 `T&&` 依然是左值，即 `T&& &` 相当于 `T&`。
- 通过引用折叠原理可以知道，`move()` 函数的形参既可以是左值也可以是右值。

## 右值引用

- 右值引用，绑定到右值的引用，用 `&&` 来获得右值引用，右值引用只能绑定到要销毁的对象。
- 为了和右值引用区分开，常规的引用称为左值引用。
- 左值也可以当做右值使用。

  ```c++
  #include <iostream>
  #include <vector>
  using namespace std;
  int main()
  {
    int var = 42;
    int &l_var = var;
    int &&r_var = var; // error: cannot bind rvalue reference of type 'int&&' to lvalue of type 'int' 错误:不能将右值引用绑定到左值上
    int &&r_var2 = var + 40; // 正确:将 r_var2 绑定到求和结果上
    return 0;
  }
  ```

## emplace

- C++11 为容器新增加的 `emplace` 操作函数，可以就地构造元素，免去了构造后再拷贝、转移的成本。

## 智能指针

```mermaid
classDiagram
  class _Mutex_base {
    # _S_need_barriers : enum
  }

  _Mutex_base <|-- _Sp_counted_base
  class _Sp_counted_base  {
    - _M_use_count : _Atomic_word //对象数据的引用计数，每次引用计数加1，每次释放计数减一，当计数为0时，释放对象空间。
    - _M_weak_count : _Atomic_word //引用计数的计数，即use_count的计数。
  }

  _Sp_counted_base *-- __shared_count
  class __shared_count{
    - _M_pi : _Sp_counted_base<_Lp>*
  }

  __shared_count *-- __shared_ptr
  class __shared_ptr{
    - _M_ptr : _Tp*
    - _M_refcount : __shared_count<_Lp>
  }

  __shared_ptr <|-- shared_ptr
  class shared_ptr{
    + shared_ptr(__r : weak_ptr<_Tp1>&)
  }

  _TP_Pointer *-- __tuple_type
  _Tp_Deleter *-- __tuple_type
  __tuple_type *-- unique_ptr
  class unique_ptr{
    - _M_t : __tuple_type
  }

  _Sp_counted_base *-- __weak_count
  class __weak_count{
    - _M_pi : _Sp_counted_base<_Lp>*
  }

  __weak_count *-- __weak_ptr
  class __weak_ptr{
    - _M_ptr : _Tp*
    - _M_refcount : __weak_count<_Lp>
  }

  __weak_ptr <|-- weak_ptr
  class weak_ptr{
    + shared_ptr<_Tp> lock()
  }
```

- 智能指针是为了解决动态内存分配时带来的内存泄漏以及多次释放同一块内存空间而提出的。C++11 中封装在了 `<memory>` 头文件中。
- 共享指针([shared_ptr](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/shared_ptr.h#L1205))，资源可以被多个指针共享，使用计数机制表明资源被几个指针共享。通过 `use_count()` 查看资源的所有者的个数，可以通过 `unique_ptr`、`weak_ptr` 来构造，调用 `release()` 释放资源的所有权，计数减 1，当计数减为 0 时，会自动释放内存空间，从而避免了内存泄漏。
- 独占指针([unique_ptr](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/unique_ptr.h#L80))，独享所有权的智能指针，资源只能被一个指针占有，该指针不能拷贝构造和赋值。但可以进行移动构造和移动赋值构造(调用 `move()` 函数)，即一个 unique_ptr 对象赋值给另一个 unique_ptr 对象，可以通过该方法进行赋值。
- 弱指针([weak_ptr](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/shared_ptr.h#L1387))，指向 share_ptr 指向的对象，能够解决由 shared_ptr 带来的循环引用问题。在需要的时候，可以调用成员函数 lock()，获取 shared_ptr(强引用)。

## delete 函数和 default 函数

- delete 函数，`= delete` 表示该函数不能被调用。
- default 函数，`= default` 表示编译器生成默认的函数。

## async 函数

- 函数 `async()` 的含义是异步运行一个任务，隐含的动作是启动一个线程去执行，但不绝对保证立即启动(也可以在第一个参数传递 std::launch::async，要求立即启动线程)。
- 任务的生存周期和返回的对象关联，对象销毁需要等待任务结束。
