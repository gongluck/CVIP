/*
 * @Author: gongluck
 * @Date: 2022-07-04 22:27:13
 * @Last Modified by: gongluck
 * @Last Modified time: 2022-07-04 23:05:15
 */

#include <iostream>

#define PRINTSIZE(x) \
  std::cout << "sizeof(" << #x << "):" << sizeof(x) << std::endl;
#define PRINTFUNC std::cout << this << "\t" << __func__ << std::endl

class nullclass
{
};

class datamember
{
public:
  void func1()
  {
    PRINTFUNC;
  }

private:
  int i;
};

class virtualfunc
{
public:
  virtual void vfunc()
  {
    PRINTFUNC;
  }
  void func2()
  {
    PRINTFUNC;
  }
};

class multinheritance : public datamember, public virtualfunc
{
public:
  void func3()
  {
    PRINTFUNC;
  }

private:
  double d;
};

int main()
{
  std::cout << "object model" << std::endl;

  nullclass null;
  datamember dmember;
  virtualfunc vfunc;
  multinheritance multinher;

  PRINTSIZE(null);      // 1
  PRINTSIZE(dmember);   // 4
  PRINTSIZE(vfunc);     // 8
  PRINTSIZE(multinher); // 24 字节对齐

  multinher.func1(); // 0x7ffdaf85b238  func1
  multinher.func2(); // 0x7ffdaf85b230  func2
  multinher.func3(); // 0x7ffdaf85b230  func3 this指针偏移

  return 0;
}