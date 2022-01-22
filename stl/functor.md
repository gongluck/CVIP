# 函数对象```functor```

- [函数对象```functor```](#函数对象functor)
  - [标准库函数对象](#标准库函数对象)

## 标准库函数对象

  [stl_function.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_function.h)

  <details>
  <summary>标准库函数对象</summary>

  ```c++
  //一元函数对象基类
  template <class Arg, class Result>
  struct unary_function
  {
    typedef Arg argument_type;
    typedef Result result_type;
  };

  //二元函数对象基类
  template <class Arg1, class Arg2, class Result>
  struct binary_function
  {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
  };

  //加
  template <class T>
  struct plus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x + y; }
  };

  //减
  template <class T>
  struct minus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x - y; }
  };

  //乘
  template <class T>
  struct multiplies : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x * y; }
  };

  //除
  template <class T>
  struct divides : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x / y; }
  };

  //加法单位元
  template <class T>
  inline T identity_element(plus<T>) { return T(0); }

  //乘法单位元
  template <class T>
  inline T identity_element(multiplies<T>) { return T(1); }

  //余
  template <class T>
  struct modulus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x % y; }
  };

  //相反数
  template <class T>
  struct negate : public unary_function<T, T>
  {
    T operator()(const T &x) const { return -x; }
  };

  //判等
  template <class T>
  struct equal_to : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x == y; }
  };

  //不等
  template <class T>
  struct not_equal_to : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x != y; }
  };

  //大于
  template <class T>
  struct greater : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x > y; }
  };

  //小于
  template <class T>
  struct less : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x < y; }
  };

  //大于等于
  template <class T>
  struct greater_equal : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x >= y; }
  };

  //小于等于
  template <class T>
  struct less_equal : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x <= y; }
  };

  //逻辑与
  template <class T>
  struct logical_and : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x && y; }
  };

  //逻辑或
  template <class T>
  struct logical_or : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x || y; }
  };

  //逻辑非
  template <class T>
  struct logical_not : public unary_function<T, bool>
  {
    bool operator()(const T &x) const { return !x; }
  };
  ```
  </details>