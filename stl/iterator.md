# 迭代器```iterator```

- [迭代器```iterator```](#迭代器iterator)

  迭代器```iterator```提供了对```容器```元素的访问方法，实现桥接容器和```算法```的方式。
  迭代器一般由容器自生内部实现，以达到隐藏容器细节的作用。

  [stl_iterator.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator.h)

  <details>
  <summary>stl_iterator</summary>

  ```C++
  //迭代器类型定义
  struct input_iterator_tag
  {
  };
  struct output_iterator_tag
  {
  };
  struct forward_iterator_tag : public input_iterator_tag
  {
  };
  struct bidirectional_iterator_tag : public forward_iterator_tag
  {
  };
  struct random_access_iterator_tag : public bidirectional_iterator_tag
  {
  };

  template <class T, class Distance>
  struct input_iterator
  {
    typedef input_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
  };

  struct output_iterator
  {
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;
  };

  template <class T, class Distance>
  struct forward_iterator
  {
    typedef forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
  };

  template <class T, class Distance>
  struct bidirectional_iterator
  {
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
  };

  template <class T, class Distance>
  struct random_access_iterator
  {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
  };

  #ifdef __STL_USE_NAMESPACES
  template <class Category, class T, class Distance = ptrdiff_t,
            class Pointer = T *, class Reference = T &>
  struct iterator
  {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
  };
  #endif /* __STL_USE_NAMESPACES */

  template <class T, class Distance>
  inline input_iterator_tag
  iterator_category(const input_iterator<T, Distance> &)
  {
    return input_iterator_tag();
  }

  inline output_iterator_tag iterator_category(const output_iterator &)
  {
    return output_iterator_tag();
  }

  template <class T, class Distance>
  inline forward_iterator_tag
  iterator_category(const forward_iterator<T, Distance> &)
  {
    return forward_iterator_tag();
  }

  template <class T, class Distance>
  inline bidirectional_iterator_tag
  iterator_category(const bidirectional_iterator<T, Distance> &)
  {
    return bidirectional_iterator_tag();
  }

  template <class T, class Distance>
  inline random_access_iterator_tag
  iterator_category(const random_access_iterator<T, Distance> &)
  {
    return random_access_iterator_tag();
  }

  template <class T>
  inline random_access_iterator_tag iterator_category(const T *)
  {
    return random_access_iterator_tag();
  }

  //迭代器value_type返回T*
  template <class T, class Distance>
  inline T *value_type(const input_iterator<T, Distance> &)
  {
    return (T *)(0);
  }

  template <class T, class Distance>
  inline T *value_type(const forward_iterator<T, Distance> &)
  {
    return (T *)(0);
  }

  template <class T, class Distance>
  inline T *value_type(const bidirectional_iterator<T, Distance> &)
  {
    return (T *)(0);
  }

  template <class T, class Distance>
  inline T *value_type(const random_access_iterator<T, Distance> &)
  {
    return (T *)(0);
  }

  template <class T>
  inline T *value_type(const T *) { return (T *)(0); }

  //迭代器distance_type返回Distance*
  template <class T, class Distance>
  inline Distance *distance_type(const input_iterator<T, Distance> &)
  {
    return (Distance *)(0);
  }

  template <class T, class Distance>
  inline Distance *distance_type(const forward_iterator<T, Distance> &)
  {
    return (Distance *)(0);
  }

  template <class T, class Distance>
  inline Distance *
  distance_type(const bidirectional_iterator<T, Distance> &)
  {
    return (Distance *)(0);
  }

  template <class T, class Distance>
  inline Distance *
  distance_type(const random_access_iterator<T, Distance> &)
  {
    return (Distance *)(0);
  }

  template <class T>
  inline ptrdiff_t *distance_type(const T *) { return (ptrdiff_t *)(0); }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  //输入迭代器distance计算 遍历
  template <class InputIterator, class Distance>
  inline void __distance(InputIterator first, InputIterator last, Distance &n,
                        input_iterator_tag)
  {
    while (first != last)
    {
      ++first;
      ++n;
    }
  }

  //随机迭代器distance计算 operator-
  template <class RandomAccessIterator, class Distance>
  inline void __distance(RandomAccessIterator first, RandomAccessIterator last,
                        Distance &n, random_access_iterator_tag)
  {
    n += last - first;
  }

  // distance接口
  template <class InputIterator, class Distance>
  inline void distance(InputIterator first, InputIterator last, Distance &n)
  {
    // InputIterator是模板参数，实际可用输入迭代器和随机迭代器类型匹配上面重载的__distance方法
    __distance(first, last, n, iterator_category(first));
  }

  //输入迭代器前进 遍历
  template <class InputIterator, class Distance>
  inline void __advance(InputIterator &i, Distance n, input_iterator_tag)
  {
    while (n--)
      ++i;
  }

  #if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
  #pragma set woff 1183
  #endif

  //双向迭代器前进或者后退 遍历
  template <class BidirectionalIterator, class Distance>
  inline void __advance(BidirectionalIterator &i, Distance n,
                        bidirectional_iterator_tag)
  {
    if (n >= 0)
      while (n--)
        ++i;
    else
      while (n++)
        --i;
  }

  #if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
  #pragma reset woff 1183
  #endif

  //随机迭代器前进 operator+
  template <class RandomAccessIterator, class Distance>
  inline void __advance(RandomAccessIterator &i, Distance n,
                        random_access_iterator_tag)
  {
    i += n;
  }

  //迭代器前进接口
  template <class InputIterator, class Distance>
  inline void advance(InputIterator &i, Distance n)
  {
    // InputIterator是模板参数，实际可用输入迭代器、双向迭代器和随机迭代器类型匹配上面重载的__advance方法
    __advance(i, n, iterator_category(i));
  }

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

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  //尾部插入迭代器属于输出迭代器类别
  template <class Container>
  inline output_iterator_tag
  iterator_category(const back_insert_iterator<Container> &)
  {
    return output_iterator_tag();
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

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

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  //头部插入迭代器属于输出迭代器类别
  template <class Container>
  inline output_iterator_tag
  iterator_category(const front_insert_iterator<Container> &)
  {
    return output_iterator_tag();
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

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

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  //插入迭代器属于输出迭代器类别
  template <class Container>
  inline output_iterator_tag
  iterator_category(const insert_iterator<Container> &)
  {
    return output_iterator_tag();
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  //生成插入迭代器
  template <class Container, class Iterator>
  inline insert_iterator<Container> inserter(Container &x, Iterator i)
  {
    typedef typename Container::iterator iter;
    return insert_iterator<Container>(x, iter(i));
  }

  #ifndef __STL_LIMITED_DEFAULT_TEMPLATES
  template <class BidirectionalIterator, class T, class Reference = T &,
            class Distance = ptrdiff_t>
  #else
  template <class BidirectionalIterator, class T, class Reference,
            class Distance>
  #endif
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

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  //反向双向迭代器属于双向迭代器类别
  template <class BidirectionalIterator, class T, class Reference,
            class Distance>
  inline bidirectional_iterator_tag
  iterator_category(const reverse_bidirectional_iterator<BidirectionalIterator,
                                                        T,
                                                        Reference, Distance> &)
  {
    return bidirectional_iterator_tag();
  }

  template <class BidirectionalIterator, class T, class Reference,
            class Distance>
  inline T *
  value_type(const reverse_bidirectional_iterator<BidirectionalIterator, T,
                                                  Reference, Distance> &)
  {
    return (T *)0;
  }

  template <class BidirectionalIterator, class T, class Reference,
            class Distance>
  inline Distance *
  distance_type(const reverse_bidirectional_iterator<BidirectionalIterator, T,
                                                    Reference, Distance> &)
  {
    return (Distance *)0;
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

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

  // This is the old version of reverse_iterator, as found in the original
  //  HP STL.  It does not use partial specialization.

  #ifndef __STL_LIMITED_DEFAULT_TEMPLATES
  template <class RandomAccessIterator, class T, class Reference = T &,
            class Distance = ptrdiff_t>
  #else
  template <class RandomAccessIterator, class T, class Reference,
            class Distance>
  #endif
  //反向迭代器
  class reverse_iterator
  {
    typedef reverse_iterator<RandomAccessIterator, T, Reference, Distance>
        self;

  protected:
    RandomAccessIterator current; //当前位置迭代器

  public:
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef Reference reference;

    reverse_iterator() {}
    explicit reverse_iterator(RandomAccessIterator x) : current(x) {}
    RandomAccessIterator base() const { return current; }
    //解引用操作返回前一个位置的迭代器的解引用
    Reference operator*() const { return *(current - 1); }
  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    //成员引用运算符返回解引用操作的结果的地址
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
    //随机迭代器能跳跃步长
    self operator+(Distance n) const
    {
      return self(current - n);
    }
    self &operator+=(Distance n)
    {
      current -= n;
      return *this;
    }
    self operator-(Distance n) const
    {
      return self(current + n);
    }
    self &operator-=(Distance n)
    {
      current += n;
      return *this;
    }
    //随机迭代器下标运算符
    Reference operator[](Distance n) const { return *(*this + n); }
  };

  //反向迭代器属于随机迭代器类别
  template <class RandomAccessIterator, class T, class Reference, class Distance>
  inline random_access_iterator_tag
  iterator_category(const reverse_iterator<RandomAccessIterator, T,
                                          Reference, Distance> &)
  {
    return random_access_iterator_tag();
  }

  template <class RandomAccessIterator, class T, class Reference, class Distance>
  inline T *value_type(const reverse_iterator<RandomAccessIterator, T,
                                              Reference, Distance> &)
  {
    return (T *)0;
  }

  template <class RandomAccessIterator, class T, class Reference, class Distance>
  inline Distance *distance_type(const reverse_iterator<RandomAccessIterator, T,
                                                        Reference, Distance> &)
  {
    return (Distance *)0;
  }

  template <class RandomAccessIterator, class T, class Reference, class Distance>
  inline bool operator==(const reverse_iterator<RandomAccessIterator, T,
                                                Reference, Distance> &x,
                        const reverse_iterator<RandomAccessIterator, T,
                                                Reference, Distance> &y)
  {
    return x.base() == y.base();
  }

  template <class RandomAccessIterator, class T, class Reference, class Distance>
  inline bool operator<(const reverse_iterator<RandomAccessIterator, T,
                                              Reference, Distance> &x,
                        const reverse_iterator<RandomAccessIterator, T,
                                              Reference, Distance> &y)
  {
    return y.base() < x.base();
  }

  template <class RandomAccessIterator, class T, class Reference, class Distance>
  inline Distance operator-(const reverse_iterator<RandomAccessIterator, T,
                                                  Reference, Distance> &x,
                            const reverse_iterator<RandomAccessIterator, T,
                                                  Reference, Distance> &y)
  {
    return y.base() - x.base();
  }

  template <class RandomAccessIter, class T, class Ref, class Dist>
  inline reverse_iterator<RandomAccessIter, T, Ref, Dist>
  operator+(Dist n, const reverse_iterator<RandomAccessIter, T, Ref, Dist> &x)
  {
    return reverse_iterator<RandomAccessIter, T, Ref, Dist>(x.base() - n);
  }

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

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  //输入流迭代器属于输入迭代器类别
  template <class T, class Distance>
  inline input_iterator_tag
  iterator_category(const istream_iterator<T, Distance> &)
  {
    return input_iterator_tag();
  }

  template <class T, class Distance>
  inline T *value_type(const istream_iterator<T, Distance> &) { return (T *)0; }

  template <class T, class Distance>
  inline Distance *distance_type(const istream_iterator<T, Distance> &)
  {
    return (Distance *)0;
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

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

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  //流输出迭代器属于输出迭代器
  template <class T>
  inline output_iterator_tag
  iterator_category(const ostream_iterator<T> &)
  {
    return output_iterator_tag();
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */
  ```
  </details>
