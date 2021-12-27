# STL标准模板库

- [STL标准模板库](#stl标准模板库)
  - [分配器```allocator```](#分配器allocator)
    - [标准分配器](#标准分配器)
    - [```SGI```特殊分配器](#sgi特殊分配器)
      - [构造和析构工具](#构造和析构工具)
      - [内存分配和释放](#内存分配和释放)
    - [内存基本处理工具](#内存基本处理工具)
  - [迭代器```iterator```](#迭代器iterator)
  - [容器 ``container``](#容器-container)
    - [序列式容器 ``sequence container``](#序列式容器-sequence-container)
      - [容器 ``array``](#容器-array)
      - [容器 ``vector``](#容器-vector)
      - [容器 ``list``](#容器-list)
      - [容器 ``forward_list``](#容器-forward_list)
      - [容器 ``deque``](#容器-deque)
    - [关联式容器 ``associative container``](#关联式容器-associative-container)
      - [容器 ``set`` ``multiset``](#容器-set-multiset)
      - [容器 ``map`` ``multimap``](#容器-map-multimap)
      - [容器 ``unordered_set`` ``unordered_multiset`` ``unordered_map`` ``unordered_multimap``](#容器-unordered_set-unordered_multiset-unordered_map-unordered_multimap)

![STL六大模块](https://github.com/gongluck/images/blob/main/STL六大模块.png)

## 分配器```allocator```

### 标准分配器

  包装```::operator new```和```::operator delete```。

  [defalloc.h](https://github.com/gongluck/sourcecode/blob/main/stl/defalloc.h)

  ```C++
  //包装::operator new
  template <class _Tp>
  inline _Tp *allocate(ptrdiff_t __size, _Tp *)
  {
      set_new_handler(0);
      _Tp *__tmp = (_Tp *)(::operator new((size_t)(__size * sizeof(_Tp))));
      if (__tmp == 0)
      {
          cerr << "out of memory" << endl;
          exit(1);
      }
      return __tmp;
  }

  //包装::operator delete
  template <class _Tp>
  inline void deallocate(_Tp *__buffer)
  {
      ::operator delete(__buffer);
  }

  template <class _Tp>
  class allocator
  {
  public:
      //下列类型定义可被萃取机使用
      typedef _Tp value_type;
      typedef _Tp *pointer;
      typedef const _Tp *const_pointer;
      typedef _Tp &reference;
      typedef const _Tp &const_reference;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;

      //内存分配函数
      pointer allocate(size_type __n)
      {
          //分配n个_Tp的内存
          return ::allocate((difference_type)__n, (pointer)0);
      }

      //内存释放函数
      void deallocate(pointer __p) { ::deallocate(__p); }
  };
  ```

### ```SGI```特殊分配器

#### 构造和析构工具

  ```construct```利用```placement new```实现。
  ```destroy```调用析构函数或者利用```萃取特性```进而调用遍历析构或者特化的版本。

  [stl_construct.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_construct.h)

  ```C++
  template <class _T1, class _T2>
  inline void _Construct(_T1 *__p, const _T2 &__value)
  {
    //placement new
    new ((void *)__p) _T1(__value);
  }

  template <class _T1>
  inline void _Construct(_T1 *__p)
  {
    //placement new
    new ((void *)__p) _T1();
  }

  template <class _Tp>
  inline void _Destroy(_Tp *__pointer)
  {
    //调用实例的析构函数
    __pointer->~_Tp();
  }

  template <class _ForwardIterator>
  void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type)
  {
    //遍历范围中的实例，逐一析构
    for (; __first != __last; ++__first)
      destroy(&*__first);
  }

  template <class _ForwardIterator>
  inline void __destroy_aux(_ForwardIterator, _ForwardIterator, __true_type) {}

  template <class _ForwardIterator, class _Tp>
  inline void
  __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp *)
  {
    //萃取trivial_destructor特性
    //trivial_destructor不重要的析构函数
    typedef typename __type_traits<_Tp>::has_trivial_destructor
        _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
  }

  template <class _ForwardIterator>
  inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
  {
    __destroy(__first, __last, __VALUE_TYPE(__first));
  }

  //析构器特化版本
  inline void _Destroy(char *, char *) {}
  inline void _Destroy(int *, int *) {}
  inline void _Destroy(long *, long *) {}
  inline void _Destroy(float *, float *) {}
  inline void _Destroy(double *, double *) {}
  #ifdef __STL_HAS_WCHAR_T
  inline void _Destroy(wchar_t *, wchar_t *)
  {
  }
  #endif /* __STL_HAS_WCHAR_T */

  // --------------------------------------------------
  // Old names from the HP STL.

  //带初始值的构造器
  template <class _T1, class _T2>
  inline void construct(_T1 *__p, const _T2 &__value)
  {
    _Construct(__p, __value);
  }

  //不带初始值的构造器
  template <class _T1>
  inline void construct(_T1 *__p)
  {
    _Construct(__p);
  }

  //析构器
  template <class _Tp>
  inline void destroy(_Tp *__pointer)
  {
    _Destroy(__pointer);
  }

  //范围析构器
  template <class _ForwardIterator>
  inline void destroy(_ForwardIterator __first, _ForwardIterator __last)
  {
    _Destroy(__first, __last);
  }
  ```

#### 内存分配和释放

  ![内存池分配器](https://github.com/gongluck/images/blob/main/内存池分配器.png)

  ```SGI```设计了双层策略。
  第一级配置器```__malloc_alloc_template```简单包装了```malloc```和```free```，并在内存分配失败时调用失败处理函数。
  ```C++
  //包装malloc和free
  template <int __inst>
  class __malloc_alloc_template
  {
  public:
    static void *allocate(size_t __n)
    {
      //使用malloc分配内存
      void *__result = malloc(__n);
      //分配内存失败时调用处理函数
      if (0 == __result)
        __result = _S_oom_malloc(__n);
      return __result;
    }

    static void deallocate(void *__p, size_t /* __n */)
    {
      //使用free释放内存
      free(__p);
    }
  };
  ```

  第二级配置器```__default_alloc_template```使用了```内存池```策略，使用```free list```实现。
  ```C++
  //内存池优化版本，现代malloc已经存在类似的内存池优化了，所以使用STL标准分配器即可。
  template <bool threads, int inst>
  class __default_alloc_template
  {
  private:
    //使用联合体合理使用内存
    __PRIVATE : union _Obj
    {
      union _Obj *_M_free_list_link;
      char _M_client_data[1]; /* The client sees this.        */
    };

  private:
  #if defined(__SUNPRO_CC) || defined(__GNUC__) || defined(__HP_aCC)
    static _Obj *__STL_VOLATILE _S_free_list[];
    // Specifying a size results in duplicate def for 4.1
  #else
    static _Obj *__STL_VOLATILE _S_free_list[_NFREELISTS];
  #endif
    //查找__bytes大小的内存块落在哪段自由链表中
    static size_t _S_freelist_index(size_t __bytes)
    {
      return (((__bytes) + (size_t)_ALIGN - 1) / (size_t)_ALIGN - 1);
    }

  public:
    //分配内存
    /* __n must be > 0      */
    static void *allocate(size_t __n)
    {
      void *__ret = 0;

      //大内存使用malloc分配
      if (__n > (size_t)_MAX_BYTES)
      {
        __ret = malloc_alloc::allocate(__n);
      }
      else
      {
        //查找内存块落在哪段自由链表中
        _Obj *__STL_VOLATILE *__my_free_list = _S_free_list + _S_freelist_index(__n);
        // Acquire the lock here with a constructor call.
        // This ensures that it is released in exit or during stack
        // unwinding.

        //获取自由链表的首块
        _Obj *__RESTRICT __result = *__my_free_list;
        if (__result == 0)
          //增加内存池容量
          __ret = _S_refill(_S_round_up(__n));
        else
        {
          //自由链表的首块指向下一个可用块
          *__my_free_list = __result->_M_free_list_link;
          __ret = __result;
        }
      }

      return __ret;
    };

    //释放内存
    /* __p may not be 0 */
    static void deallocate(void *__p, size_t __n)
    {
      //大内存使用free释放
      if (__n > (size_t)_MAX_BYTES)
        malloc_alloc::deallocate(__p, __n);
      else
      {
        //查找内存块落在哪段自由链表中
        _Obj *__STL_VOLATILE *__my_free_list = _S_free_list + _S_freelist_index(__n);
        _Obj *__q = (_Obj *)__p;

        //头插法将内存块放入自由链表
        __q->_M_free_list_link = *__my_free_list;
        *__my_free_list = __q;
        // lock is released here
      }
    }
  };
  ```

  [stl_alloc.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_alloc.h)

### 内存基本处理工具

  ```uninitialized_copy```、```uninitialized_fill```和```uninitialized_fill_n```在目标内存调用拷贝构造函数或者内存拷贝函数。

  ```C++
  // uninitialized_copy

  // Valid if copy construction is equivalent to assignment, and if the
  //  destructor is trivial.
  template <class _InputIter, class _ForwardIter>
  inline _ForwardIter
  __uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                          _ForwardIter __result,
                          __true_type)
  {
    return copy(__first, __last, __result);
  }

  template <class _InputIter, class _ForwardIter>
  _ForwardIter
  __uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                          _ForwardIter __result,
                          __false_type)
  {
    _ForwardIter __cur = __result;
    __STL_TRY
    {
      //非标量类型，逐一构造
      for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
      return __cur;
    }
    __STL_UNWIND(_Destroy(__result, __cur));
  }

  //泛化版本
  template <class _InputIter, class _ForwardIter, class _Tp>
  inline _ForwardIter
  __uninitialized_copy(_InputIter __first, _InputIter __last,
                      _ForwardIter __result, _Tp *)
  {
    //萃取是否标量类型
    typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
    return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
  }

  //uninitialized_copy将[first,last)的数据拷贝到[result,result+last-first)
  template <class _InputIter, class _ForwardIter>
  inline _ForwardIter
  uninitialized_copy(_InputIter __first, _InputIter __last,
                    _ForwardIter __result)
  {
    //__VALUE_TYPE萃取值类型
    return __uninitialized_copy(__first, __last, __result,
                                __VALUE_TYPE(__result));
  }

  //char*特化版本
  inline char *uninitialized_copy(const char *__first, const char *__last,
                                  char *__result)
  {
    memmove(__result, __first, __last - __first);
    return __result + (__last - __first);
  }

  //wchar_t*特化版本
  inline wchar_t *
  uninitialized_copy(const wchar_t *__first, const wchar_t *__last,
                    wchar_t *__result)
  {
    memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
    return __result + (__last - __first);
  }
  ```
  ```C++
  // uninitialized_fill

  // Valid if copy construction is equivalent to assignment, and if the
  // destructor is trivial.
  template <class _ForwardIter, class _Tp>
  inline void
  __uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
                          const _Tp &__x, __true_type)
  {
    fill(__first, __last, __x);
  }

  template <class _ForwardIter, class _Tp>
  void __uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
                                const _Tp &__x, __false_type)
  {
    _ForwardIter __cur = __first;
    __STL_TRY
    {
      //非标量类型，逐一构造
      for (; __cur != __last; ++__cur)
        _Construct(&*__cur, __x);
    }
    __STL_UNWIND(_Destroy(__first, __cur));
  }

  template <class _ForwardIter, class _Tp, class _Tp1>
  inline void __uninitialized_fill(_ForwardIter __first,
                                  _ForwardIter __last, const _Tp &__x, _Tp1 *)
  {
    //萃取是否标量类型
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    __uninitialized_fill_aux(__first, __last, __x, _Is_POD());
  }

  //将[first,last)区间的数据置为x
  template <class _ForwardIter, class _Tp>
  inline void uninitialized_fill(_ForwardIter __first,
                                _ForwardIter __last,
                                const _Tp &__x)
  {
    //__VALUE_TYPE萃取值类型
    __uninitialized_fill(__first, __last, __x, __VALUE_TYPE(__first));
  }
  ```
  ```C++
  //uninitialized_fill_n

  // Valid if copy construction is equivalent to assignment, and if the
  //  destructor is trivial.
  template <class _ForwardIter, class _Size, class _Tp>
  inline _ForwardIter
  __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                            const _Tp &__x, __true_type)
  {
    return fill_n(__first, __n, __x);
  }

  template <class _ForwardIter, class _Size, class _Tp>
  _ForwardIter
  __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                            const _Tp &__x, __false_type)
  {
    _ForwardIter __cur = __first;
    __STL_TRY
    {
      //非标量类型，逐一构造
      for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
      return __cur;
    }
    __STL_UNWIND(_Destroy(__first, __cur));
  }

  template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
  inline _ForwardIter
  __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp &__x, _Tp1 *)
  {
    //萃取是否标量类型
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
  }

  //将[first,first+size)区间的数据置为x
  template <class _ForwardIter, class _Size, class _Tp>
  inline _ForwardIter
  uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp &__x)
  {
    //__VALUE_TYPE萃取值类型
    return __uninitialized_fill_n(__first, __n, __x, __VALUE_TYPE(__first));
  }
  ```

  [stl_uninitialized.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_uninitialized.h)

## 迭代器```iterator```

  迭代器```iterator```提供了对```容器```元素的访问方法，实现桥接容器和```算法```的方式。
  迭代器一般由容器自生内部实现，以达到隐藏容器细节的作用。

  [stl_iterator_base.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator_base.h)

  ```C++
  //可输入迭代器型别
  struct input_iterator_tag
  {
  };
  //可输出迭代器型别
  struct output_iterator_tag
  {
  };
  //可单向迭代器型别
  struct forward_iterator_tag : public input_iterator_tag
  {
  };
  //可双向迭代器型别
  struct bidirectional_iterator_tag : public forward_iterator_tag
  {
  };
  //可随机访问迭代器型别
  struct random_access_iterator_tag : public bidirectional_iterator_tag
  {
  };

  // The base classes input_iterator, output_iterator, forward_iterator,
  // bidirectional_iterator, and random_access_iterator are not part of
  // the C++ standard.  (They have been replaced by struct iterator.)
  // They are included for backward compatibility with the HP STL.

  template <class _Tp, class _Distance>
  struct input_iterator
  {
    typedef input_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
  };

  struct output_iterator
  {
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;
  };

  template <class _Tp, class _Distance>
  struct forward_iterator
  {
    typedef forward_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
  };

  template <class _Tp, class _Distance>
  struct bidirectional_iterator
  {
    typedef bidirectional_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
  };

  template <class _Tp, class _Distance>
  struct random_access_iterator
  {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
  };

  #ifdef __STL_USE_NAMESPACES
  template <class _Category, class _Tp, class _Distance = ptrdiff_t,
            class _Pointer = _Tp *, class _Reference = _Tp &>
  struct iterator
  {
    typedef _Category iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Pointer pointer;
    typedef _Reference reference;
  };
  #endif /* __STL_USE_NAMESPACES */

  #ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

  //迭代器型别定义萃取
  template <class _Iterator>
  struct iterator_traits
  {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type value_type;
    typedef typename _Iterator::difference_type difference_type;
    typedef typename _Iterator::pointer pointer;
    typedef typename _Iterator::reference reference;
  };

  //指针型特化版本
  template <class _Tp>
  struct iterator_traits<_Tp *>
  {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
  };

  //常量指针型特化版本
  template <class _Tp>
  struct iterator_traits<const _Tp *>
  {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef const _Tp *pointer;
    typedef const _Tp &reference;
  };

  // The overloaded functions iterator_category, distance_type, and
  // value_type are not part of the C++ standard.  (They have been
  // replaced by struct iterator_traits.)  They are included for
  // backward compatibility with the HP STL.

  // We introduce internal names for these functions.

  //萃取型别定义 const型特化
  template <class _Iter>
  inline typename iterator_traits<_Iter>::iterator_category
  __iterator_category(const _Iter &)
  {
    typedef typename iterator_traits<_Iter>::iterator_category _Category;
    return _Category();
  }

  template <class _Iter>
  inline typename iterator_traits<_Iter>::difference_type *
  __distance_type(const _Iter &)
  {
    return static_cast<typename iterator_traits<_Iter>::difference_type *>(0);
  }

  template <class _Iter>
  inline typename iterator_traits<_Iter>::value_type *
  __value_type(const _Iter &)
  {
    return static_cast<typename iterator_traits<_Iter>::value_type *>(0);
  }

  //萃取型别定义
  template <class _Iter>
  inline typename iterator_traits<_Iter>::iterator_category
  iterator_category(const _Iter &__i) { return __iterator_category(__i); }

  template <class _Iter>
  inline typename iterator_traits<_Iter>::difference_type *
  distance_type(const _Iter &__i) { return __distance_type(__i); }

  template <class _Iter>
  inline typename iterator_traits<_Iter>::value_type *
  value_type(const _Iter &__i) { return __value_type(__i); }

  #define __ITERATOR_CATEGORY(__i) __iterator_category(__i)
  #define __DISTANCE_TYPE(__i) __distance_type(__i)
  #define __VALUE_TYPE(__i) __value_type(__i)

  #else /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  //萃取型别定义 input_iterator型特化
  template <class _Tp, class _Distance>
  inline input_iterator_tag
  iterator_category(const input_iterator<_Tp, _Distance> &)
  {
    return input_iterator_tag();
  }

  //萃取型别定义 output_iterator型特化
  inline output_iterator_tag iterator_category(const output_iterator &)
  {
    return output_iterator_tag();
  }

  //萃取型别定义 forward_iterator型特化
  template <class _Tp, class _Distance>
  inline forward_iterator_tag
  iterator_category(const forward_iterator<_Tp, _Distance> &)
  {
    return forward_iterator_tag();
  }

  //萃取型别定义 bidirectional_iterator型特化
  template <class _Tp, class _Distance>
  inline bidirectional_iterator_tag
  iterator_category(const bidirectional_iterator<_Tp, _Distance> &)
  {
    return bidirectional_iterator_tag();
  }

  //萃取型别定义 random_access_iterator型特化
  template <class _Tp, class _Distance>
  inline random_access_iterator_tag
  iterator_category(const random_access_iterator<_Tp, _Distance> &)
  {
    return random_access_iterator_tag();
  }

  //萃取型别定义 常量指针型特化
  template <class _Tp>
  inline random_access_iterator_tag iterator_category(const _Tp *)
  {
    return random_access_iterator_tag();
  }
  ```

  [stl_iterator.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator.h)

## 容器 ``container``

### 序列式容器 ``sequence container``

#### 容器 ``array``

  ![容器array](https://github.com/gongluck/images/blob/main/容器array.png)

  对原 ``C`` 数组的封装。

  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/array" />

#### 容器 ``vector``

  ![容器vector](https://github.com/gongluck/images/blob/main/容器vector.png)

  根据不同的实现， ``vector`` 数据成员至少包含 ``3`` 个 ``T*`` 类型的指针。分别为 ``start`` 、 ``finish`` 、 ``end_of_storage`` 。``vector::iterator`` 数据成员至少包含 ``1`` 个 ``T*`` 类型的指针。

  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/stl_vector.h" />

#### 容器 ``list``

  ![容器list](https://github.com/gongluck/images/blob/main/容器list.png)

  ``node`` 节点由前后指针和数据成员 ``T`` 组成。
  ``list`` 包含一个 ``node`` 指针指向链表头节点。
  ``list::iterator`` 包含一个 ``node`` 指针指向链表节点。

  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/stl_list.h" />

#### 容器 ``forward_list``

  ![容器forward_list](https://github.com/gongluck/images/blob/main/容器forward_list.png)

  ``node`` 节点由后指针和数据成员 ``T`` 组成。
  ``forward_list`` 包含一个 ``node`` 指针指向单向链表头节点。
  ``forward_list::iterator`` 包含一个 ``node`` 指针指向链表节点。

  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/forward_list.h" />

#### 容器 ``deque``

  ![容器deque](https://github.com/gongluck/images/blob/main/容器deque.png)

  ``deque`` 包含指向管控中心的指针 ``m_map`` 、控制中心的大小和分别指向控制中心开始和结束的两个迭代器。
  ``deque::iterator`` 包含 ``3`` 个 ``T*`` 类型的指针和 ``1`` 个 ``T**`` 类型指针。分别为 ``cur`` 、 ``first`` 、 ``last`` 指向直接内存的位置，``m_node`` 指向管控中心的节点位置。

  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/stl_deque.h" />

### 关联式容器 ``associative container``

#### 容器 ``set`` ``multiset``

  ``set`` 和 ``multiset`` 底层使用红黑树实现，``key == value``。

  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/stl_set.h" />
  <br/>
  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/stl_multiset.h" />

#### 容器 ``map`` ``multimap``

  ``map`` 和 ``multimap`` 底层使用红黑树实现，``key != value``。

  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/stl_map.h" />
  <br/>
  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/stl_multimap.h" />

#### 容器 ``unordered_set`` ``unordered_multiset`` ``unordered_map`` ``unordered_multimap``

  底层用哈希表实现的无序集合。

  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/hashtable.h" />
  <br/>
  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/unordered_set.h" />
  <br/>
  <iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/unordered_map.h" />
