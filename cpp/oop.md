# 面向对象

- [面向对象](#面向对象)
  - [面向对象特性](#面向对象特性)
  - [重载、重写、隐藏的区别](#重载重写隐藏的区别)
  - [构造函数、析构函数是否需要定义成虚函数？](#构造函数析构函数是否需要定义成虚函数)
  - [如何避免拷贝？](#如何避免拷贝)
  - [多重继承](#多重继承)
  - [虚函数的实现](#虚函数的实现)

## 面向对象特性

- 封装，将具体的实现过程和数据封装成一个函数，只能通过接口进行访问，降低耦合性。
- 继承，子类继承父类的特征和行为，子类有父类的非`private`方法或成员变量，子类可以对父类的方法进行重写，增强了类之间的耦合性，但是当父类中的成员变量、成员函数或者类本身被`final`关键字修饰时，修饰的类不能继承，修饰的成员不能重写或修改。
- 多态，多态就是不同继承类的对象，对同一消息做出不同的响应，基类的指针指向或绑定到派生类的对象，使得基类指针呈现不同的表现方式。
  - 实现方法，多态是通过虚函数实现的，虚函数的地址保存在虚函数表中，虚函数表的地址保存在含有虚函数的类的实例对象的内存空间中。
  - 实现过程，在类中用`virtual`关键字声明的函数叫做虚函数；存在虚函数的类都有一个虚函数表，当创建一个该类的对象时，该对象有一个指向虚函数表的虚表指针(虚函数表和类对应的，虚表指针是和对象对应)；当基类指针指向派生类对象，基类指针调用虚函数时，基类指针指向派生类的虚表指针，由于该虚表指针指向派生类虚函数表，通过遍历虚表，寻找相应的虚函数。

## 重载、重写、隐藏的区别

- 重载，是指同一可访问区内被声明几个具有不同参数列(参数的类型、个数、顺序)的同名函数，根据参数列表确定调用哪个函数，重载不关心函数返回类型。

  ```c++
  class A
  {
  public:
      void fun(int tmp);
      void fun(float tmp);        // 重载 参数类型不同(相对于上一个函数)
      void fun(int tmp, float tmp1); // 重载 参数个数不同(相对于上一个函数)
      void fun(float tmp, int tmp1); // 重载 参数顺序不同(相对于上一个函数)
      int fun(int tmp);            // error, 'int A::fun(int)' cannot be overloaded 错误，注意重载不关心函数返回类型
  };
  ```

- 隐藏，是指派生类的函数屏蔽了与其同名的基类函数，主要只要同名函数，不管参数列表是否相同，基类函数都会被隐藏。

  ```c++
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
      ex.fun(1, 0.01); // error, candidate expects 1 argument, 2 provided
      return 0;
  }
  //若是想调用基类中的同名函数，可以加上类型名指明
  //ex.Base::fun(1, 0.01);
  //这样就可以调用基类中的同名函数。
  ```

- 重写(覆盖)，是指派生类中存在重新定义的函数。函数名、参数列表、返回值类型都必须同基类中被重写的函数一致，只有函数体不同。派生类调用时会调用派生类的重写函数，不会调用被重写函数。重写的基类中被重写的函数必须有`virtual`修饰。

  ```c++
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

- 隐藏、重写和重载的区别，
  - 范围区别，重载发生在同一个类的内部，隐藏和重写发生在不同的类之间(子类和父类之间)。
  - 参数区别，隐藏函数和被隐藏函数参数列表可以相同，也可以不同；重载的函数需要与原函数有相同的函数名、不同的参数列表，不关注函数的返回值类型；重写的函数的函数名、参数列表和返回值类型都需要和原函数相同，父类中被重写的函数需要有`virtual`修饰。
  - virtual 关键字，重写的函数基类中必须有 virtual 关键字的修饰，隐藏和重载的函数可以有 virtual 关键字的修饰也可以没有。

## 构造函数、析构函数是否需要定义成虚函数？

- 构造函数不定义成虚函数

  - 从存储空间的角度考虑，构造函数是在实例化对象的时候进行调用，如果此时将构造函数定义成虚函数，需要通过访问该对象所在的内存空间才能进行虚函数的调用(因为需要通过指向虚函数表的指针调用虚函数表，虽然虚函数表在编译时就有了，但是没有虚函数的指针，虚函数的指针只有在创建了对象才有)，但是此时该对象还未创建，便无法进行虚函数的调用。所以构造函数不能定义成虚函数。
  - 从使用的角度考虑，虚函数是基类的指针指向派生类的对象时，通过该指针实现对派生类的虚函数的调用，构造函数是在创建对象时自动调用的。
  - 从实现上考虑，虚函数表是在创建对象之后才有的，因此不能定义成虚函数。
  - 从类型上考虑，在创建对象时需要明确其类型。

- 析构函数一般定义成虚函数
  - 析构函数定义成虚函数是为了防止内存泄漏，因为当基类的指针或者引用指向或绑定到派生类的对象时，如果未将基类的析构函数定义成虚函数，会调用基类的析构函数，那么只能将基类的成员所占的空间释放掉，派生类中特有的就会无法释放内存空间导致内存泄漏。

## 如何避免拷贝？

- 最直观的想法是，将类的拷贝构造函数和赋值构造函数声明为私有`private`，但对于类的成员函数和友元函数依然可以调用，达不到完全禁止类的对象被拷贝的目的，而且程序会出现错误，因为未对函数进行定义。
- 解决方法，定义一个基类，将其中的拷贝构造函数和赋值构造函数声明为私有`private`。派生类以私有`private`的方式继承基类。

  ```c++
  class Uncopyable
  {
  public:
    Uncopyable() {}
    ~Uncopyable() {}
  private:
    Uncopyable(const Uncopyable &);            // 拷贝构造函数
    Uncopyable &operator=(const Uncopyable &); // 赋值构造函数
  };
  class A : private Uncopyable // 注意继承方式
  {
  };
  ```

- 能够保证，在派生类 A 的成员函数和友元函数中无法进行拷贝操作，因为无法调用基类 Uncopyable 的拷贝构造函数或赋值构造函数。同样，在类的外部也无法进行拷贝操作。

## 多重继承

- 多重继承(多继承)，是指从多个直接基类中产生派生类。
- 多重继承容易出现的问题，命名冲突和数据冗余问题。

  ```c++
  #include <iostream>
  using namespace std;
  // 间接基类
  class Base1
  {
  public:
    int var1;
  };
  // 直接基类
  class Base2 : public Base1
  {
  public:
    int var2;
  };
  // 直接基类
  class Base3 : public Base1
  {
  public:
    int var3;
  };
  // 派生类
  class Derive : public Base2, public Base3
  {
  public:
    void set_var1(int tmp) { var1 = tmp; } // error, reference to 'var1' is ambiguous. 命名冲突
    void set_var2(int tmp) { var2 = tmp; }
    void set_var3(int tmp) { var3 = tmp; }
    void set_var4(int tmp) { var4 = tmp; }
  private:
    int var4;
  };
  int main()
  {
    Derive d;
    return 0;
  }
  ```

- 对于派生类 Derive 上述代码中存在直接继承关系和间接继承关系。
  - 直接继承，Base2、Base3
  - 间接继承，Base1
- 对于派生类中继承的的成员变量 var1，从继承关系来看，实际上保存了两份，一份是来自基类 Base2，一份来自基类 Base3。因此，出现了命名冲突。
- 解决方法 1，声明出现冲突的成员变量来源于哪个类

  ```c++
  #include <iostream>
  using namespace std;
  // 间接基类
  class Base1
  {
  public:
    int var1;
  };
  // 直接基类
  class Base2 : public Base1
  {
  public:
    int var2;
  };
  // 直接基类
  class Base3 : public Base1
  {
  public:
    int var3;
  };
  // 派生类
  class Derive : public Base2, public Base3
  {
  public:
    void set_var1(int tmp) { Base2::var1 = tmp; } // 这里声明成员变量来源于类 Base2，当然也可以声明来源于类 Base3
    void set_var2(int tmp) { var2 = tmp; }
    void set_var3(int tmp) { var3 = tmp; }
    void set_var4(int tmp) { var4 = tmp; }
  private:
    int var4;
  };
  int main()
  {
    Derive d;
    return 0;
  }
  ```

- 解决方法 2，虚继承

  - 使用虚继承的目的，保证存在命名冲突的成员变量在派生类中只保留一份，使间接基类中的成员在派生类中只保留一份。
  - 在菱形继承关系中，间接基类称为虚基类，直接基类和间接基类之间的继承关系称为虚继承。
  - 实现方式，在继承方式前面加上`virtual`关键字。

  ```c++
  #include <iostream>
  using namespace std;
  // 间接基类，即虚基类
  class Base1
  {
  public:
    int var1;
  };
  // 直接基类
  class Base2 : virtual public Base1 // 虚继承
  {
  public:
    int var2;
  };
  // 直接基类
  class Base3 : virtual public Base1 // 虚继承
  {
  public:
    int var3;
  };
  // 派生类
  class Derive : public Base2, public Base3
  {
  public:
    void set_var1(int tmp) { var1 = tmp; }
    void set_var2(int tmp) { var2 = tmp; }
    void set_var3(int tmp) { var3 = tmp; }
    void set_var4(int tmp) { var4 = tmp; }
  private:
    int var4;
  };
  int main()
  {
    Derive d;
    return 0;
  }
  ```

## 虚函数的实现

```c++
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

![C++虚函数表](https://github.com/gongluck/images/blob/main/cpp/virtual_function_table.png)

- 如果类中有虚函数，编译器会至少为这个类分配一个成员指针指向一张虚函数表，表中每一项指向一个虚函数地址。
- **多重继承情况下，为每个含有虚函数的基类分配一个虚表指针**。
- 每个派生类的虚函数表继承它各个基类的虚函数表。
- **如果派生类覆盖了基类对应的一个虚函数，则派生类的虚函数表的该项指向重载后的虚函数**。
- 在类对象的内存布局中，首先是该类的虚函数表指针，然后才是对象数据。
- 在通过对象指针调用一个虚函数时，编译器生成的代码将先获取对象类的虚函数表指针，然后调用虚函数表中对应的一项。
- **如果派生类自身新增虚函数的话，那么这个函数地址会被添加到最先继承的基类虚函数表的复制基类的虚函表最后一个位置中**。
