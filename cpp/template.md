# 模板

- [模板](#模板)
  - [模板定义](#模板定义)
  - [函数模板](#函数模板)
  - [类模板](#类模板)
  - [可变参数模板](#可变参数模板)
  - [模板特化](#模板特化)

## 模板定义

- 模板，创建类或者函数的蓝图或者公式，分为函数模板和类模板。
- 模板定义以关键字`template`开始，后跟一个模板参数列表。
- 模板参数列表不能为空。
- 模板类型参数前必须使用关键字`class`或者`typename`，在模板参数列表中这两个关键字含义相同，可互换使用。

## 函数模板

- 通过定义一个函数模板，可以避免为每一种类型定义一个新函数。
- 对于函数模板而言，模板类型参数可以用来指定返回类型或函数的参数类型，以及在函数体内用于变量声明或类型转换。
- 当调用一个模板时，编译器用函数实参来推断模板实参，从而使用实参的类型来确定绑定到模板参数的类型。

## 类模板

- 类模板以关键字 template 开始，后跟模板参数列表。
- 编译器不能为类模板推断模板参数类型，需要在使用该类模板时，在模板名后面的尖括号中指明类型。

## 可变参数模板

- 接受可变数目参数的模板函数或模板类，将可变数目的参数被称为参数包，包括模板参数包和函数参数包。
- 用省略号来指出一个模板参数或函数参数表示一个包，在模板参数列表中，`class...` 或 `typename...` 指出接下来的参数表示零个或多个类型的列表；一个类型名后面跟一个省略号表示零个或多个给定类型的非类型参数的列表。当需要知道包中有多少元素时，可以使用 `sizeof...` 运算符。

## 模板特化

- 模板并非对任何模板实参都合适、都能实例化，某些情况下，通用模板的定义对特定类型不合适，可能会编译失败，或者得不到正确的结果。当不希望使用模板版本时，可以定义类或者函数模板的一个特例化版本。
- 将函数模板中的全部类型进行特例化，称为函数模板特化。
- 将类模板中的部分或全部类型进行特例化，称为类模板特化。
- 全特化，模板中的模板参数全部特例化。
- 偏特化，模板中的模板参数只确定了一部分，剩余部分需要在编译器编译时确定。
- 当模板需要对某些类型进行特别处理时，使用特化。
