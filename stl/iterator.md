# 迭代器```iterator```

- [迭代器```iterator```](#迭代器iterator)

  迭代器```iterator```提供了对```容器```元素的访问方法，实现桥接容器和```算法```的方式。
  迭代器一般由容器自生内部实现，以达到隐藏容器细节的作用。

  [stl_iterator.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator.h)

  <details>
  <summary>iterator</summary>

  ```c++
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
    //输出迭代器下面的类型都是void
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

  //萃取机
  template <class Iterator>
  struct iterator_traits
  {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
  };

  template <class T>
  struct iterator_traits<T *>
  {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;
  };

  template <class T>
  struct iterator_traits<const T *>
  {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T *pointer;
    typedef const T &reference;
  };

  template <class Iterator>
  inline typename iterator_traits<Iterator>::iterator_category
  iterator_category(const Iterator &)
  {
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
  }

  template <class Iterator>
  inline typename iterator_traits<Iterator>::difference_type *
  distance_type(const Iterator &)
  {
    return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
  }

  template <class Iterator>
  inline typename iterator_traits<Iterator>::value_type *
  value_type(const Iterator &)
  {
    return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
  }

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
  ```
  </details>
