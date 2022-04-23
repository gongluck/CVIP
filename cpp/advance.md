# ```C++```新特性

- [```C++```新特性](#c新特性)
  - [nullptr](#nullptr)
  - [auto类型推导](#auto类型推导)
  - [decltype类型推导](#decltype类型推导)
  - [lambda表达式](#lambda表达式)
  - [范围for语句](#范围for语句)
  - [左值和右值](#左值和右值)
  - [std::move()](#stdmove)
  - [右值引用](#右值引用)
  - [标准库move()函数](#标准库move函数)
  - [智能指针](#智能指针)
  - [```delete```函数和```default```函数](#delete函数和default函数)
  - [模板](#模板)

## nullptr

```C++11```引入了```nullptr```关键字,专门用来区分空指针、```0```。而```nullptr```的类型为```nullptr_t```,能够隐式的转换为任何指针或成员指针的类型,也能和他们进行相等或者不等的比较。

## auto类型推导

```auto```关键字:自动类型推导,编译器会在编译期间通过初始值推导出变量的类型,通过```auto```定义的变量必须有初始值。

```auto```关键字基本的使用语法如下:

```C++
auto var = val1 + val2; // 根据 val1 和 val2 相加的结果推断出 var 的类型
```

注意:编译器推导出来的类型和初始值的类型并不完全一样,编译器会适当地改变结果类型使其更符合初始化规则。

## decltype类型推导

```decltype```关键字:```decltype```是```declare type```的缩写,译为声明类型。和```auto```的功能一样,都用来在编译时期进行自动类型推导。如果希望从表达式中推断出要定义的变量的类型,但是不想用该表达式的值初始化变量,这时就不能再用```auto```。```decltype```作用是选择并返回操作数的数据类型。

区别:

```C++
auto var = val1 + val2; 
decltype(val1 + val2) var1 = 0; 
```

- ```auto```根据```=```右边的初始值```val1 + val2```推导出变量的类型,并将该初始值赋值给变量```var```;```decltype```根据```val1 + val2```表达式推导出变量的类型,变量的初始值和与表达式的值无关。
- ```auto```要求变量必须初始化,因为它是根据初始化的值推导出变量的类型,而```decltype```不要求,定义变量的时候可初始化也可以不初始化。

## lambda表达式

编译器将```lambda```表达式翻译成一个未命名类的未命名对象。该类含有一个重载的函数调用运算符。

```lambda```匿名函数的定义:

```C++
[capture list] (parameter list) -> return type
{
   function body;
};
```

```capture list```:捕获列表,指```lambda```所在函数中定义的局部变量的列表,通常为空。
```return type```、```parameter list```、```function body```:分别表示返回值类型、参数列表、函数体,和普通函数一样。

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

## 范围for语句

```C++
for (declaration : expression){
    statement
}
```

```expression```:必须是一个序列,例如用花括号括起来的初始值列表、数组、```vector```,```string```等,这些类型的共同特点是拥有能返回迭代器的```beign```、```end```成员。
```declaration```:此处定义一个变量,序列中的每一个元素都能转化成该变量的类型,常用```auto```类型说明符。

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
程序执行结果为:
hello world!
*/
```
## 左值和右值

```左值:指表达式结束后依然存在的持久对象```。
```右值:表达式结束就不再存在的临时对象```。

- 左值和右值的区别:左值持久,右值短暂
- 左值引用不能绑定到要转换的表达式、字面常量或返回右值的表达式。右值引用恰好相反,可以绑定到这类表达式,但不能绑定到一个左值上。
- 右值引用必须绑定到右值的引用,通过```&&```获得。右值引用只能绑定到一个将要销毁的对象上,因此可以自由地移动其资源。
- ```std::move```可以将一个左值强制转化为右值,继而可以通过右值引用使用该值,以用于移动语义。

```C++
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

```C++
template <typename T>
typename remove_reference<T>::type&& move(T&& t)
{
	return static_cast<typename remove_reference<T>::type &&>(t);
}
```

- 右值传递给上述函数的形参```T&&```依然是右值,即```T&& &&```相当于```T&&```。
- 左值传递给上述函数的形参```T&&```依然是左值,即```T&& &```相当于```T&```。
- 通过引用折叠原理可以知道,```move()```函数的形参既可以是左值也可以是右值。

```remove_reference```具体实现:

```C++
//原始的,最通用的版本
template <typename T> struct remove_reference{
    typedef T type;  //定义 T 的类型别名为 type
};
//部分版本特例化,将用于左值引用和右值引用
template <class T> struct remove_reference<T&> //左值引用
{ typedef T type; }
template <class T> struct remove_reference<T&&> //右值引用
{ typedef T type; }
//举例如下,下列定义的a、b、c三个变量都是int类型
int i;
remove_refrence<decltype(42)>::type a;  //使用原版本,
remove_refrence<decltype(i)>::type b;   //左值引用特例版本
remove_refrence<decltype(std::move(i))>::type b;  //右值引用特例版本 
```

```C++
int var = 10; 
```

转化过程:

- ```std::move(var)``` => ```std::move(int&& &)``` => 折叠后 ```std::move(int&)```
- 此时:```T```的类型为```int&```,```typename remove_reference<T>::type```为```int```,这里使用```remove_reference```的左值引用的特例化版本
- 通过```static_cast```将```int&```强制转换为```int&&```

整个```std::move```被实例化如下:

```C++
int&& move(int& t) 
{
    return static_cast<int&&>(t);
}
```

```std::move()```实现原理:

- 利用引用折叠原理将右值经过```T&&```传递类型保持不变还是右值,而左值经过```T&&```变为普通的左值引用,以保证模板可以传递任意实参,且保持类型不变;
- 然后通过```remove_refrence```移除引用,得到具体的类型```T```;
- 最后通过```static_cast<>```进行强制类型转换,返回```T&&```右值引用。

## 右值引用

右值引用:绑定到右值的引用,用```&&```来获得右值引用,右值引用只能绑定到要销毁的对象。为了和右值引用区分开,常规的引用称为左值引用。

左值的英文简写为```lvalue```,右值的英文简写为```rvalue```。很多人认为它们分别是```left value```、```right value```的缩写,其实不然。
```lvalue```是```loactor value```的缩写,可意为存储在内存中、有明确存储地址(可寻址)的数据,而```rvalue```译为```read value```,指的是那些可以提供数据值的数据(不一定可以寻址,例如存储于寄存器中的数据)。
```C++```中的左值也可以当做右值使用。

```C++
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

## 标准库move()函数

```move()```函数:通过该函数可获得绑定到左值上的右值引用,该函数包括在```utility```头文件中。

## 智能指针

智能指针是为了解决动态内存分配时带来的内存泄漏以及多次释放同一块内存空间而提出的。```C++11```中封装在了```<memory>```头文件中。

```C++11```中智能指针包括以下三种:

- 共享指针(```shared_ptr```):资源可以被多个指针共享,使用计数机制表明资源被几个指针共享。通过```use_count()```查看资源的所有者的个数,可以通过```unique_ptr```、```weak_ptr```来构造,调用```release()```释放资源的所有权,计数减```1```,当计数减为 ```0```时,会自动释放内存空间,从而避免了内存泄漏。
- 独占指针(```unique_ptr```):独享所有权的智能指针,资源只能被一个指针占有,该指针不能拷贝构造和赋值。但可以进行移动构造和移动赋值构造(调用```move()```函数),即一个```unique_ptr```对象赋值给另一个```unique_ptr```对象,可以通过该方法进行赋值。
- 弱指针(```weak_ptr```):指向```share_ptr```指向的对象,能够解决由```shared_ptr```带来的循环引用问题。
  
智能指针的实现原理: 计数原理。

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
	SmartPtr(const SmartPtr &ptr) // 拷贝构造:计数 +1
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
		(*this->_count)++; // 此时 ptr 指向了新赋值的空间,该空间的计数 +1
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

一个```unique_ptr```怎么赋值给另一个```unique_ptr```对象？

- ```std::move()```可以实现将一个```unique_ptr```对象赋值给另一个```unique_ptr```对象,其目的是实现所有权的转移。

```C++
// A 作为一个类 
std::unique_ptr<A> ptr1(new A());
std::unique_ptr<A> ptr2 = std::move(ptr1);
```

智能指针可能出现的问题:循环引用

在如下例子中定义了两个类```Parent```、```Child```,在两个类中分别定义另一个类的对象的共享指针,由于在程序结束后,两个指针相互指向对方的内存空间,导致内存无法释放。

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

循环引用的解决方法:```weak_ptr```

循环引用:该被调用的析构函数没有被调用,从而出现了内存泄漏。

- ```weak_ptr```对被```shared_ptr```管理的对象存在非拥有性(弱)引用,在访问所引用的对象前必须先转化为```shared_ptr```;
- ```weak_ptr```用来打断```shared_ptr```所管理对象的循环引用问题,若这种环被孤立(没有指向环中的外部共享指针),```shared_ptr```引用计数无法抵达```0```,内存被泄露;令环中的指针之一为弱指针可以避免该情况;
- ```weak_ptr```用来表达临时所有权的概念,当某个对象只有存在时才需要被访问,而且随时可能被他人删除,可以用```weak_ptr```跟踪该对象;需要获得所有权时将其转化为```shared_ptr```,此时如果原来的```shared_ptr```被销毁,则该对象的生命期被延长至这个临时的 ```shared_ptr```同样被销毁。

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

## ```delete```函数和```default```函数

```delete```函数:```= delete```表示该函数不能被调用。
```default```函数:```= default```表示编译器生成默认的函数,例如:生成默认的构造函数。

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

## 模板

模板:创建类或者函数的蓝图或者公式,分为函数模板和类模板。
实现方式:模板定义以关键字```template```开始,后跟一个模板参数列表。

- 模板参数列表不能为空;
- 模板类型参数前必须使用关键字```class```或者```typename```,在模板参数列表中这两个关键字含义相同,可互换使用。

```C++
template <typename T, typename U, ...>
```

函数模板:通过定义一个函数模板,可以避免为每一种类型定义一个新函数。

- 对于函数模板而言,模板类型参数可以用来指定返回类型或函数的参数类型,以及在函数体内用于变量声明或类型转换。
- 函数模板实例化:当调用一个模板时,编译器用函数实参来推断模板实参,从而使用实参的类型来确定绑定到模板参数的类型。

```C++
#include<iostream>
using namespace std;
template <typename T>
T add_fun(const T & tmp1, const T & tmp2){
    return tmp1 + tmp2;
}
int main(){
    int var1, var2;
    cin >> var1 >> var2;
    cout << add_fun(var1, var2);
    double var3, var4;
    cin >> var3 >> var4;
    cout << add_fun(var3, var4);
    return 0;
}
```

类模板:类似函数模板,类模板以关键字```template```开始,后跟模板参数列表。但是,编译器不能为类模板推断模板参数类型,需要在使用该类模板时,在模板名后面的尖括号中指明类型。

```C++
#include <iostream>
using namespace std;
template <typename T>
class Complex
{
public:
    //构造函数
    Complex(T a, T b)
    {
        this->a = a;
        this->b = b;
    }
    //运算符重载
    Complex<T> operator+(Complex &c)
    {
        Complex<T> tmp(this->a + c.a, this->b + c.b);
        cout << tmp.a << " " << tmp.b << endl;
        return tmp;
    }
private:
    T a;
    T b;
};
int main()
{
    Complex<int> a(10, 20);
    Complex<int> b(20, 30);
    Complex<int> c = a + b;
    return 0;
}
```

可变参数模板:接受可变数目参数的模板函数或模板类。将可变数目的参数被称为参数包,包括模板参数包和函数参数包。

- 模板参数包:表示零个或多个模板参数;
- 函数参数包:表示零个或多个函数参数。

用省略号来指出一个模板参数或函数参数表示一个包,在模板参数列表中,```class...```或```typename...```指出接下来的参数表示零个或多个类型的列表;一个类型名后面跟一个省略号表示零个或多个给定类型的非类型参数的列表。当需要知道包中有多少元素时,可以使用```sizeof...```运算符。

```C++
template <typename T, typename... Args> // Args 是模板参数包
void foo(const T &t, const Args&... rest); // 可变参数模板,rest 是函数参数包
```

```C++
#include <iostream>
using namespace std;
template <typename T>
void print_fun(const T &t)
{
    cout << t << endl; // 最后一个元素
}
template <typename T, typename... Args>
void print_fun(const T &t, const Args &...args)
{
    cout << t << " ";
    print_fun(args...);
}
int main()
{
    print_fun("Hello", "world", "!");
    return 0;
}
/*运行结果:
Hello world !
*/
```

可变参数函数通常是递归的,第一个版本的```print_fun```负责终止递归并打印初始调用中的最后一个实参。第二个版本的```print_fun```是可变参数版本,打印绑定到```t```的实参,并用来调用自身来打印函数参数包中的剩余值。

模板特化的原因:模板并非对任何模板实参都合适、都能实例化,某些情况下,通用模板的定义对特定类型不合适,可能会编译失败,或者得不到正确的结果。因此,当不希望使用模板版本时,可以定义类或者函数模板的一个特例化版本。

模板特化:模板参数在某种特定类型下的具体实现。分为函数模板特化和类模板特化

- 函数模板特化:将函数模板中的全部类型进行特例化,称为函数模板特化。
- 类模板特化:将类模板中的部分或全部类型进行特例化,称为类模板特化。

特化分为全特化和偏特化:

- 全特化:模板中的模板参数全部特例化。
- 偏特化:模板中的模板参数只确定了一部分,剩余部分需要在编译器编译时确定。

定义函数模板的特化版本,本质上是接管了编译器的工作,为原函数模板定义了一个特殊实例,而不是函数重载,函数模板特化并不影响函数匹配。

```C++
#include <iostream>
#include <cstring>
using namespace std;
//函数模板
template <class T>
bool compare(T t1, T t2)
{
    cout << "通用版本:";
    return t1 == t2;
}
template <> //函数模板特化
bool compare(char *t1, char *t2)
{
    cout << "特化版本:";
    return strcmp(t1, t2) == 0;
}
int main(int argc, char *argv[])
{
    char arr1[] = "hello";
    char arr2[] = "abc";
    cout << compare(123, 123) << endl;
    cout << compare(arr1, arr2) << endl;
    return 0;
}
/*
运行结果:
通用版本:1
特化版本:0
*/
```

- 不当地使用模板会导致代码膨胀,严重影响程序运行效率,```应该把模板与参数无关的代码分离出来```。
- 当模板需要对某些类型进行特别处理时,使用```特化```。
