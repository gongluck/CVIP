# 适配器adapter

- [适配器adapter](#适配器adapter)
  - [容器适配器](#容器适配器)
    - [容器适配器stack](#容器适配器stack)
    - [容器适配器queue](#容器适配器queue)
    - [容器适配器priority\_queue](#容器适配器priority_queue)
  - [迭代器适配器](#迭代器适配器)
    - [插入迭代器insert iterator](#插入迭代器insert-iterator)
    - [反向迭代器reverse iterator](#反向迭代器reverse-iterator)
    - [流迭代器steam iterator](#流迭代器steam-iterator)
  - [函数适配器](#函数适配器)

## 容器适配器

### 容器适配器stack

  栈`stack`只在尾部对元素做增删操作，满足的底层容器有`vector`、`deque`和`list`。

  [stl_stack.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_stack.h#L36)

  <details>
  <summary>stack</summary>

  ```C++
  //栈
  #ifndef __STL_LIMITED_DEFAULT_TEMPLATES
  template <class T, class Sequence = deque<T>>
  #else
  template <class T, class Sequence>
  #endif
  class stack
  {
    friend bool operator== __STL_NULL_TMPL_ARGS(const stack &, const stack &);
    friend bool operator<__STL_NULL_TMPL_ARGS(const stack &, const stack &);

  public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

  protected:
    Sequence c; //底层容器 符合可在尾端增删

  public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type &x) { c.push_back(x); }
    void pop() { c.pop_back(); }
  };

  template <class T, class Sequence>
  bool operator==(const stack<T, Sequence> &x, const stack<T, Sequence> &y)
  {
    return x.c == y.c;
  }

  template <class T, class Sequence>
  bool operator<(const stack<T, Sequence> &x, const stack<T, Sequence> &y)
  {
    return x.c < y.c;
  }
  ```
  </details>

### 容器适配器queue

  队列`queue`只在尾部对元素做增操作和头部做删除，满足的底层容器有`deque`和`list`。(`vector`没有`pop_front`，即使有也会相当低效！)

  [stl_queue.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_queue.h#L36)

  <details>
  <summary>queue</summary>

  ```C++
  //队列
  template <class T, class Sequence = deque<T>>
  class queue
  {
    friend bool operator== __STL_NULL_TMPL_ARGS(const queue &x, const queue &y);
    friend bool operator<__STL_NULL_TMPL_ARGS(const queue &x, const queue &y);

  public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

  protected:
    Sequence c; //底层容器 符合尾增头删

  public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }
    void push(const value_type &x) { c.push_back(x); }
    void pop() { c.pop_front(); }
  };

  template <class T, class Sequence>
  bool operator==(const queue<T, Sequence> &x, const queue<T, Sequence> &y)
  {
    return x.c == y.c;
  }

  template <class T, class Sequence>
  bool operator<(const queue<T, Sequence> &x, const queue<T, Sequence> &y)
  {
    return x.c < y.c;
  }
  ```
  </details>

### 容器适配器priority_queue

  优先队列`priority_queue`底层依赖`堆`实现。

  [stl_queue.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_queue.h#L79)

  <details>
  <summary>priority_queue</summary>

  ```C++
  //优先队列
  template <class T, class Sequence = vector<T>,
            class Compare = less<typename Sequence::value_type>>
  class priority_queue
  {
  public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

  protected:
    Sequence c;   //底层容器 默认vector
    Compare comp; //比较方法 默认less

  public:
    priority_queue() : c() {}
    explicit priority_queue(const Compare &x) : c(), comp(x) {}

    priority_queue(const value_type *first, const value_type *last,
                  const Compare &x) : c(first, last), comp(x)
    {
      //构建堆
      make_heap(c.begin(), c.end(), comp);
    }
    priority_queue(const value_type *first, const value_type *last)
        : c(first, last) { make_heap(c.begin(), c.end(), comp); }

    bool empty() const
    {
      return c.empty();
    }
    size_type size() const { return c.size(); }
    const_reference top() const { return c.front(); }
    void push(const value_type &x)
    {
      __STL_TRY
      {
        //添加元素到底层容器
        c.push_back(x);
        //将最后的元素添加到堆
        push_heap(c.begin(), c.end(), comp);
      }
      __STL_UNWIND(c.clear());
    }
    void pop()
    {
      __STL_TRY
      {
        //将堆顶元素移出，实际是移动到底层容器的末尾并调整堆
        pop_heap(c.begin(), c.end(), comp);
        //从底层容器中移除最后元素
        c.pop_back();
      }
      __STL_UNWIND(c.clear());
    }
  };
  ```
  </details>

## 迭代器适配器

### 插入迭代器insert iterator

  [stl_iterator.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator.h#L381)

  <details>
  <summary>insert iterator</summary>

  ```C++
  //尾部插入迭代器
  template <class Container>
  class back_insert_iterator
  {
  protected:
    Container *container; //指向容器

  public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    explicit back_insert_iterator(Container &x) : container(&x) {}
    //在容器尾部插入
    back_insert_iterator<Container> &
    operator=(const typename Container::value_type &value)
    {
      container->push_back(value);
      return *this;
    }
    //前置和后置operator++都返回*this，结合operator=，+=实现链式尾部插入
    back_insert_iterator<Container> &operator*() { return *this; }
    back_insert_iterator<Container> &operator++() { return *this; }
    back_insert_iterator<Container> &operator++(int) { return *this; }
  };

  //生成尾部插入迭代器
  template <class Container>
  inline back_insert_iterator<Container> back_inserter(Container &x)
  {
    return back_insert_iterator<Container>(x);
  }

  //头部插入迭代器
  template <class Container>
  class front_insert_iterator
  {
  protected:
    Container *container; //指向容器

  public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    explicit front_insert_iterator(Container &x) : container(&x) {}
    //在容器头部插入
    front_insert_iterator<Container> &
    operator=(const typename Container::value_type &value)
    {
      container->push_front(value);
      return *this;
    }
    //前置和后置operator++都返回*this，结合operator=，+=实现链式头部插入
    front_insert_iterator<Container> &operator*() { return *this; }
    front_insert_iterator<Container> &operator++() { return *this; }
    front_insert_iterator<Container> &operator++(int) { return *this; }
  };

  //生成头部插入迭代器
  template <class Container>
  inline front_insert_iterator<Container> front_inserter(Container &x)
  {
    return front_insert_iterator<Container>(x);
  }

  //插入迭代器
  template <class Container>
  class insert_iterator
  {
  protected:
    Container *container;              //指向容器
    typename Container::iterator iter; //当前插入点的迭代器

  public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    insert_iterator(Container &x, typename Container::iterator i)
        : container(&x), iter(i) {}
    //在iter处插入
    insert_iterator<Container> &
    operator=(const typename Container::value_type &value)
    {
      iter = container->insert(iter, value);
      ++iter;
      return *this;
    }
    //前置和后置operator++都返回*this，结合operator=，+=实现链式插入
    insert_iterator<Container> &operator*() { return *this; }
    insert_iterator<Container> &operator++() { return *this; }
    insert_iterator<Container> &operator++(int) { return *this; }
  };

  //生成插入迭代器
  template <class Container, class Iterator>
  inline insert_iterator<Container> inserter(Container &x, Iterator i)
  {
    typedef typename Container::iterator iter;
    return insert_iterator<Container>(x, iter(i));
  }
  ```
  </details>

### 反向迭代器reverse iterator

  [stl_iterator.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator.h#L526)

  <details>
  <summary>reverse iterator</summary>

  ```C++
  template <class BidirectionalIterator, class T, class Reference = T &,
            class Distance = ptrdiff_t>
  //反向双向迭代器
  class reverse_bidirectional_iterator
  {
    typedef reverse_bidirectional_iterator<BidirectionalIterator, T, Reference,
                                          Distance>
        self;

  protected:
    BidirectionalIterator current; //当前位置迭代器

  public:
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef Reference reference;

    reverse_bidirectional_iterator() {}
    explicit reverse_bidirectional_iterator(BidirectionalIterator x)
        : current(x) {}
    BidirectionalIterator base() const { return current; }
    //解引用操作
    Reference operator*() const
    {
      BidirectionalIterator tmp = current;
      //向前移动后，解引用
      return *--tmp;
    }
  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    //成员引用运算符返回operator*操作结果的地址
    pointer operator->() const
    {
      return &(operator*());
    }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */

    //反向操作
    self &operator++()
    {
      --current;
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      --current;
      return tmp;
    }
    self &operator--()
    {
      ++current;
      return *this;
    }
    self operator--(int)
    {
      self tmp = *this;
      ++current;
      return tmp;
    }
  };

  template <class BidirectionalIterator, class T, class Reference,
            class Distance>
  inline bool operator==(
      const reverse_bidirectional_iterator<BidirectionalIterator, T, Reference,
                                          Distance> &x,
      const reverse_bidirectional_iterator<BidirectionalIterator, T, Reference,
                                          Distance> &y)
  {
    return x.base() == y.base();
  }

  // This is the new version of reverse_iterator, as defined in the
  //  draft C++ standard.  It relies on the iterator_traits template,
  //  which in turn relies on partial specialization.  The class
  //  reverse_bidirectional_iterator is no longer part of the draft
  //  standard, but it is retained for backward compatibility.

  template <class Iterator>
  class reverse_iterator
  {
  protected:
    Iterator current;

  public:
    typedef typename iterator_traits<Iterator>::iterator_category
        iterator_category;
    typedef typename iterator_traits<Iterator>::value_type
        value_type;
    typedef typename iterator_traits<Iterator>::difference_type
        difference_type;
    typedef typename iterator_traits<Iterator>::pointer
        pointer;
    typedef typename iterator_traits<Iterator>::reference
        reference;

    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator> self;

  public:
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type x) : current(x) {}

    reverse_iterator(const self &x) : current(x.current) {}

    template <class Iter>
    reverse_iterator(const reverse_iterator<Iter> &x) : current(x.current)
    {
    }

    iterator_type base() const
    {
      return current;
    }
    reference operator*() const
    {
      Iterator tmp = current;
      return *--tmp;
    }
  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const
    {
      return &(operator*());
    }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */

    self &operator++()
    {
      --current;
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      --current;
      return tmp;
    }
    self &operator--()
    {
      ++current;
      return *this;
    }
    self operator--(int)
    {
      self tmp = *this;
      ++current;
      return tmp;
    }

    self operator+(difference_type n) const
    {
      return self(current - n);
    }
    self &operator+=(difference_type n)
    {
      current -= n;
      return *this;
    }
    self operator-(difference_type n) const
    {
      return self(current + n);
    }
    self &operator-=(difference_type n)
    {
      current += n;
      return *this;
    }
    reference operator[](difference_type n) const { return *(*this + n); }
  };

  template <class Iterator>
  inline bool operator==(const reverse_iterator<Iterator> &x,
                        const reverse_iterator<Iterator> &y)
  {
    return x.base() == y.base();
  }

  template <class Iterator>
  inline bool operator<(const reverse_iterator<Iterator> &x,
                        const reverse_iterator<Iterator> &y)
  {
    return y.base() < x.base();
  }

  template <class Iterator>
  inline typename reverse_iterator<Iterator>::difference_type
  operator-(const reverse_iterator<Iterator> &x,
            const reverse_iterator<Iterator> &y)
  {
    return y.base() - x.base();
  }

  template <class Iterator>
  inline reverse_iterator<Iterator>
  operator+(reverse_iterator<Iterator>::difference_type n,
            const reverse_iterator<Iterator> &x)
  {
    return reverse_iterator<Iterator>(x.base() - n);
  }
  ```
  </details>

### 流迭代器steam iterator

  [stl_iterator.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator.h#L914)

  <details>
  <summary>steam iterator</summary>

  ```C++
  //输入流迭代器
  template <class T, class Distance = ptrdiff_t>
  class istream_iterator
  {
    friend bool
    operator== __STL_NULL_TMPL_ARGS(const istream_iterator<T, Distance> &x,
                                    const istream_iterator<T, Distance> &y);

  protected:
    istream *stream; //指向输入流
    T value;
    bool end_marker; //流结束标记
    //读取一个元素
    void read()
    {
      end_marker = (*stream) ? true : false;
      if (end_marker)
        *stream >> value; //从流中读取一个元素
      end_marker = (*stream) ? true : false;
    }

  public:
    typedef input_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef const T *pointer;
    typedef const T &reference;

    //默认从cin中读取
    istream_iterator() : stream(&cin), end_marker(false) {}
    //自定义输入流时，构造时就读取一次了
    istream_iterator(istream &s) : stream(&s) { read(); }
    //解引用操作获取读入的元素
    reference operator*() const { return value; }
  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const
    {
      return &(operator*());
    }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */
    istream_iterator<T, Distance> &operator++()
    {
      read();
      return *this;
    }
    istream_iterator<T, Distance> operator++(int)
    {
      istream_iterator<T, Distance> tmp = *this;
      read();
      return tmp;
    }
  };

  template <class T, class Distance>
  inline bool operator==(const istream_iterator<T, Distance> &x,
                        const istream_iterator<T, Distance> &y)
  {
    return x.stream == y.stream && x.end_marker == y.end_marker ||
          x.end_marker == false && y.end_marker == false;
  }

  //输出流迭代器
  template <class T>
  class ostream_iterator
  {
  protected:
    ostream *stream; //指向输出流
    const char *string;

  public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;

    ostream_iterator(ostream &s) : stream(&s), string(0) {}
    ostream_iterator(ostream &s, const char *c) : stream(&s), string(c) {}
    //输出到流
    ostream_iterator<T> &operator=(const T &value)
    {
      *stream << value;
      if (string)
        *stream << string;
      return *this;
    }
    //前置和后置operator++都返回*this，结合operator=，+=实现链式输出
    ostream_iterator<T> &operator*() { return *this; }
    ostream_iterator<T> &operator++() { return *this; }
    ostream_iterator<T> &operator++(int) { return *this; }
  };
  ```
  </details>

## 函数适配器

  [stl_function.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_function.h#L166)

  <details>
  <summary>function adapter</summary>

  ```C++
  //一元非
  template <class Predicate>
  class unary_negate
      : public unary_function<typename Predicate::argument_type, bool>
  {
  protected:
    Predicate pred; //一元谓词对象

  public:
    explicit unary_negate(const Predicate &x) : pred(x) {}
    bool operator()(const typename Predicate::argument_type &x) const
    {
      return !pred(x);
    }
  };
  //构造一元非对象
  template <class Predicate>
  inline unary_negate<Predicate> not1(const Predicate &pred)
  {
    return unary_negate<Predicate>(pred);
  }

  //二元非
  template <class Predicate>
  class binary_negate
      : public binary_function<typename Predicate::first_argument_type,
                              typename Predicate::second_argument_type,
                              bool>
  {
  protected:
    Predicate pred; //二元谓词对象

  public:
    explicit binary_negate(const Predicate &x) : pred(x) {}
    bool operator()(const typename Predicate::first_argument_type &x,
                    const typename Predicate::second_argument_type &y) const
    {
      return !pred(x, y);
    }
  };
  //构造二元非对象
  template <class Predicate>
  inline binary_negate<Predicate> not2(const Predicate &pred)
  {
    return binary_negate<Predicate>(pred);
  }

  //绑定二元函数对象的第一个参数
  template <class Operation>
  class binder1st
      : public unary_function<typename Operation::second_argument_type,
                              typename Operation::result_type>
  {
  protected:
    Operation op;                                  //二元函数对象
    typename Operation::first_argument_type value; //绑定的参数

  public:
    binder1st(const Operation &x,
              const typename Operation::first_argument_type &y)
        : op(x), value(y) {}
    typename Operation::result_type
    operator()(const typename Operation::second_argument_type &x) const
    {
      //绑定参数
      return op(value, x);
    }
  };
  template <class Operation, class T>
  inline binder1st<Operation> bind1st(const Operation &op, const T &x)
  {
    typedef typename Operation::first_argument_type arg1_type;
    return binder1st<Operation>(op, arg1_type(x));
  }

  //绑定二元函数对象的第二个参数
  template <class Operation>
  class binder2nd
      : public unary_function<typename Operation::first_argument_type,
                              typename Operation::result_type>
  {
  protected:
    Operation op;                                   //二元函数对象
    typename Operation::second_argument_type value; //绑定的参数

  public:
    binder2nd(const Operation &x,
              const typename Operation::second_argument_type &y)
        : op(x), value(y) {}
    typename Operation::result_type
    operator()(const typename Operation::first_argument_type &x) const
    {
      //绑定参数
      return op(x, value);
    }
  };
  template <class Operation, class T>
  inline binder2nd<Operation> bind2nd(const Operation &op, const T &x)
  {
    typedef typename Operation::second_argument_type arg2_type;
    return binder2nd<Operation>(op, arg2_type(x));
  }

  //级联调用
  template <class Operation1, class Operation2>
  class unary_compose : public unary_function<typename Operation2::argument_type,
                                              typename Operation1::result_type>
  {
  protected:
    Operation1 op1;
    Operation2 op2;

  public:
    unary_compose(const Operation1 &x, const Operation2 &y) : op1(x), op2(y) {}
    typename Operation1::result_type
    operator()(const typename Operation2::argument_type &x) const
    {
      return op1(op2(x));
    }
  };
  template <class Operation1, class Operation2>
  inline unary_compose<Operation1, Operation2> compose1(const Operation1 &op1,
                                                        const Operation2 &op2)
  {
    return unary_compose<Operation1, Operation2>(op1, op2);
  }

  template <class Operation1, class Operation2, class Operation3>
  class binary_compose
      : public unary_function<typename Operation2::argument_type,
                              typename Operation1::result_type>
  {
  protected:
    Operation1 op1;
    Operation2 op2;
    Operation3 op3;

  public:
    binary_compose(const Operation1 &x, const Operation2 &y,
                  const Operation3 &z) : op1(x), op2(y), op3(z) {}
    typename Operation1::result_type
    operator()(const typename Operation2::argument_type &x) const
    {
      return op1(op2(x), op3(x));
    }
  };
  template <class Operation1, class Operation2, class Operation3>
  inline binary_compose<Operation1, Operation2, Operation3>
  compose2(const Operation1 &op1, const Operation2 &op2, const Operation3 &op3)
  {
    return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
  }

  //将函数指针转化成一元函数对象
  template <class Arg, class Result>
  class pointer_to_unary_function : public unary_function<Arg, Result>
  {
  protected:
    Result (*ptr)(Arg);

  public:
    pointer_to_unary_function() {}
    explicit pointer_to_unary_function(Result (*x)(Arg)) : ptr(x) {}
    Result operator()(Arg x) const { return ptr(x); }
  };
  template <class Arg, class Result>
  inline pointer_to_unary_function<Arg, Result> ptr_fun(Result (*x)(Arg))
  {
    return pointer_to_unary_function<Arg, Result>(x);
  }

  //将函数指针转化成二元函数对象
  template <class Arg1, class Arg2, class Result>
  class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result>
  {
  protected:
    Result (*ptr)(Arg1, Arg2);

  public:
    pointer_to_binary_function() {}
    explicit pointer_to_binary_function(Result (*x)(Arg1, Arg2)) : ptr(x) {}
    Result operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
  };
  template <class Arg1, class Arg2, class Result>
  inline pointer_to_binary_function<Arg1, Arg2, Result>
  ptr_fun(Result (*x)(Arg1, Arg2))
  {
    return pointer_to_binary_function<Arg1, Arg2, Result>(x);
  }

  template <class T>
  struct identity : public unary_function<T, T>
  {
    const T &operator()(const T &x) const { return x; }
  };

  //提取pair的first
  template <class Pair>
  struct select1st : public unary_function<Pair, typename Pair::first_type>
  {
    const typename Pair::first_type &operator()(const Pair &x) const
    {
      return x.first;
    }
  };
  //提取pair的second
  template <class Pair>
  struct select2nd : public unary_function<Pair, typename Pair::second_type>
  {
    const typename Pair::second_type &operator()(const Pair &x) const
    {
      return x.second;
    }
  };

  template <class Arg1, class Arg2>
  struct project1st : public binary_function<Arg1, Arg2, Arg1>
  {
    Arg1 operator()(const Arg1 &x, const Arg2 &) const { return x; }
  };
  template <class Arg1, class Arg2>
  struct project2nd : public binary_function<Arg1, Arg2, Arg2>
  {
    Arg2 operator()(const Arg1 &, const Arg2 &y) const { return y; }
  };

  //增加const修饰
  template <class Result>
  struct constant_void_fun
  {
    typedef Result result_type;
    result_type val;
    constant_void_fun(const result_type &v) : val(v) {}
    const result_type &operator()() const { return val; }
  };

  #ifndef __STL_LIMITED_DEFAULT_TEMPLATES
  template <class Result, class Argument = Result>
  #else
  template <class Result, class Argument>
  #endif
  struct constant_unary_fun : public unary_function<Argument, Result>
  {
    Result val;
    constant_unary_fun(const Result &v) : val(v) {}
    const Result &operator()(const Argument &) const { return val; }
  };

  #ifndef __STL_LIMITED_DEFAULT_TEMPLATES
  template <class Result, class Arg1 = Result, class Arg2 = Arg1>
  #else
  template <class Result, class Arg1, class Arg2>
  #endif
  struct constant_binary_fun : public binary_function<Arg1, Arg2, Result>
  {
    Result val;
    constant_binary_fun(const Result &v) : val(v) {}
    const Result &operator()(const Arg1 &, const Arg2 &) const
    {
      return val;
    }
  };

  template <class Result>
  inline constant_void_fun<Result> constant0(const Result &val)
  {
    return constant_void_fun<Result>(val);
  }

  template <class Result>
  inline constant_unary_fun<Result, Result> constant1(const Result &val)
  {
    return constant_unary_fun<Result, Result>(val);
  }

  template <class Result>
  inline constant_binary_fun<Result, Result, Result> constant2(const Result &val)
  {
    return constant_binary_fun<Result, Result, Result>(val);
  }

  //随机数生成器
  // Note: this code assumes that int is 32 bits.
  class subtractive_rng : public unary_function<unsigned int, unsigned int>
  {
  private:
    unsigned int table[55];
    size_t index1;
    size_t index2;

  public:
    unsigned int operator()(unsigned int limit)
    {
      index1 = (index1 + 1) % 55;
      index2 = (index2 + 1) % 55;
      table[index1] = table[index1] - table[index2];
      return table[index1] % limit;
    }

    void initialize(unsigned int seed)
    {
      unsigned int k = 1;
      table[54] = seed;
      size_t i;
      for (i = 0; i < 54; i++)
      {
        size_t ii = (21 * (i + 1) % 55) - 1;
        table[ii] = k;
        k = seed - k;
        seed = table[ii];
      }
      for (int loop = 0; loop < 4; loop++)
      {
        for (i = 0; i < 55; i++)
          table[i] = table[i] - table[(1 + i + 30) % 55];
      }
      index1 = 0;
      index2 = 31;
    }

    subtractive_rng(unsigned int seed) { initialize(seed); }
    subtractive_rng() { initialize(161803398u); }
  };

  // Adaptor function objects: pointers to member functions.

  // There are a total of 16 = 2^4 function objects in this family.
  //  (1) Member functions taking no arguments vs member functions taking
  //       one argument.
  //  (2) Call through pointer vs call through reference.
  //  (3) Member function with void return type vs member function with
  //      non-void return type.
  //  (4) Const vs non-const member function.

  // Note that choice (4) is not present in the 8/97 draft C++ standard,
  //  which only allows these adaptors to be used with non-const functions.
  //  This is likely to be recified before the standard becomes final.
  // Note also that choice (3) is nothing more than a workaround: according
  //  to the draft, compilers should handle void and non-void the same way.
  //  This feature is not yet widely implemented, though.  You can only use
  //  member functions returning void if your compiler supports partial
  //  specialization.

  // All of this complexity is in the function objects themselves.  You can
  //  ignore it by using the helper function mem_fun, mem_fun_ref,
  //  mem_fun1, and mem_fun1_ref, which create whichever type of adaptor
  //  is appropriate.

  template <class S, class T>
  class mem_fun_t : public unary_function<T *, S>
  {
  public:
    explicit mem_fun_t(S (T::*pf)()) : f(pf) {}
    S operator()(T *p) const { return (p->*f)(); }

  private:
    S(T::*f)
    ();
  };

  template <class S, class T>
  class const_mem_fun_t : public unary_function<const T *, S>
  {
  public:
    explicit const_mem_fun_t(S (T::*pf)() const) : f(pf) {}
    S operator()(const T *p) const { return (p->*f)(); }

  private:
    S(T::*f)
    () const;
  };

  template <class S, class T>
  class mem_fun_ref_t : public unary_function<T, S>
  {
  public:
    explicit mem_fun_ref_t(S (T::*pf)()) : f(pf) {}
    S operator()(T &r) const { return (r.*f)(); }

  private:
    S(T::*f)
    ();
  };

  template <class S, class T>
  class const_mem_fun_ref_t : public unary_function<T, S>
  {
  public:
    explicit const_mem_fun_ref_t(S (T::*pf)() const) : f(pf) {}
    S operator()(const T &r) const { return (r.*f)(); }

  private:
    S(T::*f)
    () const;
  };

  template <class S, class T, class A>
  class mem_fun1_t : public binary_function<T *, A, S>
  {
  public:
    explicit mem_fun1_t(S (T::*pf)(A)) : f(pf) {}
    S operator()(T *p, A x) const { return (p->*f)(x); }

  private:
    S(T::*f)
    (A);
  };

  template <class S, class T, class A>
  class const_mem_fun1_t : public binary_function<const T *, A, S>
  {
  public:
    explicit const_mem_fun1_t(S (T::*pf)(A) const) : f(pf) {}
    S operator()(const T *p, A x) const { return (p->*f)(x); }

  private:
    S(T::*f)
    (A) const;
  };

  template <class S, class T, class A>
  class mem_fun1_ref_t : public binary_function<T, A, S>
  {
  public:
    explicit mem_fun1_ref_t(S (T::*pf)(A)) : f(pf) {}
    S operator()(T &r, A x) const { return (r.*f)(x); }

  private:
    S(T::*f)
    (A);
  };

  template <class S, class T, class A>
  class const_mem_fun1_ref_t : public binary_function<T, A, S>
  {
  public:
    explicit const_mem_fun1_ref_t(S (T::*pf)(A) const) : f(pf) {}
    S operator()(const T &r, A x) const { return (r.*f)(x); }

  private:
    S(T::*f)
    (A) const;
  };

  // Mem_fun adaptor helper functions.  There are only four:
  //  mem_fun, mem_fun_ref, mem_fun1, mem_fun1_ref.

  template <class S, class T>
  inline mem_fun_t<S, T> mem_fun(S (T::*f)())
  {
    return mem_fun_t<S, T>(f);
  }

  template <class S, class T>
  inline const_mem_fun_t<S, T> mem_fun(S (T::*f)() const)
  {
    return const_mem_fun_t<S, T>(f);
  }

  template <class S, class T>
  inline mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)())
  {
    return mem_fun_ref_t<S, T>(f);
  }

  template <class S, class T>
  inline const_mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)() const)
  {
    return const_mem_fun_ref_t<S, T>(f);
  }

  template <class S, class T, class A>
  inline mem_fun1_t<S, T, A> mem_fun1(S (T::*f)(A))
  {
    return mem_fun1_t<S, T, A>(f);
  }

  template <class S, class T, class A>
  inline const_mem_fun1_t<S, T, A> mem_fun1(S (T::*f)(A) const)
  {
    return const_mem_fun1_t<S, T, A>(f);
  }

  template <class S, class T, class A>
  inline mem_fun1_ref_t<S, T, A> mem_fun1_ref(S (T::*f)(A))
  {
    return mem_fun1_ref_t<S, T, A>(f);
  }

  template <class S, class T, class A>
  inline const_mem_fun1_ref_t<S, T, A> mem_fun1_ref(S (T::*f)(A) const)
  {
    return const_mem_fun1_ref_t<S, T, A>(f);
  }
  ```
  </details>