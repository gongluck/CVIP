# STL标准模板库

- [STL标准模板库](#stl标准模板库)
  - [分配器```allocator```](#分配器allocator)
    - [标准分配器](#标准分配器)
    - [```SGI```特殊分配器](#sgi特殊分配器)
      - [构造和析构工具](#构造和析构工具)
      - [内存分配和释放](#内存分配和释放)
    - [内存基本处理工具](#内存基本处理工具)
  - [迭代器```iterator```](#迭代器iterator)
  - [容器```container```](#容器container)
    - [序列式容器```sequence container```](#序列式容器sequence-container)
      - [容器 ``array``](#容器-array)
      - [容器```vector```](#容器vector)
      - [容器```list```](#容器list)
      - [容器 ``forward_list``](#容器-forward_list)
      - [容器 ``deque``](#容器-deque)
    - [关联式容器 ``associative container``](#关联式容器-associative-container)
      - [容器 ``set`` ``multiset``](#容器-set-multiset)
      - [容器 ``map`` ``multimap``](#容器-map-multimap)
      - [容器 ``unordered_set`` ``unordered_multiset`` ``unordered_map`` ``unordered_multimap``](#容器-unordered_set-unordered_multiset-unordered_map-unordered_multimap)

![STL六大模块](https://github.com/gongluck/images/blob/main/stl/STL六大模块.png)

## 分配器```allocator```

### 标准分配器

  包装```::operator new```和```::operator delete```。

  [defalloc.h](https://github.com/gongluck/sourcecode/blob/main/stl/defalloc.h)

  <details>
  <summary>标准分配器</summary>

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
  </details>

### ```SGI```特殊分配器

#### 构造和析构工具

  ```construct```利用```placement new```实现。
  ```destroy```调用析构函数或者利用```萃取特性```进而调用遍历析构或者特化的版本。

  [stl_construct.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_construct.h)

  <details>
  <summary>构造和析构工具</summary>

  ```C++
  template <class _T1, class _T2>
  inline void _Construct(_T1 *__p, const _T2 &__value)
  {
    // placement new
    new ((void *)__p) _T1(__value);
  }

  template <class _T1>
  inline void _Construct(_T1 *__p)
  {
    // placement new
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
    // trivial_destructor不重要的析构函数
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
  </details>

#### 内存分配和释放

  ![内存池分配器](https://github.com/gongluck/images/blob/main/stl/内存池分配器.png)

  [stl_alloc.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_alloc.h)

  ```SGI```设计了双层策略。
  第一级配置器```__malloc_alloc_template```简单包装了```malloc```和```free```，并在内存分配失败时调用失败处理函数。

  <details>
  <summary>第一级配置器</summary>

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

    static void *reallocate(void *__p, size_t /* old_sz */, size_t __new_sz)
    {
      void *__result = realloc(__p, __new_sz);
      if (0 == __result)
        __result = _S_oom_realloc(__p, __new_sz);
      return __result;
    }

    static void (*__set_malloc_handler(void (*__f)()))()
    {
      void (*__old)() = __malloc_alloc_oom_handler;
      __malloc_alloc_oom_handler = __f;
      return (__old);
    }
  };
  ```
  </details>

  第二级配置器```__default_alloc_template```使用了```内存池```策略，使用```free list```实现。

  <details>
  <summary>第二级配置器</summary>

  ```C++
  //内存池优化版本，现代malloc已经存在类似的内存池优化了，所以使用STL标准分配器即可。
  template <bool threads, int inst>
  class __default_alloc_template
  {

  private:
    // Really we should use static const int x = N
    // instead of enum { x = N }, but few compilers accept the former.
  #if !(defined(__SUNPRO_CC) || defined(__GNUC__))
    enum
    {
      _ALIGN = 8
    };
    enum
    {
      _MAX_BYTES = 128
    };
    enum
    {
      _NFREELISTS = 16
    }; // _MAX_BYTES/_ALIGN
  #endif
    //对输入边界对齐到8的倍数
    static size_t
    _S_round_up(size_t __bytes)
    {
      return (((__bytes) + (size_t)_ALIGN - 1) & ~((size_t)_ALIGN - 1));
    }

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

    // Returns an object of size __n, and optionally adds to size __n free list.
    static void *_S_refill(size_t __n);
    // Allocates a chunk for nobjs of size size.  nobjs may be reduced
    // if it is inconvenient to allocate the requested number.
    static char *_S_chunk_alloc(size_t __size, int &__nobjs);

    // Chunk allocation state.
    static char *_S_start_free;
    static char *_S_end_free;
    static size_t _S_heap_size;

  #ifdef __STL_THREADS
    static _STL_mutex_lock _S_node_allocator_lock;
  #endif

    // It would be nice to use _STL_auto_lock here.  But we
    // don't need the NULL check.  And we do need a test whether
    // threads have actually been started.
    class _Lock;
    friend class _Lock;
    class _Lock
    {
    public:
      _Lock() { __NODE_ALLOCATOR_LOCK; }
      ~_Lock() { __NODE_ALLOCATOR_UNLOCK; }
    };

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
  #ifndef _NOTHREADS
        /*REFERENCED*/
        _Lock __lock_instance;
  #endif
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

        // acquire lock
  #ifndef _NOTHREADS
        /*REFERENCED*/
        _Lock __lock_instance;
  #endif /* _NOTHREADS */
        //头插法将内存块放入自由链表
        __q->_M_free_list_link = *__my_free_list;
        *__my_free_list = __q;
        // lock is released here
      }
    }

    static void *reallocate(void *__p, size_t __old_sz, size_t __new_sz);
  };
  ```
  </details>

  <details>
  <summary>封装分配器</summary>

  ```C++
  //对构造器简单封装，以元素字节大小为单位分配内存
  template <class _Tp, class _Alloc>
  class simple_alloc
  {

  public:
    static _Tp *allocate(size_t __n)
    {
      return 0 == __n ? 0 : (_Tp *)_Alloc::allocate(__n * sizeof(_Tp));
    }
    static _Tp *allocate(void)
    {
      return (_Tp *)_Alloc::allocate(sizeof(_Tp));
    }
    static void deallocate(_Tp *__p, size_t __n)
    {
      if (0 != __n)
        _Alloc::deallocate(__p, __n * sizeof(_Tp));
    }
    static void deallocate(_Tp *__p)
    {
      _Alloc::deallocate(__p, sizeof(_Tp));
    }
  };
  ```
  </details>

### 内存基本处理工具

  ```uninitialized_copy```、```uninitialized_fill```和```uninitialized_fill_n```在目标内存调用拷贝构造函数或者内存拷贝函数。

  [stl_uninitialized.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_uninitialized.h)

  <details>
  <summary>uninitialized_copy</summary>

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
  </details>

  <details>
  <summary>uninitialized_fill</summary>

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
  </details>

  <details>
  <summary>uninitialized_fill_n</summary>

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
  </details>

## 迭代器```iterator```

  迭代器```iterator```提供了对```容器```元素的访问方法，实现桥接容器和```算法```的方式。
  迭代器一般由容器自生内部实现，以达到隐藏容器细节的作用。

  [stl_iterator_base.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator_base.h)

  <details>
  <summary>迭代器基本定义</summary>

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

  template <class _Tp, class _Distance>
  inline _Tp *value_type(const input_iterator<_Tp, _Distance> &)
  {
    return (_Tp *)(0);
  }

  template <class _Tp, class _Distance>
  inline _Tp *value_type(const forward_iterator<_Tp, _Distance> &)
  {
    return (_Tp *)(0);
  }

  template <class _Tp, class _Distance>
  inline _Tp *value_type(const bidirectional_iterator<_Tp, _Distance> &)
  {
    return (_Tp *)(0);
  }

  template <class _Tp, class _Distance>
  inline _Tp *value_type(const random_access_iterator<_Tp, _Distance> &)
  {
    return (_Tp *)(0);
  }

  template <class _Tp>
  inline _Tp *value_type(const _Tp *) { return (_Tp *)(0); }

  template <class _Tp, class _Distance>
  inline _Distance *distance_type(const input_iterator<_Tp, _Distance> &)
  {
    return (_Distance *)(0);
  }

  template <class _Tp, class _Distance>
  inline _Distance *distance_type(const forward_iterator<_Tp, _Distance> &)
  {
    return (_Distance *)(0);
  }

  template <class _Tp, class _Distance>
  inline _Distance *
  distance_type(const bidirectional_iterator<_Tp, _Distance> &)
  {
    return (_Distance *)(0);
  }

  template <class _Tp, class _Distance>
  inline _Distance *
  distance_type(const random_access_iterator<_Tp, _Distance> &)
  {
    return (_Distance *)(0);
  }

  template <class _Tp>
  inline ptrdiff_t *distance_type(const _Tp *) { return (ptrdiff_t *)(0); }

  // Without partial specialization we can't use iterator_traits, so
  // we must keep the old iterator query functions around.

  #define __ITERATOR_CATEGORY(__i) iterator_category(__i)
  #define __DISTANCE_TYPE(__i) distance_type(__i)
  #define __VALUE_TYPE(__i) value_type(__i)

  template <class _InputIterator, class _Distance>
  inline void __distance(_InputIterator __first, _InputIterator __last,
                        _Distance &__n, input_iterator_tag)
  {
    while (__first != __last)
    {
      ++__first;
      ++__n;
    }
  }

  template <class _RandomAccessIterator, class _Distance>
  inline void __distance(_RandomAccessIterator __first,
                        _RandomAccessIterator __last,
                        _Distance &__n, random_access_iterator_tag)
  {
    __STL_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
    __n += __last - __first;
  }

  template <class _InputIterator, class _Distance>
  inline void distance(_InputIterator __first,
                      _InputIterator __last, _Distance &__n)
  {
    __STL_REQUIRES(_InputIterator, _InputIterator);
    __distance(__first, __last, __n, iterator_category(__first));
  }

  template <class _InputIter, class _Distance>
  inline void __advance(_InputIter &__i, _Distance __n, input_iterator_tag)
  {
    while (__n--)
      ++__i;
  }

  #if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
  #pragma set woff 1183
  #endif

  template <class _BidirectionalIterator, class _Distance>
  inline void __advance(_BidirectionalIterator &__i, _Distance __n,
                        bidirectional_iterator_tag)
  {
    __STL_REQUIRES(_BidirectionalIterator, _BidirectionalIterator);
    if (__n >= 0)
      while (__n--)
        ++__i;
    else
      while (__n++)
        --__i;
  }

  template <class _RandomAccessIterator, class _Distance>
  inline void __advance(_RandomAccessIterator &__i, _Distance __n,
                        random_access_iterator_tag)
  {
    __STL_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
    __i += __n;
  }

  template <class _InputIterator, class _Distance>
  inline void advance(_InputIterator &__i, _Distance __n)
  {
    __STL_REQUIRES(_InputIterator, _InputIterator);
    __advance(__i, __n, iterator_category(__i));
  }
  ```
  </details>

  [stl_iterator.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator.h)

## 容器```container```

### 序列式容器```sequence container```

#### 容器 ``array``

  ![容器array](https://github.com/gongluck/images/blob/main/容器array.png)

  对原 ``C`` 数组的封装。

  [array](https://github.com/gongluck/sourcecode/blob/main/stl/array)

#### 容器```vector```

  ![容器vector](https://github.com/gongluck/images/blob/main/容器vector.png)

  根据不同的实现，```vector```数据成员至少包含```3```个```T*```类型的指针。分别为```start```、```finish```、```end_of_storage```。```vector::iterator```数据成员至少包含```1```个```T*```类型的指针。

  [stl_vector.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_vector.h)

  <details>
  <summary>vector</summary>

  ```C++
  // vector_base处理内存的分配和析构
  template <class _Tp, class _Alloc>
  class _Vector_base {
  public:
    typedef _Alloc allocator_type;
    //获取分配器
    allocator_type get_allocator() const { return allocator_type(); }

    _Vector_base(const _Alloc &)
        : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    _Vector_base(size_t __n, const _Alloc &)
        : _M_start(0), _M_finish(0), _M_end_of_storage(0) {
      _M_start = _M_allocate(__n);
      _M_finish = _M_start;
      _M_end_of_storage = _M_start + __n;
    }

    ~_Vector_base() { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }

  protected:
    //可用区间[start, end)
    //已用区间[start, finish)
    _Tp *_M_start;           //内存起始
    _Tp *_M_finish;          //当前使用结束位置
    _Tp *_M_end_of_storage;  //内存结束

    //分配器
    typedef simple_alloc<_Tp, _Alloc> _M_data_allocator;
    //分配内存
    _Tp *_M_allocate(size_t __n) { return _M_data_allocator::allocate(__n); }
    //销毁内存
    void _M_deallocate(_Tp *__p, size_t __n) {
      _M_data_allocator::deallocate(__p, __n);
    }
  };

  template <class _Tp, class _Alloc = allocator<_Tp> >
  class vector : protected _Vector_base<_Tp, _Alloc> {
    // requirements:

    __STL_CLASS_REQUIRES(_Tp, _Assignable);

  private:
    typedef _Vector_base<_Tp, _Alloc> _Base;
    typedef vector<_Tp, _Alloc> vector_type;

  public:
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef __normal_iterator<pointer, vector_type> iterator;  //将指针转化迭代器
    typedef __normal_iterator<const_pointer, vector_type> const_iterator;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return _Base::get_allocator(); }

  #ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;
  #else  /* __STL_CLASS_PARTIAL_SPECIALIZATION */
    typedef reverse_iterator<const_iterator, value_type, const_reference,
                            difference_type>
        const_reverse_iterator;
    typedef reverse_iterator<iterator, value_type, reference, difference_type>
        reverse_iterator;
  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  protected:
  #ifdef __STL_HAS_NAMESPACES
    using _Base::_M_allocate;
    using _Base::_M_deallocate;
    using _Base::_M_end_of_storage;
    using _Base::_M_finish;
    using _Base::_M_start;
  #endif /* __STL_HAS_NAMESPACES */

  protected:
    void _M_insert_aux(iterator __position, const _Tp &__x);
    void _M_insert_aux(iterator __position);

  public:
    iterator begin() { return iterator(_M_start); }
    const_iterator begin() const { return const_iterator(_M_start); }
    iterator end() { return iterator(_M_finish); }
    const_iterator end() const { return const_iterator(_M_finish); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const {
      return const_reverse_iterator(end());
    }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const {
      return const_reverse_iterator(begin());
    }

    size_type size() const { return size_type(end() - begin()); }
    size_type max_size() const { return size_type(-1) / sizeof(_Tp); }
    size_type capacity() const {
      return size_type(const_iterator(_M_end_of_storage) - begin());
    }
    bool empty() const { return begin() == end(); }

    reference operator[](size_type __n) { return *(begin() + __n); }
    const_reference operator[](size_type __n) const { return *(begin() + __n); }

  #ifdef __STL_THROW_RANGE_ERRORS
    void _M_range_check(size_type __n) const {
      if (__n >= this->size()) __out_of_range("vector");
    }

    reference at(size_type __n) {
      _M_range_check(__n);
      return (*this)[__n];
    }
    const_reference at(size_type __n) const {
      _M_range_check(__n);
      return (*this)[__n];
    }
  #endif /* __STL_THROW_RANGE_ERRORS */

    explicit vector(const allocator_type &__a = allocator_type()) : _Base(__a) {}

    vector(size_type __n, const _Tp &__value,
          const allocator_type &__a = allocator_type())
        : _Base(__n, __a) {
      //前n个元素赋值(或者原地构造)为value
      _M_finish = uninitialized_fill_n(_M_start, __n, __value);
    }

    explicit vector(size_type __n) : _Base(__n, allocator_type()) {
      //前n个元素赋值(或者原地构造)为默认值
      _M_finish = uninitialized_fill_n(_M_start, __n, _Tp());
    }

    vector(const vector<_Tp, _Alloc> &__x)
        : _Base(__x.size(), __x.get_allocator()) {
      // vector复制
      _M_finish = uninitialized_copy(__x.begin(), __x.end(), _M_start);
    }

    //使用[first, last)的元素初始化
    vector(const _Tp *__first, const _Tp *__last,
          const allocator_type &__a = allocator_type())
        : _Base(__last - __first, __a) {
      _M_finish = uninitialized_copy(__first, __last, _M_start);
    }

    ~vector() { destroy(_M_start, _M_finish); }

    vector<_Tp, _Alloc> &operator=(const vector<_Tp, _Alloc> &__x);
    //重分配内存容量
    void reserve(size_type __n) {
      if (capacity() < __n) {
        const size_type __old_size = size();
        //分配新空间和赋值(或者原地构造)[start, finish)
        pointer __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
        //析构原来的元素
        destroy(_M_start, _M_finish);
        //销毁原来的内存
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        //使用新空间和元素
        _M_start = __tmp;
        _M_finish = __tmp + __old_size;
        _M_end_of_storage = _M_start + __n;
      }
    }

    // assign(), a generalized assignment member function.  Two
    // versions: one that takes a count, and one that takes a range.
    // The range version is a member template, so we dispatch on whether
    // or not the type is an integer.

    void assign(size_type __n, const _Tp &__val) { _M_fill_assign(__n, __val); }
    void _M_fill_assign(size_type __n, const _Tp &__val);

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }

    void push_back(const _Tp &__x) {
      if (_M_finish != _M_end_of_storage) {
        //原地构造
        construct(_M_finish, __x);
        ++_M_finish;
      } else
        //空间不够，调用插入函数，内部重新分配空间并插入尾部
        _M_insert_aux(end(), __x);
    }
    void push_back() {
      if (_M_finish != _M_end_of_storage) {
        construct(_M_finish);
        ++_M_finish;
      } else
        _M_insert_aux(end());
    }
    void swap(vector<_Tp, _Alloc> &__x) {
      //交换关键(指针)成员即可完成交换！
      __STD::swap(_M_start, __x._M_start);
      __STD::swap(_M_finish, __x._M_finish);
      __STD::swap(_M_end_of_storage, __x._M_end_of_storage);
    }

    iterator insert(iterator __position, const _Tp &__x) {
      size_type __n = __position - begin();
      //尾部插入并且空间充足，直接在尾部原地构造
      if (_M_finish != _M_end_of_storage && __position == end()) {
        construct(_M_finish, __x);
        ++_M_finish;
      } else
        _M_insert_aux(iterator(__position), __x);
      //返回插入元素的前一个位置，可以实现头插法
      return begin() + __n;
    }
    iterator insert(iterator __position) {
      size_type __n = __position - begin();
      if (_M_finish != _M_end_of_storage && __position == end()) {
        construct(_M_finish);
        ++_M_finish;
      } else
        _M_insert_aux(iterator(__position));
      return begin() + __n;
    }

    void insert(iterator __position, const_iterator __first,
                const_iterator __last);

    void insert(iterator __pos, size_type __n, const _Tp &__x) {
      _M_fill_insert(__pos, __n, __x);
    }

    void _M_fill_insert(iterator __pos, size_type __n, const _Tp &__x);

    void pop_back() {
      --_M_finish;
      destroy(_M_finish);
    }
    iterator erase(iterator __position) {
      //如果不是最后一个元素，将后面的元素都前移一个位置
      if (__position + 1 != end()) copy(__position + 1, end(), __position);
      --_M_finish;
      //析构
      destroy(_M_finish);
      //返回原删除元素的位置，实际指向了原来的下一个元素，位置相同，值却是原来的后一个元素
      return __position;
    }
    iterator erase(iterator __first, iterator __last) {
      iterator __i(copy(__last, end(), __first));
      destroy(__i, end());
      _M_finish = _M_finish - (__last - __first);
      return __first;
    }

    void resize(size_type __new_size, const _Tp &__x) {
      if (__new_size < size())
        erase(begin() + __new_size, end());
      else
        insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    void clear() { erase(begin(), end()); }

  protected:
    pointer _M_allocate_and_copy(size_type __n, const_iterator __first,
                                const_iterator __last) {
      iterator __result(_M_allocate(__n));
      __STL_TRY {
        uninitialized_copy(__first, __last, __result);
        return __result;
      }
      __STL_UNWIND(_M_deallocate(__result, __n));
    }
  };

  template <class _Tp, class _Alloc>
  inline bool operator==(const vector<_Tp, _Alloc> &__x,
                        const vector<_Tp, _Alloc> &__y) {
    return __x.size() == __y.size() && equal(__x.begin(), __x.end(), __y.begin());
  }

  template <class _Tp, class _Alloc>
  inline bool operator<(const vector<_Tp, _Alloc> &__x,
                        const vector<_Tp, _Alloc> &__y) {
    return lexicographical_compare(__x.begin(), __x.end(), __y.begin(),
                                  __y.end());
  }

  template <class _Tp, class _Alloc>
  vector<_Tp, _Alloc> &vector<_Tp, _Alloc>::operator=(
      const vector<_Tp, _Alloc> &__x) {
    if (&__x != this) {
      const size_type __xlen = __x.size();
      if (__xlen > capacity()) {
        pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
        destroy(_M_start, _M_finish);
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __tmp;
        _M_end_of_storage = _M_start + __xlen;
      } else if (size() >= __xlen) {
        iterator __i(copy(__x.begin(), __x.end(), begin()));
        destroy(__i, end());
      } else {
        copy(__x.begin(), __x.begin() + size(), _M_start);
        uninitialized_copy(__x.begin() + size(), __x.end(), _M_finish);
      }
      _M_finish = _M_start + __xlen;
    }
    return *this;
  }

  template <class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::_M_fill_assign(size_t __n, const value_type &__val) {
    if (__n > capacity()) {
      vector<_Tp, _Alloc> __tmp(__n, __val, get_allocator());
      __tmp.swap(*this);
    } else if (__n > size()) {
      fill(begin(), end(), __val);
      _M_finish = uninitialized_fill_n(_M_finish, __n - size(), __val);
    } else
      erase(fill_n(begin(), __n, __val), end());
  }

  template <class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp &__x) {
    //空间足够
    if (_M_finish != _M_end_of_storage) {
      //在备用空间中构造新元素
      construct(_M_finish, *(_M_finish - 1));
      ++_M_finish;
      _Tp __x_copy = __x;
      //[position, finish-2)元素后移一个位置到[finish-1+(finish-2 - position),
      //finish-1)
      copy_backward(__position, iterator(_M_finish - 2), iterator(_M_finish - 1));
      // position位置到元素修改为插入值
      *__position = __x_copy;
    }
    //空间不足
    else {
      //空间翻倍
      const size_type __old_size = size();
      const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
      iterator __new_start(_M_allocate(__len));
      iterator __new_finish(__new_start);
      __STL_TRY {
        //处理插入位前到元素
        __new_finish =
            uninitialized_copy(iterator(_M_start), __position, __new_start);
        //处理插入位
        construct(__new_finish.base(), __x);
        ++__new_finish;
        //处理剩下元素
        __new_finish =
            uninitialized_copy(__position, iterator(_M_finish), __new_finish);
      }
      //异常处理，销毁新空间
      __STL_UNWIND((destroy(__new_start, __new_finish),
                    _M_deallocate(__new_start.base(), __len)));
      //处理旧空间
      destroy(begin(), end());
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      //使用新空间
      _M_start = __new_start.base();
      _M_finish = __new_finish.base();
      _M_end_of_storage = __new_start.base() + __len;
    }
  }

  template <class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::_M_insert_aux(iterator __position) {
    if (_M_finish != _M_end_of_storage) {
      construct(_M_finish, *(_M_finish - 1));
      ++_M_finish;
      copy_backward(__position, iterator(_M_finish - 2), iterator(_M_finish - 1));
      *__position = _Tp();
    } else {
      const size_type __old_size = size();
      const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      __STL_TRY {
        __new_finish =
            uninitialized_copy(iterator(_M_start), __position, __new_start);
        construct(__new_finish);
        ++__new_finish;
        __new_finish =
            uninitialized_copy(__position, iterator(_M_finish), __new_finish);
      }
      __STL_UNWIND((destroy(__new_start, __new_finish),
                    _M_deallocate(__new_start, __len)));
      destroy(begin(), end());
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len;
    }
  }

  template <class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n,
                                          const _Tp &__x) {
    if (__n != 0) {
      if (size_type(_M_end_of_storage - _M_finish) >= __n) {
        _Tp __x_copy = __x;
        const size_type __elems_after = end() - __position;
        iterator __old_finish(_M_finish);
        if (__elems_after > __n) {
          uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
          _M_finish += __n;
          copy_backward(__position, __old_finish - __n, __old_finish);
          fill(__position, __position + __n, __x_copy);
        } else {
          uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
          _M_finish += __n - __elems_after;
          uninitialized_copy(__position, __old_finish, _M_finish);
          _M_finish += __elems_after;
          fill(__position, __old_finish, __x_copy);
        }
      } else {
        const size_type __old_size = size();
        const size_type __len = __old_size + max(__old_size, __n);
        iterator __new_start(_M_allocate(__len));
        iterator __new_finish(__new_start);
        __STL_TRY {
          __new_finish = uninitialized_copy(begin(), __position, __new_start);
          __new_finish = uninitialized_fill_n(__new_finish, __n, __x);
          __new_finish = uninitialized_copy(__position, end(), __new_finish);
        }
        __STL_UNWIND((destroy(__new_start, __new_finish),
                      _M_deallocate(__new_start.base(), __len)));
        destroy(_M_start, _M_finish);
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __new_start.base();
        _M_finish = __new_finish.base();
        _M_end_of_storage = __new_start.base() + __len;
      }
    }
  }

  template <class _Tp, class _Alloc>
  void vector<_Tp, _Alloc>::insert(iterator __position, const_iterator __first,
                                  const_iterator __last) {
    if (__first != __last) {
      size_type __n = 0;
      distance(__first, __last, __n);
      if (size_type(_M_end_of_storage - _M_finish) >= __n) {
        const size_type __elems_after = _M_finish - __position;
        iterator __old_finish(_M_finish);
        if (__elems_after > __n) {
          uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
          _M_finish += __n;
          copy_backward(__position, __old_finish - __n, __old_finish);
          copy(__first, __last, __position);
        } else {
          uninitialized_copy(__first + __elems_after, __last, _M_finish);
          _M_finish += __n - __elems_after;
          uninitialized_copy(__position, __old_finish, _M_finish);
          _M_finish += __elems_after;
          copy(__first, __first + __elems_after, __position);
        }
      } else {
        const size_type __old_size = size();
        const size_type __len = __old_size + max(__old_size, __n);
        iterator __new_start(_M_allocate(__len));
        iterator __new_finish(__new_start);
        __STL_TRY {
          __new_finish = uninitialized_copy(_M_start, __position, __new_start);
          __new_finish = uninitialized_copy(__first, __last, __new_finish);
          __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
        }
        __STL_UNWIND((destroy(__new_start, __new_finish),
                      _M_deallocate(__new_start, __len)));
        destroy(_M_start, _M_finish);
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage = __new_start + __len;
      }
    }
  }
  ```
  </details>

#### 容器```list```

  ![容器list](https://github.com/gongluck/images/blob/main/容器list.png)

  ```node```节点由前后指针和数据成员```T```组成。
  ```list```包含一个```node```指针指向链表```哨兵```节点。
  ```list::iterator```包含一个```node```指针指向链表节点。

  [stl_list.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_list.h)

  <details>
  <summary>list</summary>

  ```C++
  //链表节点基类
  struct _List_node_base {
    _List_node_base *_M_next;  //指向下一个节点
    _List_node_base *_M_prev;  //指向上一个节点
  };

  //链表节点
  template <class _Tp>
  struct _List_node : public _List_node_base {
    _Tp _M_data;  //元素
  };

  //链表迭代器基类
  struct _List_iterator_base {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag iterator_category;  //可双向访问

    _List_node_base *_M_node;  //节点指针

    _List_iterator_base(_List_node_base *__x) : _M_node(__x) {}
    _List_iterator_base() {}

    void _M_incr() { _M_node = _M_node->_M_next; }  //++
    void _M_decr() { _M_node = _M_node->_M_prev; }  //--

    bool operator==(const _List_iterator_base &__x) const {
      return _M_node == __x._M_node;
    }
    bool operator!=(const _List_iterator_base &__x) const {
      return _M_node != __x._M_node;
    }
  };

  //链表迭代器
  template <class _Tp, class _Ref, class _Ptr>
  struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Tp &, _Tp *> iterator;
    typedef _List_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;
    typedef _List_iterator<_Tp, _Ref, _Ptr> _Self;

    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef _List_node<_Tp> _Node;

    _List_iterator(_Node *__x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    _List_iterator(const iterator &__x) : _List_iterator_base(__x._M_node) {}

    //迭代器解引用返回元素的数据字段
    reference operator*() const { return ((_Node *)_M_node)->_M_data; }

  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    //同上，迭代器->运算符转为对元素的数据字段的->操作
    pointer operator->() const { return &(operator*()); }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */

    //自增自减操作
    _Self &operator++() {
      this->_M_incr();
      return *this;
    }
    _Self operator++(int) {
      _Self __tmp = *this;
      this->_M_incr();
      return __tmp;
    }
    _Self &operator--() {
      this->_M_decr();
      return *this;
    }
    _Self operator--(int) {
      _Self __tmp = *this;
      this->_M_decr();
      return __tmp;
    }
  };

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  inline bidirectional_iterator_tag iterator_category(
      const _List_iterator_base &) {
    return bidirectional_iterator_tag();
  }

  template <class _Tp, class _Ref, class _Ptr>
  inline _Tp *value_type(const _List_iterator<_Tp, _Ref, _Ptr> &) {
    return 0;
  }

  inline ptrdiff_t *distance_type(const _List_iterator_base &) { return 0; }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  //链表基类
  template <class _Tp, class _Alloc>
  class _List_base {
  public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    _List_base(const allocator_type &) {
      _M_node =
          _M_get_node();  //分配一个节点作为哨兵，作为“🈳️”节点
      _M_node->_M_next = _M_node;  //指向“🈳️”节点
      _M_node->_M_prev = _M_node;  //指向“🈳️”节点
    }
    ~_List_base() {
      //清空链表
      clear();
      //销毁空节点
      _M_put_node(_M_node);
    }

    void clear();

  protected:
    typedef simple_alloc<_List_node<_Tp>, _Alloc> _Alloc_type;
    //分配一个节点的内存空间
    _List_node<_Tp> *_M_get_node() { return _Alloc_type::allocate(1); }
    //销毁节点的内存空间
    void _M_put_node(_List_node<_Tp> *__p) { _Alloc_type::deallocate(__p, 1); }

  protected:
    _List_node<_Tp> *_M_node;  //哨兵节点
  };

  //清空链表
  template <class _Tp, class _Alloc>
  void _List_base<_Tp, _Alloc>::clear() {
    //从哨兵节点的下一个节点开始处理
    _List_node<_Tp> *__cur = (_List_node<_Tp> *)_M_node->_M_next;
    while (__cur != _M_node) {
      _List_node<_Tp> *__tmp = __cur;
      __cur = (_List_node<_Tp> *)__cur->_M_next;
      //调用析构操作
      _Destroy(&__tmp->_M_data);
      //销毁节点内存
      _M_put_node(__tmp);
    }
    //指向哨兵，回归“🈳️”链表状态
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
  }

  //链表
  template <class _Tp, class _Alloc = allocator<_Tp>>
  class list : protected _List_base<_Tp, _Alloc> {
    // requirements:

    __STL_CLASS_REQUIRES(_Tp, _Assignable);

    typedef _List_base<_Tp, _Alloc> _Base;

  protected:
    typedef void *_Void_pointer;

  public:
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef _List_node<_Tp> _Node;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return _Base::get_allocator(); }

  public:
    typedef _List_iterator<_Tp, _Tp &, _Tp *> iterator;
    typedef _List_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;

  #ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;
  #else  /* __STL_CLASS_PARTIAL_SPECIALIZATION */
    typedef reverse_bidirectional_iterator<const_iterator, value_type,
                                          const_reference, difference_type>
        const_reverse_iterator;
    typedef reverse_bidirectional_iterator<iterator, value_type, reference,
                                          difference_type>
        reverse_iterator;
  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  protected:
  #ifdef __STL_HAS_NAMESPACES
    using _Base::_M_get_node;
    using _Base::_M_node;
    using _Base::_M_put_node;
  #endif /* __STL_HAS_NAMESPACES */

  protected:
    //创建新节点
    _Node *_M_create_node(const _Tp &__x) {
      //获取新节点内存
      _Node *__p = _M_get_node();
      __STL_TRY {
        //调用构造
        _Construct(&__p->_M_data, __x);
      }
      // catch
      __STL_UNWIND(_M_put_node(__p));
      return __p;
    }

    //创建新节点，使用默认值版本
    _Node *_M_create_node() {
      _Node *__p = _M_get_node();
      __STL_TRY { _Construct(&__p->_M_data); }
      __STL_UNWIND(_M_put_node(__p));
      return __p;
    }

  public:
    explicit list(const allocator_type &__a = allocator_type()) : _Base(__a) {}

    //第一个元素是哨兵的下一个元素
    //最后一个元素是哨兵
    //下面的函数定义简单明了

    iterator begin() { return (_Node *)(_M_node->_M_next); }
    const_iterator begin() const { return (_Node *)(_M_node->_M_next); }

    iterator end() { return _M_node; }
    const_iterator end() const { return _M_node; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const {
      return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
      // reverse_iterator的operator*是(--*node)的，所以用begin()初始化没有问题
      return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const {
      // const_reverse_iterator的operator*是(--*node)的，所以用begin()初始化没有问题
      return const_reverse_iterator(begin());
    }

    //判断第一个逻辑节点是否指向哨兵作为链表空的依据
    bool empty() const { return _M_node->_M_next == _M_node; }
    size_type size() const {
      size_type __result = 0;
      //非可随机访问类别迭代器的distance实际上遍历了一遍所有元素！
      distance(begin(), end(), __result);
      return __result;
    }
    size_type max_size() const { return size_type(-1); }

    //第一个元素是哨兵的下一个元素
    //最后一个元素是哨兵
    //下面的函数定义简单明了

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }

    //交换哨兵相当于交换了链表！
    void swap(list<_Tp, _Alloc> &__x) { __STD::swap(_M_node, __x._M_node); }

    //插入新元素
    iterator insert(iterator __position, const _Tp &__x) {
      //构造一个新节点
      _Node *__tmp = _M_create_node(__x);
      //头插法
      __tmp->_M_next = __position._M_node;
      __tmp->_M_prev = __position._M_node->_M_prev;
      __position._M_node->_M_prev->_M_next = __tmp;
      __position._M_node->_M_prev = __tmp;
      //返回新节点，同时它也是position的元素。
      return __tmp;
    }
    //插入新元素，默认版本
    iterator insert(iterator __position) { return insert(__position, _Tp()); }

    void insert(iterator __position, const _Tp *__first, const _Tp *__last);
    void insert(iterator __position, const_iterator __first,
                const_iterator __last);

    //插入多个新元素
    void insert(iterator __pos, size_type __n, const _Tp &__x) {
      _M_fill_insert(__pos, __n, __x);
    }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp &__x);

    //在最前面插入
    void push_front(const _Tp &__x) { insert(begin(), __x); }
    void push_front() { insert(begin()); }
    //在尾部插入
    void push_back(const _Tp &__x) { insert(end(), __x); }
    void push_back() { insert(end()); }

    //删除该位置的元素
    iterator erase(iterator __position) {
      //保存删除位元素的前后指向
      _List_node_base *__next_node = __position._M_node->_M_next;
      _List_node_base *__prev_node = __position._M_node->_M_prev;
      //获取删除位元素
      _Node *__n = (_Node *)__position._M_node;
      //连接获取删除位元素的前后元素
      __prev_node->_M_next = __next_node;
      __next_node->_M_prev = __prev_node;
      //析构
      _Destroy(&__n->_M_data);
      //销毁内存
      _M_put_node(__n);
      //返回原下一个元素，此时它就在position位！
      return iterator((_Node *)__next_node);
    }
    iterator erase(iterator __first, iterator __last);
    void clear() { _Base::clear(); }

    void resize(size_type __new_size, const _Tp &__x);
    void resize(size_type __new_size) { this->resize(__new_size, _Tp()); }

    //删除首元素
    void pop_front() { erase(begin()); }
    //删除尾元素，非哨兵！
    void pop_back() {
      //哨兵
      iterator __tmp = end();
      //删除哨兵前一个元素
      erase(--__tmp);
    }
    //构造，n个元素，值都为value
    list(size_type __n, const _Tp &__value,
        const allocator_type &__a = allocator_type())
        : _Base(__a) {
      insert(begin(), __n, __value);
    }
    //构造，n个元素，值都为默认值
    explicit list(size_type __n) : _Base(allocator_type()) {
      insert(begin(), __n, _Tp());
    }

    //构造，初始为[first, last)的值
    list(const _Tp *__first, const _Tp *__last,
        const allocator_type &__a = allocator_type())
        : _Base(__a) {
      this->insert(begin(), __first, __last);
    }
    list(const_iterator __first, const_iterator __last,
        const allocator_type &__a = allocator_type())
        : _Base(__a) {
      this->insert(begin(), __first, __last);
    }

    //拷贝构造
    list(const list<_Tp, _Alloc> &__x) : _Base(__x.get_allocator()) {
      insert(begin(), __x.begin(), __x.end());
    }

    //链表基类负责内存管理了，链表类无需处理
    ~list() {}

    list<_Tp, _Alloc> &operator=(const list<_Tp, _Alloc> &__x);

  public:
    // assign(), a generalized assignment member function.  Two
    // versions: one that takes a count, and one that takes a range.
    // The range version is a member template, so we dispatch on whether
    // or not the type is an integer.

    void assign(size_type __n, const _Tp &__val) { _M_fill_assign(__n, __val); }

    void _M_fill_assign(size_type __n, const _Tp &__val);

  protected:
    // 移动[first, last)的元素到position前
    void transfer(iterator __position, iterator __first, iterator __last) {
      if (__position != __last) {
        // Remove [first, last) from its old position.

        // position节点成为last前节点的后继
        __last._M_node->_M_prev->_M_next = __position._M_node;
        // last节点成为first前节点的后继
        __first._M_node->_M_prev->_M_next = __last._M_node;
        // first节点成为position前节点的后继
        __position._M_node->_M_prev->_M_next = __first._M_node;

        // Splice [first, last) into its new position.

        _List_node_base *__tmp = __position._M_node->_M_prev;
        // last前节点成为position前节点
        __position._M_node->_M_prev = __last._M_node->_M_prev;
        // fist前节点成为last前节点
        __last._M_node->_M_prev = __first._M_node->_M_prev;
        // position前节点成为first前节点
        __first._M_node->_M_prev = __tmp;
      }
    }

  public:
    //将链表x拼接到position前
    void splice(iterator __position, list &__x) {
      if (!__x.empty()) this->transfer(__position, __x.begin(), __x.end());
    }
    //将i移动到position前
    void splice(iterator __position, list &, iterator __i) {
      iterator __j = __i;
      ++__j;
      if (__position == __i || __position == __j) return;
      this->transfer(__position, __i, __j);
    }
    //将[first, last)移动到position前
    void splice(iterator __position, list &, iterator __first, iterator __last) {
      if (__first != __last) this->transfer(__position, __first, __last);
    }
    void remove(const _Tp &__value);
    void unique();
    void merge(list &__x);
    void reverse();
    void sort();
  };

  template <class _Tp, class _Alloc>
  inline bool operator==(const list<_Tp, _Alloc> &__x,
                        const list<_Tp, _Alloc> &__y) {
    typedef typename list<_Tp, _Alloc>::const_iterator const_iterator;
    const_iterator __end1 = __x.end();
    const_iterator __end2 = __y.end();

    const_iterator __i1 = __x.begin();
    const_iterator __i2 = __y.begin();
    while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2) {
      ++__i1;
      ++__i2;
    }
    return __i1 == __end1 && __i2 == __end2;
  }

  template <class _Tp, class _Alloc>
  inline bool operator<(const list<_Tp, _Alloc> &__x,
                        const list<_Tp, _Alloc> &__y) {
    return lexicographical_compare(__x.begin(), __x.end(), __y.begin(),
                                  __y.end());
  }

  // position前插入[first, last)
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::insert(iterator __position, const _Tp *__first,
                                const _Tp *__last) {
    for (; __first != __last; ++__first) insert(__position, *__first);
  }

  // position前插入[first, last)
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::insert(iterator __position, const_iterator __first,
                                const_iterator __last) {
    for (; __first != __last; ++__first) insert(__position, *__first);
  }

  // position前插入n个x
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n,
                                        const _Tp &__x) {
    for (; __n > 0; --__n) insert(__position, __x);
  }

  // 删除[first, last)
  template <class _Tp, class _Alloc>
  typename list<_Tp, _Alloc>::iterator list<_Tp, _Alloc>::erase(iterator __first,
                                                                iterator __last) {
    while (__first != __last) erase(__first++);
    return __last;
  }

  // 重新设置大小
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::resize(size_type __new_size, const _Tp &__x) {
    iterator __i = begin();
    size_type __len = 0;
    for (; __i != end() && __len < __new_size; ++__i, ++__len)
      ;
    if (__len == __new_size)
      erase(__i, end());  //如果当前大小大于新大小，删除多余元素
    else                  // __i == end()
      insert(end(), __new_size - __len, __x);  //尾部补充缺少元素
  }

  // 链表拷贝
  template <class _Tp, class _Alloc>
  list<_Tp, _Alloc> &list<_Tp, _Alloc>::operator=(const list<_Tp, _Alloc> &__x) {
    if (this != &__x) {
      iterator __first1 = begin();
      iterator __last1 = end();
      const_iterator __first2 = __x.begin();
      const_iterator __last2 = __x.end();
      // 元素值拷贝
      while (__first1 != __last1 && __first2 != __last2)
        *__first1++ = *__first2++;
      if (__first2 == __last2)
        erase(__first1, __last1);  //删除多余元素
      else
        insert(__last1, __first2, __last2);  //补充插入剩下的元素
    }
    return *this;
  }

  // 链表重新赋值为n个val
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::_M_fill_assign(size_type __n, const _Tp &__val) {
    iterator __i = begin();
    for (; __i != end() && __n > 0; ++__i, --__n) *__i = __val;
    if (__n > 0)
      insert(end(), __n, __val);  //补充插入
    else
      erase(__i, end());  //删除多余
  }

  //删除value元素
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::remove(const _Tp &__value) {
    iterator __first = begin();
    iterator __last = end();
    while (__first != __last) {
      //先获取下一个迭代器
      iterator __next = __first;
      ++__next;
      //等于value就删除
      if (*__first == __value) erase(__first);
      //赋值为next进行下一轮循环
      __first = __next;
    }
  }

  //删除多余重复元素
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::unique() {
    iterator __first = begin();
    iterator __last = end();
    if (__first == __last) return;
    iterator __next = __first;
    while (++__next != __last) {
      if (*__first == *__next)
        erase(__next);  // *first==*lext，有重复，删除next
      else
        __first = __next;
      __next = __first;
    }
  }

  //合并链表
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::merge(list<_Tp, _Alloc> &__x) {
    iterator __first1 = begin();
    iterator __last1 = end();
    iterator __first2 = __x.begin();
    iterator __last2 = __x.end();
    while (__first1 != __last1 && __first2 != __last2)
      if (*__first2 < *__first1) {
        //插入first2
        iterator __next = __first2;
        transfer(__first1, __first2, ++__next);
        __first2 = __next;
      } else
        // first1滑动一个位置
        ++__first1;
    if (__first2 != __last2)
      //插入剩余节点
      transfer(__last1, __first2, __last2);
  }

  //反转链表
  inline void __List_base_reverse(_List_node_base *__p) {
    _List_node_base *__tmp = __p;
    do {
      //循环交换prev和next
      __STD::swap(__tmp->_M_next, __tmp->_M_prev);
      __tmp = __tmp->_M_prev;  // Old next node is now prev.
    } while (__tmp != __p);
  }

  //反转链表
  template <class _Tp, class _Alloc>
  inline void list<_Tp, _Alloc>::reverse() {
    __List_base_reverse(this->_M_node);
  }

  //排序
  template <class _Tp, class _Alloc>
  void list<_Tp, _Alloc>::sort() {
    // Do nothing if the list has length 0 or 1.
    if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node) {
      list<_Tp, _Alloc> __carry;
      list<_Tp, _Alloc> __counter[64];
      int __fill = 0;
      while (!empty()) {
        //移动第一个元素到临时链表开头
        __carry.splice(__carry.begin(), *this, begin());
        int __i = 0;
        while (__i < __fill && !__counter[__i].empty()) {
          __counter[__i].merge(__carry);
          __carry.swap(__counter[__i++]);
        }
        __carry.swap(__counter[__i]);
        if (__i == __fill) ++__fill;
      }

      for (int __i = 1; __i < __fill; ++__i)
        __counter[__i].merge(__counter[__i - 1]);
      swap(__counter[__fill - 1]);
    }
  }
  ```
  </details>

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
