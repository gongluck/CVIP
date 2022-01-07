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
      - [容器```array```](#容器array)
      - [容器```vector```](#容器vector)
      - [容器```list```](#容器list)
      - [容器```slist```](#容器slist)
      - [容器```deque```](#容器deque)
      - [容器适配器](#容器适配器)
        - [容器适配器```stack```](#容器适配器stack)
        - [容器适配器```queue```](#容器适配器queue)
        - [容器适配器```priority_queue```](#容器适配器priority_queue)
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

#### 容器```array```

  ![容器array](https://github.com/gongluck/images/blob/main/stl/容器array.png)

  对原```C```数组的封装。

  [array](https://github.com/gongluck/sourcecode/blob/main/stl/array)

  <details>
  <summary>array</summary>

  ```C++
  //数组
  template <typename _Tp, std::size_t _Nm>
  struct array
  {
    typedef _Tp value_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // Support for zero-sized arrays mandatory.
    value_type _M_instance[_Nm ? _Nm : 1]; //底层数组

    // No explicit construct/copy/destroy for aggregate type.

    //覆盖值
    void assign(const value_type &__u)
    {
      std::fill_n(begin(), size(), __u);
    }

    //交换
    void swap(array &__other)
    {
      std::swap_ranges(begin(), end(), __other.begin());
    }

    // Iterators.
    iterator begin()
    {
      return iterator(std::__addressof(_M_instance[0]));
    }

    const_iterator begin() const
    {
      return const_iterator(std::__addressof(_M_instance[0]));
    }

    iterator end()
    {
      return iterator(std::__addressof(_M_instance[_Nm]));
    }

    const_iterator end() const
    {
      return const_iterator(std::__addressof(_M_instance[_Nm]));
    }

    reverse_iterator rbegin()
    {
      return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }

    reverse_iterator rend()
    {
      return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }

    // Capacity.
    size_type size() const { return _Nm; }

    size_type max_size() const { return _Nm; }

    bool empty() const { return size() == 0; }

    // Element access.
    reference operator[](size_type __n)
    {
      return _M_instance[__n];
    }

    const_reference operator[](size_type __n) const
    {
      return _M_instance[__n];
    }

    reference at(size_type __n)
    {
      if (__n >= _Nm)
        std::__throw_out_of_range(__N("array::at"));
      return _M_instance[__n];
    }

    const_reference at(size_type __n) const
    {
      if (__n >= _Nm)
        std::__throw_out_of_range(__N("array::at"));
      return _M_instance[__n];
    }

    reference front()
    {
      return *begin();
    }

    const_reference front() const
    {
      return *begin();
    }

    reference back()
    {
      return _Nm ? *(end() - 1) : *end();
    }

    const_reference back() const
    {
      return _Nm ? *(end() - 1) : *end();
    }

    _Tp *data()
    {
      return std::__addressof(_M_instance[0]);
    }

    const _Tp *data() const
    {
      return std::__addressof(_M_instance[0]);
    }
  };

  // Array comparisons.
  template <typename _Tp, std::size_t _Nm>
  inline bool operator==(const array<_Tp, _Nm> &__one, const array<_Tp, _Nm> &__two)
  {
    return std::equal(__one.begin(), __one.end(), __two.begin());
  }

  template <typename _Tp, std::size_t _Nm>
  inline bool operator!=(const array<_Tp, _Nm> &__one, const array<_Tp, _Nm> &__two)
  {
    return !(__one == __two);
  }

  template <typename _Tp, std::size_t _Nm>
  inline bool operator<(const array<_Tp, _Nm> &__a, const array<_Tp, _Nm> &__b)
  {
    return std::lexicographical_compare(__a.begin(), __a.end(),
                                        __b.begin(), __b.end());
  }

  template <typename _Tp, std::size_t _Nm>
  inline bool operator>(const array<_Tp, _Nm> &__one, const array<_Tp, _Nm> &__two)
  {
    return __two < __one;
  }

  template <typename _Tp, std::size_t _Nm>
  inline bool operator<=(const array<_Tp, _Nm> &__one, const array<_Tp, _Nm> &__two)
  {
    return !(__one > __two);
  }

  template <typename _Tp, std::size_t _Nm>
  inline bool operator>=(const array<_Tp, _Nm> &__one, const array<_Tp, _Nm> &__two)
  {
    return !(__one < __two);
  }

  // Specialized algorithms [6.2.2.2].
  template <typename _Tp, std::size_t _Nm>
  inline void
  swap(array<_Tp, _Nm> &__one, array<_Tp, _Nm> &__two)
  {
    __one.swap(__two);
  }

  // Tuple interface to class template array [6.2.2.5].

  /// tuple_size
  template <typename _Tp>
  class tuple_size;

  /// tuple_element
  template <int _Int, typename _Tp>
  class tuple_element;

  template <typename _Tp, std::size_t _Nm>
  struct tuple_size<array<_Tp, _Nm> >
  {
    static const int value = _Nm;
  };

  template <typename _Tp, std::size_t _Nm>
  const int
      tuple_size<array<_Tp, _Nm> >::value;

  template <int _Int, typename _Tp, std::size_t _Nm>
  struct tuple_element<_Int, array<_Tp, _Nm> >
  {
    typedef _Tp type;
  };

  template <int _Int, typename _Tp, std::size_t _Nm>
  inline _Tp &
  get(array<_Tp, _Nm> &__arr)
  {
    return __arr[_Int];
  }

  template <int _Int, typename _Tp, std::size_t _Nm>
  inline const _Tp &
  get(const array<_Tp, _Nm> &__arr)
  {
    return __arr[_Int];
  }
  ```
  </details>

#### 容器```vector```

  ![容器vector](https://github.com/gongluck/images/blob/main/stl/容器vector.png)

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

  ![容器list](https://github.com/gongluck/images/blob/main/stl/容器list.png)

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

#### 容器```slist```

  ```node```节点由后指针和数据成员```T```组成。
  ```slist```包含一个```node```指针指向单向链表头节点(哨兵)。
  ```slist::iterator```包含一个```node```指针指向链表节点。

  [slist](https://github.com/gongluck/sourcecode/blob/main/stl/slist)

  <details>
  <summary>slist</summary>

  ```C++
  //单向链表节点基类
  struct _Slist_node_base
  {
    _Slist_node_base *_M_next; //指向下一个节点
  };

  //连接两个节点 新节点插入到prev节点后，返回新节点
  inline _Slist_node_base *
  __slist_make_link(_Slist_node_base *__prev_node,
                    _Slist_node_base *__new_node)
  {
    //新节点插入到prev节点后
    __new_node->_M_next = __prev_node->_M_next;
    __prev_node->_M_next = __new_node;
    return __new_node;
  }

  //返回指定节点的前一个节点
  inline _Slist_node_base *
  __slist_previous(_Slist_node_base *__head,
                  const _Slist_node_base *__node)
  {
    //单向链表只能从头遍历寻找
    while (__head && __head->_M_next != __node)
      __head = __head->_M_next;
    return __head;
  }

  inline const _Slist_node_base *
  __slist_previous(const _Slist_node_base *__head,
                  const _Slist_node_base *__node)
  {
    while (__head && __head->_M_next != __node)
      __head = __head->_M_next;
    return __head;
  }

  //在pos后拼接(first, last]节点
  inline void __slist_splice_after(_Slist_node_base *__pos,
                                  _Slist_node_base *__before_first,
                                  _Slist_node_base *__before_last)
  {
    if (__pos != __before_first && __pos != __before_last)
    {
      _Slist_node_base *__first = __before_first->_M_next;
      _Slist_node_base *__after = __pos->_M_next;
      //抽出(first, last]
      __before_first->_M_next = __before_last->_M_next;
      //(first, last]插到pos后
      __pos->_M_next = __first;
      __before_last->_M_next = __after;
    }
  }

  //在pos后拼接(head, ...)的节点
  inline void
  __slist_splice_after(_Slist_node_base *__pos, _Slist_node_base *__head)
  {
    //获取最后一个节点
    _Slist_node_base *__before_last = __slist_previous(__head, 0);
    if (__before_last != __head)
    {
      _Slist_node_base *__after = __pos->_M_next;
      //插入head指向的链表到pos后
      __pos->_M_next = __head->_M_next;
      __before_last->_M_next = __after;
      //置空原链表
      __head->_M_next = 0;
    }
  }

  //反转链表
  inline _Slist_node_base *__slist_reverse(_Slist_node_base *__node)
  {
    //头节点
    _Slist_node_base *__result = __node;
    // next节点
    __node = __node->_M_next;
    __result->_M_next = 0;
    while (__node)
    {
      _Slist_node_base *__next = __node->_M_next;
      // result  -->  node  -->  next
      __node->_M_next = __result;
      // result  <--  node       next
      __result = __node;
      //             result      node
      __node = __next;
    }
    return __result;
  }

  //遍历得出链表长度
  inline size_t __slist_size(_Slist_node_base *__node)
  {
    size_t __result = 0;
    for (; __node != 0; __node = __node->_M_next)
      ++__result;
    return __result;
  }

  //链表节点
  template <class _Tp>
  struct _Slist_node : public _Slist_node_base
  {
    _Tp _M_data;
  };

  //链表迭代器基类
  struct _Slist_iterator_base
  {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef forward_iterator_tag iterator_category; //可单向遍历类别

    //指向节点
    _Slist_node_base *_M_node;

    _Slist_iterator_base(_Slist_node_base *__x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; } //++

    bool operator==(const _Slist_iterator_base &__x) const
    {
      return _M_node == __x._M_node; //节点指针比较
    }
    bool operator!=(const _Slist_iterator_base &__x) const
    {
      return _M_node != __x._M_node;
    }
  };

  //链表迭代器
  template <class _Tp, class _Ref, class _Ptr>
  struct _Slist_iterator : public _Slist_iterator_base
  {
    typedef _Slist_iterator<_Tp, _Tp &, _Tp *> iterator;
    typedef _Slist_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;
    typedef _Slist_iterator<_Tp, _Ref, _Ptr> _Self;

    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef _Slist_node<_Tp> _Node;

    _Slist_iterator(_Node *__x) : _Slist_iterator_base(__x) {}
    _Slist_iterator() : _Slist_iterator_base(0) {}
    _Slist_iterator(const iterator &__x) : _Slist_iterator_base(__x._M_node) {}

    //解引用实际转换为节点数据类型
    reference operator*() const { return ((_Node *)_M_node)->_M_data; }
  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const
    {
      return &(operator*());
    }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */

    //后置++
    _Self &operator++()
    {
      _M_incr();
      return *this;
    }
    _Self operator++(int)
    {
      _Self __tmp = *this;
      _M_incr();
      return __tmp;
    }
  };

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  inline ptrdiff_t *distance_type(const _Slist_iterator_base &)
  {
    return 0;
  }

  inline forward_iterator_tag iterator_category(const _Slist_iterator_base &)
  {
    return forward_iterator_tag();
  }

  template <class _Tp, class _Ref, class _Ptr>
  inline _Tp *value_type(const _Slist_iterator<_Tp, _Ref, _Ptr> &)
  {
    return 0;
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  //链表基类
  template <class _Tp, class _Alloc>
  struct _Slist_base
  {
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    _Slist_base(const allocator_type &) { _M_head._M_next = 0; }
    ~_Slist_base() { _M_erase_after(&_M_head, 0); }

  protected:
    typedef simple_alloc<_Slist_node<_Tp>, _Alloc> _Alloc_type;
    //分配节点空间
    _Slist_node<_Tp> *_M_get_node() { return _Alloc_type::allocate(1); }
    //销毁节点空间
    void _M_put_node(_Slist_node<_Tp> *__p) { _Alloc_type::deallocate(__p, 1); }

    //删除pos后的节点
    _Slist_node_base *_M_erase_after(_Slist_node_base *__pos)
    {
      // pos  -->  next  -->  nextnext
      _Slist_node<_Tp> *__next = (_Slist_node<_Tp> *)(__pos->_M_next);
      _Slist_node_base *__next_next = __next->_M_next; //__next为NULL会抛出空指针访问异常
      __pos->_M_next = __next_next;
      // pos       next  -->  nextnext
      //  |                       ^
      //  - - - - - - - - - - - - |
      destroy(&__next->_M_data); //析构
      _M_put_node(__next);       //销毁内存
      return __next_next;        //返回删除节点的后一个节点
    }
    _Slist_node_base *_M_erase_after(_Slist_node_base *, _Slist_node_base *);

  protected:
    _Slist_node_base _M_head; //哨兵
  };

  //删除(first, last)
  template <class _Tp, class _Alloc>
  _Slist_node_base *
  _Slist_base<_Tp, _Alloc>::_M_erase_after(_Slist_node_base *__before_first,
                                          _Slist_node_base *__last_node)
  {
    _Slist_node<_Tp> *__cur = (_Slist_node<_Tp> *)(__before_first->_M_next);
    while (__cur != __last_node)
    {
      _Slist_node<_Tp> *__tmp = __cur;
      __cur = (_Slist_node<_Tp> *)__cur->_M_next;
      destroy(&__tmp->_M_data);
      _M_put_node(__tmp);
    }
    __before_first->_M_next = __last_node;
    return __last_node;
  }

  //单向链表
  template <class _Tp, class _Alloc = __STL_DEFAULT_ALLOCATOR(_Tp)>
  class slist : private _Slist_base<_Tp, _Alloc>
  {
    // requirements:

    __STL_CLASS_REQUIRES(_Tp, _Assignable);

  private:
    typedef _Slist_base<_Tp, _Alloc> _Base;

  public:
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef _Slist_iterator<_Tp, _Tp &, _Tp *> iterator;
    typedef _Slist_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return _Base::get_allocator(); }

  private:
    typedef _Slist_node<_Tp> _Node;
    typedef _Slist_node_base _Node_base;
    typedef _Slist_iterator_base _Iterator_base;

    //创建节点
    _Node *_M_create_node(const value_type &__x)
    {
      //申请空间
      _Node *__node = this->_M_get_node();
      __STL_TRY
      {
        //构造
        construct(&__node->_M_data, __x);
        __node->_M_next = 0;
      }
      __STL_UNWIND(this->_M_put_node(__node));
      return __node;
    }
    _Node *_M_create_node()
    {
      _Node *__node = this->_M_get_node();
      __STL_TRY
      {
        construct(&__node->_M_data);
        __node->_M_next = 0;
      }
      __STL_UNWIND(this->_M_put_node(__node));
      return __node;
    }

  public:
    explicit slist(const allocator_type &__a = allocator_type()) : _Base(__a) {}

    slist(size_type __n, const value_type &__x,
          const allocator_type &__a = allocator_type()) : _Base(__a)
    {
      _M_insert_after_fill(&this->_M_head, __n, __x);
    }

    explicit slist(size_type __n) : _Base(allocator_type())
    {
      _M_insert_after_fill(&this->_M_head, __n, value_type());
    }

    slist(const_iterator __first, const_iterator __last,
          const allocator_type &__a = allocator_type()) : _Base(__a)
    {
      _M_insert_after_range(&this->_M_head, __first, __last);
    }
    slist(const value_type *__first, const value_type *__last,
          const allocator_type &__a = allocator_type()) : _Base(__a)
    {
      _M_insert_after_range(&this->_M_head, __first, __last);
    }

    slist(const slist &__x) : _Base(__x.get_allocator())
    {
      _M_insert_after_range(&this->_M_head, __x.begin(), __x.end());
    }

    slist &operator=(const slist &__x);

    ~slist() {}

  public:
    // assign(), a generalized assignment member function.  Two
    // versions: one that takes a count, and one that takes a range.
    // The range version is a member template, so we dispatch on whether
    // or not the type is an integer.

    void assign(size_type __n, const _Tp &__val)
    {
      _M_fill_assign(__n, __val);
    }

    void _M_fill_assign(size_type __n, const _Tp &__val);

  public:
    //类似list的设计，真正可用节点是哨兵节点后的节点
    iterator begin() { return iterator((_Node *)this->_M_head._M_next); }
    const_iterator begin() const
    {
      return const_iterator((_Node *)this->_M_head._M_next);
    }

    iterator end() { return iterator(0); }
    const_iterator end() const { return const_iterator(0); }

    // Experimental new feature: before_begin() returns a
    // non-dereferenceable iterator that, when incremented, yields
    // begin().  This iterator may be used as the argument to
    // insert_after, erase_after, etc.  Note that even for an empty
    // slist, before_begin() is not the same iterator as end().  It
    // is always necessary to increment before_begin() at least once to
    // obtain end().
    iterator before_begin() { return iterator((_Node *)&this->_M_head); }
    const_iterator before_begin() const
    {
      return const_iterator((_Node *)&this->_M_head);
    }

    size_type size() const { return __slist_size(this->_M_head._M_next); }

    size_type max_size() const { return size_type(-1); }

    //哨兵指向空即为空链表
    bool empty() const { return this->_M_head._M_next == 0; }

    void swap(slist &__x)
    {
      __STD::swap(this->_M_head._M_next, __x._M_head._M_next);
    }

  public:
    reference front() { return ((_Node *)this->_M_head._M_next)->_M_data; }
    const_reference front() const
    {
      return ((_Node *)this->_M_head._M_next)->_M_data;
    }
    void push_front(const value_type &__x)
    {
      __slist_make_link(&this->_M_head, _M_create_node(__x));
    }
    void push_front() { __slist_make_link(&this->_M_head, _M_create_node()); }
    void pop_front()
    {
      _Node *__node = (_Node *)this->_M_head._M_next;
      this->_M_head._M_next = __node->_M_next;
      destroy(&__node->_M_data);
      this->_M_put_node(__node);
    }

    iterator previous(const_iterator __pos)
    {
      return iterator((_Node *)__slist_previous(&this->_M_head, __pos._M_node));
    }
    const_iterator previous(const_iterator __pos) const
    {
      return const_iterator((_Node *)__slist_previous(&this->_M_head,
                                                      __pos._M_node));
    }

  private:
    _Node *_M_insert_after(_Node_base *__pos, const value_type &__x)
    {
      return (_Node *)(__slist_make_link(__pos, _M_create_node(__x)));
    }

    _Node *_M_insert_after(_Node_base *__pos)
    {
      return (_Node *)(__slist_make_link(__pos, _M_create_node()));
    }

    void _M_insert_after_fill(_Node_base *__pos,
                              size_type __n, const value_type &__x)
    {
      for (size_type __i = 0; __i < __n; ++__i)
        __pos = __slist_make_link(__pos, _M_create_node(__x));
    }

    // pos后插入[first, last)
    void _M_insert_after_range(_Node_base *__pos,
                              const_iterator __first, const_iterator __last)
    {
      while (__first != __last)
      {
        __pos = __slist_make_link(__pos, _M_create_node(*__first));
        ++__first;
      }
    }
    void _M_insert_after_range(_Node_base *__pos,
                              const value_type *__first,
                              const value_type *__last)
    {
      while (__first != __last)
      {
        __pos = __slist_make_link(__pos, _M_create_node(*__first));
        ++__first;
      }
    }

  public:
    iterator insert_after(iterator __pos, const value_type &__x)
    {
      return iterator(_M_insert_after(__pos._M_node, __x));
    }

    iterator insert_after(iterator __pos)
    {
      return insert_after(__pos, value_type());
    }

    void insert_after(iterator __pos, size_type __n, const value_type &__x)
    {
      _M_insert_after_fill(__pos._M_node, __n, __x);
    }

    void insert_after(iterator __pos,
                      const_iterator __first, const_iterator __last)
    {
      _M_insert_after_range(__pos._M_node, __first, __last);
    }
    void insert_after(iterator __pos,
                      const value_type *__first, const value_type *__last)
    {
      _M_insert_after_range(__pos._M_node, __first, __last);
    }

    iterator insert(iterator __pos, const value_type &__x)
    {
      return iterator(_M_insert_after(__slist_previous(&this->_M_head,
                                                      __pos._M_node),
                                      __x));
    }

    iterator insert(iterator __pos)
    {
      return iterator(_M_insert_after(__slist_previous(&this->_M_head,
                                                      __pos._M_node),
                                      value_type()));
    }

    void insert(iterator __pos, size_type __n, const value_type &__x)
    {
      _M_insert_after_fill(__slist_previous(&this->_M_head, __pos._M_node),
                          __n, __x);
    }

    void insert(iterator __pos, const_iterator __first, const_iterator __last)
    {
      _M_insert_after_range(__slist_previous(&this->_M_head, __pos._M_node),
                            __first, __last);
    }
    void insert(iterator __pos, const value_type *__first,
                const value_type *__last)
    {
      _M_insert_after_range(__slist_previous(&this->_M_head, __pos._M_node),
                            __first, __last);
    }

  public:
    iterator erase_after(iterator __pos)
    {
      return iterator((_Node *)this->_M_erase_after(__pos._M_node));
    }
    iterator erase_after(iterator __before_first, iterator __last)
    {
      return iterator((_Node *)this->_M_erase_after(__before_first._M_node,
                                                    __last._M_node));
    }

    iterator erase(iterator __pos)
    {
      return (_Node *)this->_M_erase_after(__slist_previous(&this->_M_head,
                                                            __pos._M_node));
    }
    iterator erase(iterator __first, iterator __last)
    {
      return (_Node *)this->_M_erase_after(
          __slist_previous(&this->_M_head, __first._M_node), __last._M_node);
    }

    void resize(size_type new_size, const _Tp &__x);
    void resize(size_type new_size) { resize(new_size, _Tp()); }
    void clear() { this->_M_erase_after(&this->_M_head, 0); }

  public:
    // Moves the range [__before_first + 1, __before_last + 1) to *this,
    //  inserting it immediately after __pos.  This is constant time.
    void splice_after(iterator __pos,
                      iterator __before_first, iterator __before_last)
    {
      if (__before_first != __before_last)
        __slist_splice_after(__pos._M_node, __before_first._M_node,
                            __before_last._M_node);
    }

    // Moves the element that follows __prev to *this, inserting it immediately
    //  after __pos.  This is constant time.
    void splice_after(iterator __pos, iterator __prev)
    {
      __slist_splice_after(__pos._M_node,
                          __prev._M_node, __prev._M_node->_M_next);
    }

    // Removes all of the elements from the list __x to *this, inserting
    // them immediately after __pos.  __x must not be *this.  Complexity:
    // linear in __x.size().
    void splice_after(iterator __pos, slist &__x)
    {
      __slist_splice_after(__pos._M_node, &__x._M_head);
    }

    // Linear in distance(begin(), __pos), and linear in __x.size().
    void splice(iterator __pos, slist &__x)
    {
      if (__x._M_head._M_next)
        __slist_splice_after(__slist_previous(&this->_M_head, __pos._M_node),
                            &__x._M_head, __slist_previous(&__x._M_head, 0));
    }

    // Linear in distance(begin(), __pos), and in distance(__x.begin(), __i).
    void splice(iterator __pos, slist &__x, iterator __i)
    {
      __slist_splice_after(__slist_previous(&this->_M_head, __pos._M_node),
                          __slist_previous(&__x._M_head, __i._M_node),
                          __i._M_node);
    }

    // Linear in distance(begin(), __pos), in distance(__x.begin(), __first),
    // and in distance(__first, __last).
    void splice(iterator __pos, slist &__x, iterator __first, iterator __last)
    {
      if (__first != __last)
        __slist_splice_after(__slist_previous(&this->_M_head, __pos._M_node),
                            __slist_previous(&__x._M_head, __first._M_node),
                            __slist_previous(__first._M_node, __last._M_node));
    }

  public:
    void reverse()
    {
      if (this->_M_head._M_next)
        this->_M_head._M_next = __slist_reverse(this->_M_head._M_next);
    }

    void remove(const _Tp &__val);
    void unique();
    void merge(slist &__x);
    void sort();
  };

  template <class _Tp, class _Alloc>
  slist<_Tp, _Alloc> &slist<_Tp, _Alloc>::operator=(const slist<_Tp, _Alloc> &__x)
  {
    if (&__x != this)
    {
      _Node_base *__p1 = &this->_M_head;
      _Node *__n1 = (_Node *)this->_M_head._M_next;
      const _Node *__n2 = (const _Node *)__x._M_head._M_next;
      while (__n1 && __n2)
      {
        __n1->_M_data = __n2->_M_data;
        __p1 = __n1;
        __n1 = (_Node *)__n1->_M_next;
        __n2 = (const _Node *)__n2->_M_next;
      }
      if (__n2 == 0)
        this->_M_erase_after(__p1, 0);
      else
        _M_insert_after_range(__p1, const_iterator((_Node *)__n2),
                              const_iterator(0));
    }
    return *this;
  }

  template <class _Tp, class _Alloc>
  void slist<_Tp, _Alloc>::_M_fill_assign(size_type __n, const _Tp &__val)
  {
    _Node_base *__prev = &this->_M_head;
    _Node *__node = (_Node *)this->_M_head._M_next;
    for (; __node != 0 && __n > 0; --__n)
    {
      __node->_M_data = __val;
      __prev = __node;
      __node = (_Node *)__node->_M_next;
    }
    if (__n > 0)
      _M_insert_after_fill(__prev, __n, __val);
    else
      this->_M_erase_after(__prev, 0);
  }

  template <class _Tp, class _Alloc>
  inline bool
  operator==(const slist<_Tp, _Alloc> &_SL1, const slist<_Tp, _Alloc> &_SL2)
  {
    typedef typename slist<_Tp, _Alloc>::const_iterator const_iterator;
    const_iterator __end1 = _SL1.end();
    const_iterator __end2 = _SL2.end();

    const_iterator __i1 = _SL1.begin();
    const_iterator __i2 = _SL2.begin();
    while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2)
    {
      ++__i1;
      ++__i2;
    }
    return __i1 == __end1 && __i2 == __end2;
  }

  template <class _Tp, class _Alloc>
  inline bool
  operator<(const slist<_Tp, _Alloc> &_SL1, const slist<_Tp, _Alloc> &_SL2)
  {
    return lexicographical_compare(_SL1.begin(), _SL1.end(),
                                  _SL2.begin(), _SL2.end());
  }

  template <class _Tp, class _Alloc>
  void slist<_Tp, _Alloc>::resize(size_type __len, const _Tp &__x)
  {
    _Node_base *__cur = &this->_M_head;
    while (__cur->_M_next != 0 && __len > 0)
    {
      --__len;
      __cur = __cur->_M_next;
    }
    if (__cur->_M_next)
      this->_M_erase_after(__cur, 0);
    else
      _M_insert_after_fill(__cur, __len, __x);
  }

  template <class _Tp, class _Alloc>
  void slist<_Tp, _Alloc>::remove(const _Tp &__val)
  {
    _Node_base *__cur = &this->_M_head;
    while (__cur && __cur->_M_next)
    {
      if (((_Node *)__cur->_M_next)->_M_data == __val)
        this->_M_erase_after(__cur);
      else
        __cur = __cur->_M_next;
    }
  }

  template <class _Tp, class _Alloc>
  void slist<_Tp, _Alloc>::unique()
  {
    _Node_base *__cur = this->_M_head._M_next;
    if (__cur)
    {
      while (__cur->_M_next)
      {
        if (((_Node *)__cur)->_M_data ==
            ((_Node *)(__cur->_M_next))->_M_data)
          this->_M_erase_after(__cur);
        else
          __cur = __cur->_M_next;
      }
    }
  }

  template <class _Tp, class _Alloc>
  void slist<_Tp, _Alloc>::merge(slist<_Tp, _Alloc> &__x)
  {
    _Node_base *__n1 = &this->_M_head;
    while (__n1->_M_next && __x._M_head._M_next)
    {
      if (((_Node *)__x._M_head._M_next)->_M_data <
          ((_Node *)__n1->_M_next)->_M_data)
        __slist_splice_after(__n1, &__x._M_head, __x._M_head._M_next);
      __n1 = __n1->_M_next;
    }
    if (__x._M_head._M_next)
    {
      __n1->_M_next = __x._M_head._M_next;
      __x._M_head._M_next = 0;
    }
  }

  template <class _Tp, class _Alloc>
  void slist<_Tp, _Alloc>::sort()
  {
    if (this->_M_head._M_next && this->_M_head._M_next->_M_next)
    {
      slist __carry;
      slist __counter[64];
      int __fill = 0;
      while (!empty())
      {
        __slist_splice_after(&__carry._M_head,
                            &this->_M_head, this->_M_head._M_next);
        int __i = 0;
        while (__i < __fill && !__counter[__i].empty())
        {
          __counter[__i].merge(__carry);
          __carry.swap(__counter[__i]);
          ++__i;
        }
        __carry.swap(__counter[__i]);
        if (__i == __fill)
          ++__fill;
      }

      for (int __i = 1; __i < __fill; ++__i)
        __counter[__i].merge(__counter[__i - 1]);
      this->swap(__counter[__fill - 1]);
    }
  }
  ```
  </details>

#### 容器```deque```

  ![容器deque](https://github.com/gongluck/images/blob/main/stl/容器deque.png)

  ```deque```包含指向管控中心的指针```m_map```、控制中心的大小和分别指向控制中心开始和结束的两个```迭代器```。
  ```deque::iterator```包含```3```个```T*```类型的指针和```1```个```T**```类型指针。分别为```cur```、```first```、 ```last```指向直接内存的位置，```m_node```指向管控中心的节点位置。

  [stl_deque.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_deque.h)

  <details>
  <summary>deque</summary>

  ```C++
  // Note: this function is simply a kludge to work around several compilers'
  //  bugs in handling constant expressions.
  inline size_t __deque_buf_size(size_t __size)
  {
    //字节数>=512Byte
    //元素个数>=1
    return __size < 512 ? size_t(512 / __size) : size_t(1);
  }

  //双向队列迭代器
  template <class _Tp, class _Ref, class _Ptr>
  struct _Deque_iterator
  {
    typedef _Deque_iterator<_Tp, _Tp &, _Tp *> iterator;
    typedef _Deque_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;
    //计算单元格容纳的节点数
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }

    typedef random_access_iterator_tag iterator_category; //可随机访问类别
    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp **_Map_pointer; //控制中心

    typedef _Deque_iterator _Self;

    _Tp *_M_cur;          //指向当前元素
    _Tp *_M_first;        //指向当前单元的开始
    _Tp *_M_last;         //指向当前单元的结束
    _Map_pointer _M_node; //指向控制中心

    _Deque_iterator(_Tp *__x, _Map_pointer __y)
        : _M_cur(__x), _M_first(*__y),
          _M_last(*__y + _S_buffer_size()), _M_node(__y) {}
    _Deque_iterator() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) {}
    _Deque_iterator(const iterator &__x)
        : _M_cur(__x._M_cur), _M_first(__x._M_first),
          _M_last(__x._M_last), _M_node(__x._M_node) {}

    //解引用实际操作指向的当前元素
    reference operator*() const { return *_M_cur; }
  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    //成员指向符号实际操作指向的当前元素
    pointer operator->() const
    {
      return _M_cur;
    }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */

    //减号运算计算距离，相差的元素个数
    difference_type operator-(const _Self &__x) const
    {
      // difference_type(_S_buffer_size()) 一个单元格的节点数
      // (_M_node - __x._M_node - 1) 前面的单元格数-1
      // (_M_cur - _M_first) + (__x._M_last - __x._M_cur) 一个单元格+差值
      return difference_type(_S_buffer_size()) * (_M_node - __x._M_node - 1) +
            (_M_cur - _M_first) + (__x._M_last - __x._M_cur);
    }

    _Self &operator++()
    {
      //当前单元格游标递增
      ++_M_cur;
      //判断是否超出当前单元格
      if (_M_cur == _M_last)
      {
        //使用下一个单元格
        _M_set_node(_M_node + 1);
        _M_cur = _M_first;
      }
      return *this;
    }
    //后置++
    _Self operator++(int)
    {
      _Self __tmp = *this;
      ++*this;
      return __tmp;
    }

    _Self &operator--()
    {
      //判断是否当前单元格的第一个元素
      if (_M_cur == _M_first)
      {
        //使用上一个单元格
        _M_set_node(_M_node - 1);
        _M_cur = _M_last;
      }
      //当前游标向前偏移
      --_M_cur;
      return *this;
    }
    //后置--
    _Self operator--(int)
    {
      _Self __tmp = *this;
      --*this;
      return __tmp;
    }

    _Self &operator+=(difference_type __n)
    {
      //计算目标点距离当前单元格开始的偏移
      difference_type __offset = __n + (_M_cur - _M_first);
      //偏移在当前单元格，直接运算
      if (__offset >= 0 && __offset < difference_type(_S_buffer_size()))
        _M_cur += __n;
      //偏移超出单元格
      else
      {
        //计算单元格在控制中心的偏移
        difference_type __node_offset =
            __offset > 0 ? __offset / difference_type(_S_buffer_size())
                        : -difference_type((-__offset - 1) / _S_buffer_size()) - 1;
        //移动单元格
        _M_set_node(_M_node + __node_offset);
        //游标偏移
        _M_cur = _M_first +
                (__offset - __node_offset * difference_type(_S_buffer_size()));
      }
      return *this;
    }

    // 3个const版本
    _Self operator+(difference_type __n) const
    {
      _Self __tmp = *this;
      return __tmp += __n;
    }
    _Self &operator-=(difference_type __n) { return *this += -__n; }
    _Self operator-(difference_type __n) const
    {
      _Self __tmp = *this;
      return __tmp -= __n;
    }

    //下标运算符
    reference operator[](difference_type __n) const { return *(*this + __n); }

    bool operator==(const _Self &__x) const { return _M_cur == __x._M_cur; }
    bool operator!=(const _Self &__x) const { return !(*this == __x); }
    bool operator<(const _Self &__x) const
    {
      return (_M_node == __x._M_node) ? (_M_cur < __x._M_cur) : (_M_node < __x._M_node);
    }
    bool operator>(const _Self &__x) const { return __x < *this; }
    bool operator<=(const _Self &__x) const { return !(__x < *this); }
    bool operator>=(const _Self &__x) const { return !(*this < __x); }

    //设置单元格
    void _M_set_node(_Map_pointer __new_node)
    {
      _M_node = __new_node;
      _M_first = *__new_node;
      _M_last = _M_first + difference_type(_S_buffer_size());
    }
  };

  template <class _Tp, class _Ref, class _Ptr>
  inline _Deque_iterator<_Tp, _Ref, _Ptr>
  operator+(ptrdiff_t __n, const _Deque_iterator<_Tp, _Ref, _Ptr> &__x)
  {
    return __x + __n;
  }

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  template <class _Tp, class _Ref, class _Ptr>
  inline random_access_iterator_tag
  iterator_category(const _Deque_iterator<_Tp, _Ref, _Ptr> &)
  {
    return random_access_iterator_tag();
  }

  template <class _Tp, class _Ref, class _Ptr>
  inline _Tp *value_type(const _Deque_iterator<_Tp, _Ref, _Ptr> &) { return 0; }

  template <class _Tp, class _Ref, class _Ptr>
  inline ptrdiff_t *distance_type(const _Deque_iterator<_Tp, _Ref, _Ptr> &)
  {
    return 0;
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  // Deque base class.  It has two purposes.  First, its constructor
  //  and destructor allocate (but don't initialize) storage.  This makes
  //  exception safety easier.  Second, the base class encapsulates all of
  //  the differences between SGI-style allocators and standard-conforming
  //  allocators.

  //双向链表基类
  template <class _Tp, class _Alloc>
  class _Deque_base
  {
  public:
    typedef _Deque_iterator<_Tp, _Tp &, _Tp *> iterator;
    typedef _Deque_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;

    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    _Deque_base(const allocator_type &, size_t __num_elements)
        : _M_map(0), _M_map_size(0), _M_start(), _M_finish()
    {
      //初始化控制中心
      _M_initialize_map(__num_elements);
    }
    _Deque_base(const allocator_type &)
        : _M_map(0), _M_map_size(0), _M_start(), _M_finish() {}
    ~_Deque_base();

  protected:
    void _M_initialize_map(size_t);
    void _M_create_nodes(_Tp **__nstart, _Tp **__nfinish);
    void _M_destroy_nodes(_Tp **__nstart, _Tp **__nfinish);
    enum
    {
      _S_initial_map_size = 8
    };

  protected:
    _Tp **_M_map;       //控制中心
    size_t _M_map_size; //控制中心大小
    iterator _M_start;  //控制中心逻辑起点
    iterator _M_finish; //控制中心逻辑结束

    typedef simple_alloc<_Tp, _Alloc> _Node_alloc_type;
    typedef simple_alloc<_Tp *, _Alloc> _Map_alloc_type;

    //分配一个单元格
    _Tp *_M_allocate_node()
    {
      return _Node_alloc_type::allocate(__deque_buf_size(sizeof(_Tp)));
    }
    //销毁单元格
    void _M_deallocate_node(_Tp *__p)
    {
      _Node_alloc_type::deallocate(__p, __deque_buf_size(sizeof(_Tp)));
    }
    //分配控制中心
    _Tp **_M_allocate_map(size_t __n)
    {
      return _Map_alloc_type::allocate(__n);
    }
    //销毁控制中心
    void _M_deallocate_map(_Tp **__p, size_t __n)
    {
      _Map_alloc_type::deallocate(__p, __n);
    }
  };

  // Non-inline member functions from _Deque_base.

  template <class _Tp, class _Alloc>
  _Deque_base<_Tp, _Alloc>::~_Deque_base()
  {
    if (_M_map)
    {
      //调用节点的析构
      _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1);
      //销毁控制中心
      _M_deallocate_map(_M_map, _M_map_size);
    }
  }

  //初始化控制中心
  template <class _Tp, class _Alloc>
  void _Deque_base<_Tp, _Alloc>::_M_initialize_map(size_t __num_elements)
  {
    //计算单元格数
    size_t __num_nodes =
        __num_elements / __deque_buf_size(sizeof(_Tp)) + 1;

    //计算控制中心大小
    _M_map_size = max((size_t)_S_initial_map_size, __num_nodes + 2);
    //分配控制中心
    _M_map = _M_allocate_map(_M_map_size);

    //控制中心从中部开始使用
    _Tp **__nstart = _M_map + (_M_map_size - __num_nodes) / 2;
    _Tp **__nfinish = __nstart + __num_nodes;

    __STL_TRY
    {
      //创建单元格
      _M_create_nodes(__nstart, __nfinish);
    }
    __STL_UNWIND((_M_deallocate_map(_M_map, _M_map_size),
                  _M_map = 0, _M_map_size = 0));
    //设置控制中心逻辑起点和结束
    _M_start._M_set_node(__nstart);
    _M_finish._M_set_node(__nfinish - 1);
    _M_start._M_cur = _M_start._M_first;
    _M_finish._M_cur = _M_finish._M_first +
                      __num_elements % __deque_buf_size(sizeof(_Tp));
  }

  //在[start, finish)直接分配单元格
  template <class _Tp, class _Alloc>
  void _Deque_base<_Tp, _Alloc>::_M_create_nodes(_Tp **__nstart, _Tp **__nfinish)
  {
    _Tp **__cur;
    __STL_TRY
    {
      for (__cur = __nstart; __cur < __nfinish; ++__cur)
        *__cur = _M_allocate_node(); //分配一个单元格
    }
    __STL_UNWIND(_M_destroy_nodes(__nstart, __cur));
  }

  //销毁单元格
  template <class _Tp, class _Alloc>
  void _Deque_base<_Tp, _Alloc>::_M_destroy_nodes(_Tp **__nstart, _Tp **__nfinish)
  {
    for (_Tp **__n = __nstart; __n < __nfinish; ++__n)
      _M_deallocate_node(*__n);
  }

  //双端队列
  template <class _Tp, class _Alloc = __STL_DEFAULT_ALLOCATOR(_Tp)>
  class deque : protected _Deque_base<_Tp, _Alloc>
  {

    // requirements:

    __STL_CLASS_REQUIRES(_Tp, _Assignable);

    typedef _Deque_base<_Tp, _Alloc> _Base;

  public: // Basic types
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return _Base::get_allocator(); }

  public: // Iterators
    typedef typename _Base::iterator iterator;
    typedef typename _Base::const_iterator const_iterator;

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

  protected: // Internal typedefs
    typedef pointer *_Map_pointer;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }

  protected:
  #ifdef __STL_USE_NAMESPACES
    using _Base::_M_allocate_map;
    using _Base::_M_allocate_node;
    using _Base::_M_create_nodes;
    using _Base::_M_deallocate_map;
    using _Base::_M_deallocate_node;
    using _Base::_M_destroy_nodes;
    using _Base::_M_initialize_map;

    using _Base::_M_finish;
    using _Base::_M_map;
    using _Base::_M_map_size;
    using _Base::_M_start;
  #endif /* __STL_USE_NAMESPACES */

  public: // Basic accessors
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }

    reverse_iterator rbegin() { return reverse_iterator(_M_finish); }
    reverse_iterator rend() { return reverse_iterator(_M_start); }
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(_M_finish);
    }
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(_M_start);
    }

    reference operator[](size_type __n)
    {
      return _M_start[difference_type(__n)];
    }
    const_reference operator[](size_type __n) const
    {
      return _M_start[difference_type(__n)];
    }

  #ifdef __STL_THROW_RANGE_ERRORS
    void _M_range_check(size_type __n) const
    {
      if (__n >= this->size())
        __stl_throw_range_error("deque");
    }

    reference at(size_type __n)
    {
      _M_range_check(__n);
      return (*this)[__n];
    }
    const_reference at(size_type __n) const
    {
      _M_range_check(__n);
      return (*this)[__n];
    }
  #endif /* __STL_THROW_RANGE_ERRORS */

    reference front()
    {
      return *_M_start;
    }
    reference back()
    {
      iterator __tmp = _M_finish;
      --__tmp;
      return *__tmp;
    }
    const_reference front() const { return *_M_start; }
    const_reference back() const
    {
      const_iterator __tmp = _M_finish;
      --__tmp;
      return *__tmp;
    }

    //直接使用迭代器的-运算！
    size_type size() const { return _M_finish - _M_start; }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return _M_finish == _M_start; }

  public: // Constructor, destructor.
    explicit deque(const allocator_type &__a = allocator_type())
        : _Base(__a, 0) {}
    //因为迭代器实现了++、*等运算符，所以可以使用内存工具函数
    deque(const deque &__x) : _Base(__x.get_allocator(), __x.size())
    {
      uninitialized_copy(__x.begin(), __x.end(), _M_start);
    }
    deque(size_type __n, const value_type &__value,
          const allocator_type &__a = allocator_type()) : _Base(__a, __n)
    {
      _M_fill_initialize(__value);
    }
    explicit deque(size_type __n) : _Base(allocator_type(), __n)
    {
      _M_fill_initialize(value_type());
    }

    deque(const value_type *__first, const value_type *__last,
          const allocator_type &__a = allocator_type())
        : _Base(__a, __last - __first)
    {
      uninitialized_copy(__first, __last, _M_start);
    }
    deque(const_iterator __first, const_iterator __last,
          const allocator_type &__a = allocator_type())
        : _Base(__a, __last - __first)
    {
      uninitialized_copy(__first, __last, _M_start);
    }

    ~deque()
    {
      //析构所有节点
      destroy(_M_start, _M_finish);
    }

    deque &operator=(const deque &__x)
    {
      const size_type __len = size();
      if (&__x != this)
      {
        if (__len >= __x.size())
          erase(copy(__x.begin(), __x.end(), _M_start), _M_finish);
        else
        {
          const_iterator __mid = __x.begin() + difference_type(__len);
          copy(__x.begin(), __mid, _M_start);
          insert(_M_finish, __mid, __x.end());
        }
      }
      return *this;
    }

    //交换
    void swap(deque &__x)
    {
      __STD::swap(_M_start, __x._M_start);
      __STD::swap(_M_finish, __x._M_finish);
      __STD::swap(_M_map, __x._M_map);
      __STD::swap(_M_map_size, __x._M_map_size);
    }

  public:
    // assign(), a generalized assignment member function.  Two
    // versions: one that takes a count, and one that takes a range.
    // The range version is a member template, so we dispatch on whether
    // or not the type is an integer.

    void _M_fill_assign(size_type __n, const _Tp &__val)
    {
      if (__n > size())
      {
        fill(begin(), end(), __val);
        insert(end(), __n - size(), __val);
      }
      else
      {
        erase(begin() + __n, end());
        fill(begin(), end(), __val);
      }
    }

    void assign(size_type __n, const _Tp &__val)
    {
      _M_fill_assign(__n, __val);
    }

  public: // push_* and pop_*
    void push_back(const value_type &__t)
    {
      //不超出当前单元格
      if (_M_finish._M_cur != _M_finish._M_last - 1)
      {
        //原地构造
        construct(_M_finish._M_cur, __t);
        //游标偏移
        ++_M_finish._M_cur;
      }
      else
        _M_push_back_aux(__t);
    }

    void push_back()
    {
      if (_M_finish._M_cur != _M_finish._M_last - 1)
      {
        construct(_M_finish._M_cur);
        ++_M_finish._M_cur;
      }
      else
        _M_push_back_aux();
    }

    void push_front(const value_type &__t)
    {
      //不超出当前单元格
      if (_M_start._M_cur != _M_start._M_first)
      {
        //原地构造
        construct(_M_start._M_cur - 1, __t);
        //游标偏移，这里会偏移单元格ƒ
        --_M_start._M_cur;
      }
      else
        _M_push_front_aux(__t);
    }

    void push_front()
    {
      if (_M_start._M_cur != _M_start._M_first)
      {
        construct(_M_start._M_cur - 1);
        --_M_start._M_cur;
      }
      else
        _M_push_front_aux();
    }

    void pop_back()
    {
      if (_M_finish._M_cur != _M_finish._M_first)
      {
        --_M_finish._M_cur;
        destroy(_M_finish._M_cur);
      }
      else
        _M_pop_back_aux();
    }

    void pop_front()
    {
      if (_M_start._M_cur != _M_start._M_last - 1)
      {
        destroy(_M_start._M_cur);
        ++_M_start._M_cur;
      }
      else
        _M_pop_front_aux();
    }

  public: // Insert
    iterator insert(iterator position, const value_type &__x)
    {
      if (position._M_cur == _M_start._M_cur)
      {
        push_front(__x);
        return _M_start;
      }
      else if (position._M_cur == _M_finish._M_cur)
      {
        push_back(__x);
        iterator __tmp = _M_finish;
        --__tmp;
        return __tmp;
      }
      else
      {
        return _M_insert_aux(position, __x);
      }
    }

    iterator insert(iterator __position)
    {
      return insert(__position, value_type());
    }

    void insert(iterator __pos, size_type __n, const value_type &__x)
    {
      _M_fill_insert(__pos, __n, __x);
    }

    void _M_fill_insert(iterator __pos, size_type __n, const value_type &__x);

    void insert(iterator __pos,
                const value_type *__first, const value_type *__last);
    void insert(iterator __pos,
                const_iterator __first, const_iterator __last);

    void resize(size_type __new_size, const value_type &__x)
    {
      const size_type __len = size();
      if (__new_size < __len)
        erase(_M_start + __new_size, _M_finish);
      else
        insert(_M_finish, __new_size - __len, __x);
    }

    void resize(size_type new_size) { resize(new_size, value_type()); }

  public: // Erase
    //选择较小的一端移动所有元素
    iterator erase(iterator __pos)
    {
      iterator __next = __pos;
      ++__next;
      size_type __index = __pos - _M_start;
      if (__index < (size() >> 1))
      {
        copy_backward(_M_start, __pos, __next);
        pop_front();
      }
      else
      {
        copy(__next, _M_finish, __pos);
        pop_back();
      }
      return _M_start + __index;
    }

    iterator erase(iterator __first, iterator __last);
    void clear();

  protected: // Internal construction/destruction
    void _M_fill_initialize(const value_type &__value);

  protected: // Internal push_* and pop_*
    void _M_push_back_aux(const value_type &);
    void _M_push_back_aux();
    void _M_push_front_aux(const value_type &);
    void _M_push_front_aux();
    void _M_pop_back_aux();
    void _M_pop_front_aux();

  protected: // Internal insert functions
    iterator _M_insert_aux(iterator __pos, const value_type &__x);
    iterator _M_insert_aux(iterator __pos);
    void _M_insert_aux(iterator __pos, size_type __n, const value_type &__x);

    void _M_insert_aux(iterator __pos,
                      const value_type *__first, const value_type *__last,
                      size_type __n);

    void _M_insert_aux(iterator __pos,
                      const_iterator __first, const_iterator __last,
                      size_type __n);

    //向前扩充
    iterator _M_reserve_elements_at_front(size_type __n)
    {
      size_type __vacancies = _M_start._M_cur - _M_start._M_first;
      if (__n > __vacancies)
        _M_new_elements_at_front(__n - __vacancies);
      return _M_start - difference_type(__n);
    }

    //向后扩充
    iterator _M_reserve_elements_at_back(size_type __n)
    {
      size_type __vacancies = (_M_finish._M_last - _M_finish._M_cur) - 1;
      if (__n > __vacancies)
        _M_new_elements_at_back(__n - __vacancies);
      return _M_finish + difference_type(__n);
    }

    void _M_new_elements_at_front(size_type __new_elements);
    void _M_new_elements_at_back(size_type __new_elements);

  protected: // Allocation of _M_map and nodes
    // Makes sure the _M_map has space for new nodes.  Does not actually
    //  add the nodes.  Can invalidate _M_map pointers.  (And consequently,
    //  deque iterators.)

    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
      //储备量不足，要重新分配新内存
      if (__nodes_to_add + 1 > _M_map_size - (_M_finish._M_node - _M_map))
        _M_reallocate_map(__nodes_to_add, false);
    }

    void _M_reserve_map_at_front(size_type __nodes_to_add = 1)
    {
      //储备量不足，要重新分配新内存
      if (__nodes_to_add > size_type(_M_start._M_node - _M_map))
        _M_reallocate_map(__nodes_to_add, true);
    }

    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);
  };

  // Non-inline member functions

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_fill_insert(iterator __pos,
                                          size_type __n, const value_type &__x)
  {
    if (__pos._M_cur == _M_start._M_cur)
    {
      //头部扩充内存
      iterator __new_start = _M_reserve_elements_at_front(__n);
      __STL_TRY
      {
        // fill
        uninitialized_fill(__new_start, _M_start, __x);
        _M_start = __new_start;
      }
      __STL_UNWIND(_M_destroy_nodes(__new_start._M_node, _M_start._M_node));
    }
    else if (__pos._M_cur == _M_finish._M_cur)
    {
      //尾部扩充内存
      iterator __new_finish = _M_reserve_elements_at_back(__n);
      __STL_TRY
      {
        uninitialized_fill(_M_finish, __new_finish, __x);
        _M_finish = __new_finish;
      }
      __STL_UNWIND(_M_destroy_nodes(_M_finish._M_node + 1,
                                    __new_finish._M_node + 1));
    }
    else
      _M_insert_aux(__pos, __n, __x);
  }

  #ifndef __STL_MEMBER_TEMPLATES

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::insert(iterator __pos,
                                  const value_type *__first,
                                  const value_type *__last)
  {
    size_type __n = __last - __first;
    if (__pos._M_cur == _M_start._M_cur)
    {
      iterator __new_start = _M_reserve_elements_at_front(__n);
      __STL_TRY
      {
        uninitialized_copy(__first, __last, __new_start);
        _M_start = __new_start;
      }
      __STL_UNWIND(_M_destroy_nodes(__new_start._M_node, _M_start._M_node));
    }
    else if (__pos._M_cur == _M_finish._M_cur)
    {
      iterator __new_finish = _M_reserve_elements_at_back(__n);
      __STL_TRY
      {
        uninitialized_copy(__first, __last, _M_finish);
        _M_finish = __new_finish;
      }
      __STL_UNWIND(_M_destroy_nodes(_M_finish._M_node + 1,
                                    __new_finish._M_node + 1));
    }
    else
      _M_insert_aux(__pos, __first, __last, __n);
  }

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::insert(iterator __pos,
                                  const_iterator __first, const_iterator __last)
  {
    size_type __n = __last - __first;
    if (__pos._M_cur == _M_start._M_cur)
    {
      iterator __new_start = _M_reserve_elements_at_front(__n);
      __STL_TRY
      {
        uninitialized_copy(__first, __last, __new_start);
        _M_start = __new_start;
      }
      __STL_UNWIND(_M_destroy_nodes(__new_start._M_node, _M_start._M_node));
    }
    else if (__pos._M_cur == _M_finish._M_cur)
    {
      iterator __new_finish = _M_reserve_elements_at_back(__n);
      __STL_TRY
      {
        uninitialized_copy(__first, __last, _M_finish);
        _M_finish = __new_finish;
      }
      __STL_UNWIND(_M_destroy_nodes(_M_finish._M_node + 1,
                                    __new_finish._M_node + 1));
    }
    else
      _M_insert_aux(__pos, __first, __last, __n);
  }

  #endif /* __STL_MEMBER_TEMPLATES */

  template <class _Tp, class _Alloc>
  typename deque<_Tp, _Alloc>::iterator
  deque<_Tp, _Alloc>::erase(iterator __first, iterator __last)
  {
    if (__first == _M_start && __last == _M_finish)
    {
      clear();
      return _M_finish;
    }
    else
    {
      //下面算法画图理解比较容易！
      difference_type __n = __last - __first;
      difference_type __elems_before = __first - _M_start;
      if (static_cast<size_type>(__elems_before) < (size() - __n) / 2)
      {
        copy_backward(_M_start, __first, __last);
        iterator __new_start = _M_start + __n;
        destroy(_M_start, __new_start);
        _M_destroy_nodes(__new_start._M_node, _M_start._M_node);
        _M_start = __new_start;
      }
      else
      {
        copy(__last, _M_finish, __first);
        iterator __new_finish = _M_finish - __n;
        destroy(__new_finish, _M_finish);
        _M_destroy_nodes(__new_finish._M_node + 1, _M_finish._M_node + 1);
        _M_finish = __new_finish;
      }
      return _M_start + __elems_before;
    }
  }

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::clear()
  {
    for (_Map_pointer __node = _M_start._M_node + 1;
        __node < _M_finish._M_node;
        ++__node)
    {
      destroy(*__node, *__node + _S_buffer_size());
      _M_deallocate_node(*__node);
    }

    if (_M_start._M_node != _M_finish._M_node)
    {
      destroy(_M_start._M_cur, _M_start._M_last);
      destroy(_M_finish._M_first, _M_finish._M_cur);
      _M_deallocate_node(_M_finish._M_first);
    }
    else
      destroy(_M_start._M_cur, _M_finish._M_cur);

    _M_finish = _M_start;
  }

  // Precondition: _M_start and _M_finish have already been initialized,
  // but none of the deque's elements have yet been constructed.
  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_fill_initialize(const value_type &__value)
  {
    _Map_pointer __cur;
    __STL_TRY
    {
      for (__cur = _M_start._M_node; __cur < _M_finish._M_node; ++__cur)
        uninitialized_fill(*__cur, *__cur + _S_buffer_size(), __value);
      uninitialized_fill(_M_finish._M_first, _M_finish._M_cur, __value);
    }
    __STL_UNWIND(destroy(_M_start, iterator(*__cur, __cur)));
  }

  // Called only if _M_finish._M_cur == _M_finish._M_last - 1.
  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_push_back_aux(const value_type &__t)
  {
    value_type __t_copy = __t;
    _M_reserve_map_at_back();
    *(_M_finish._M_node + 1) = _M_allocate_node();
    __STL_TRY
    {
      construct(_M_finish._M_cur, __t_copy);
      _M_finish._M_set_node(_M_finish._M_node + 1);
      _M_finish._M_cur = _M_finish._M_first;
    }
    __STL_UNWIND(_M_deallocate_node(*(_M_finish._M_node + 1)));
  }

  // Called only if _M_finish._M_cur == _M_finish._M_last - 1.
  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_push_back_aux()
  {
    _M_reserve_map_at_back();
    *(_M_finish._M_node + 1) = _M_allocate_node();
    __STL_TRY
    {
      construct(_M_finish._M_cur);
      _M_finish._M_set_node(_M_finish._M_node + 1);
      _M_finish._M_cur = _M_finish._M_first;
    }
    __STL_UNWIND(_M_deallocate_node(*(_M_finish._M_node + 1)));
  }

  // Called only if _M_start._M_cur == _M_start._M_first.
  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_push_front_aux(const value_type &__t)
  {
    value_type __t_copy = __t;
    _M_reserve_map_at_front();
    *(_M_start._M_node - 1) = _M_allocate_node();
    __STL_TRY
    {
      _M_start._M_set_node(_M_start._M_node - 1);
      _M_start._M_cur = _M_start._M_last - 1;
      construct(_M_start._M_cur, __t_copy);
    }
    __STL_UNWIND((++_M_start, _M_deallocate_node(*(_M_start._M_node - 1))));
  }

  // Called only if _M_start._M_cur == _M_start._M_first.
  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_push_front_aux()
  {
    _M_reserve_map_at_front();
    *(_M_start._M_node - 1) = _M_allocate_node();
    __STL_TRY
    {
      _M_start._M_set_node(_M_start._M_node - 1);
      _M_start._M_cur = _M_start._M_last - 1;
      construct(_M_start._M_cur);
    }
    __STL_UNWIND((++_M_start, _M_deallocate_node(*(_M_start._M_node - 1))));
  }

  // Called only if _M_finish._M_cur == _M_finish._M_first.
  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_pop_back_aux()
  {
    _M_deallocate_node(_M_finish._M_first);
    _M_finish._M_set_node(_M_finish._M_node - 1);
    _M_finish._M_cur = _M_finish._M_last - 1;
    destroy(_M_finish._M_cur);
  }

  // Called only if _M_start._M_cur == _M_start._M_last - 1.  Note that
  // if the deque has at least one element (a precondition for this member
  // function), and if _M_start._M_cur == _M_start._M_last, then the deque
  // must have at least two nodes.
  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_pop_front_aux()
  {
    destroy(_M_start._M_cur);
    _M_deallocate_node(_M_start._M_first);
    _M_start._M_set_node(_M_start._M_node + 1);
    _M_start._M_cur = _M_start._M_first;
  }

  template <class _Tp, class _Alloc>
  typename deque<_Tp, _Alloc>::iterator
  deque<_Tp, _Alloc>::_M_insert_aux(iterator __pos, const value_type &__x)
  {
    difference_type __index = __pos - _M_start;
    value_type __x_copy = __x;
    if (static_cast<size_type>(__index) < size() / 2)
    {
      push_front(front());
      iterator __front1 = _M_start;
      ++__front1;
      iterator __front2 = __front1;
      ++__front2;
      __pos = _M_start + __index;
      iterator __pos1 = __pos;
      ++__pos1;
      copy(__front2, __pos1, __front1);
    }
    else
    {
      push_back(back());
      iterator __back1 = _M_finish;
      --__back1;
      iterator __back2 = __back1;
      --__back2;
      __pos = _M_start + __index;
      copy_backward(__pos, __back2, __back1);
    }
    *__pos = __x_copy;
    return __pos;
  }

  template <class _Tp, class _Alloc>
  typename deque<_Tp, _Alloc>::iterator
  deque<_Tp, _Alloc>::_M_insert_aux(iterator __pos)
  {
    difference_type __index = __pos - _M_start;
    if (static_cast<size_type>(__index) < size() / 2)
    {
      push_front(front());
      iterator __front1 = _M_start;
      ++__front1;
      iterator __front2 = __front1;
      ++__front2;
      __pos = _M_start + __index;
      iterator __pos1 = __pos;
      ++__pos1;
      copy(__front2, __pos1, __front1);
    }
    else
    {
      push_back(back());
      iterator __back1 = _M_finish;
      --__back1;
      iterator __back2 = __back1;
      --__back2;
      __pos = _M_start + __index;
      copy_backward(__pos, __back2, __back1);
    }
    *__pos = value_type();
    return __pos;
  }

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_insert_aux(iterator __pos,
                                        size_type __n,
                                        const value_type &__x)
  {
    const difference_type __elems_before = __pos - _M_start;
    size_type __length = this->size();
    value_type __x_copy = __x;
    if (__elems_before < difference_type(__length / 2))
    {
      iterator __new_start = _M_reserve_elements_at_front(__n);
      iterator __old_start = _M_start;
      __pos = _M_start + __elems_before;
      __STL_TRY
      {
        if (__elems_before >= difference_type(__n))
        {
          iterator __start_n = _M_start + difference_type(__n);
          uninitialized_copy(_M_start, __start_n, __new_start);
          _M_start = __new_start;
          copy(__start_n, __pos, __old_start);
          fill(__pos - difference_type(__n), __pos, __x_copy);
        }
        else
        {
          __uninitialized_copy_fill(_M_start, __pos, __new_start,
                                    _M_start, __x_copy);
          _M_start = __new_start;
          fill(__old_start, __pos, __x_copy);
        }
      }
      __STL_UNWIND(_M_destroy_nodes(__new_start._M_node, _M_start._M_node));
    }
    else
    {
      iterator __new_finish = _M_reserve_elements_at_back(__n);
      iterator __old_finish = _M_finish;
      const difference_type __elems_after =
          difference_type(__length) - __elems_before;
      __pos = _M_finish - __elems_after;
      __STL_TRY
      {
        if (__elems_after > difference_type(__n))
        {
          iterator __finish_n = _M_finish - difference_type(__n);
          uninitialized_copy(__finish_n, _M_finish, _M_finish);
          _M_finish = __new_finish;
          copy_backward(__pos, __finish_n, __old_finish);
          fill(__pos, __pos + difference_type(__n), __x_copy);
        }
        else
        {
          __uninitialized_fill_copy(_M_finish, __pos + difference_type(__n),
                                    __x_copy, __pos, _M_finish);
          _M_finish = __new_finish;
          fill(__pos, __old_finish, __x_copy);
        }
      }
      __STL_UNWIND(_M_destroy_nodes(_M_finish._M_node + 1,
                                    __new_finish._M_node + 1));
    }
  }

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_insert_aux(iterator __pos,
                                        const value_type *__first,
                                        const value_type *__last,
                                        size_type __n)
  {
    const difference_type __elemsbefore = __pos - _M_start;
    size_type __length = size();
    if (__elemsbefore < __length / 2)
    {
      iterator __new_start = _M_reserve_elements_at_front(__n);
      iterator __old_start = _M_start;
      __pos = _M_start + __elemsbefore;
      __STL_TRY
      {
        if (__elemsbefore >= difference_type(__n))
        {
          iterator __start_n = _M_start + difference_type(__n);
          uninitialized_copy(_M_start, __start_n, __new_start);
          _M_start = __new_start;
          copy(__start_n, __pos, __old_start);
          copy(__first, __last, __pos - difference_type(__n));
        }
        else
        {
          const value_type *__mid =
              __first + (difference_type(__n) - __elemsbefore);
          __uninitialized_copy_copy(_M_start, __pos, __first, __mid,
                                    __new_start);
          _M_start = __new_start;
          copy(__mid, __last, __old_start);
        }
      }
      __STL_UNWIND(_M_destroy_nodes(__new_start._M_node, _M_start._M_node));
    }
    else
    {
      iterator __new_finish = _M_reserve_elements_at_back(__n);
      iterator __old_finish = _M_finish;
      const difference_type __elemsafter =
          difference_type(__length) - __elemsbefore;
      __pos = _M_finish - __elemsafter;
      __STL_TRY
      {
        if (__elemsafter > difference_type(__n))
        {
          iterator __finish_n = _M_finish - difference_type(__n);
          uninitialized_copy(__finish_n, _M_finish, _M_finish);
          _M_finish = __new_finish;
          copy_backward(__pos, __finish_n, __old_finish);
          copy(__first, __last, __pos);
        }
        else
        {
          const value_type *__mid = __first + __elemsafter;
          __uninitialized_copy_copy(__mid, __last, __pos, _M_finish, _M_finish);
          _M_finish = __new_finish;
          copy(__first, __mid, __pos);
        }
      }
      __STL_UNWIND(_M_destroy_nodes(_M_finish._M_node + 1,
                                    __new_finish._M_node + 1));
    }
  }

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_insert_aux(iterator __pos,
                                        const_iterator __first,
                                        const_iterator __last,
                                        size_type __n)
  {
    const difference_type __elemsbefore = __pos - _M_start;
    size_type __length = size();
    if (__elemsbefore < __length / 2)
    {
      iterator __new_start = _M_reserve_elements_at_front(__n);
      iterator __old_start = _M_start;
      __pos = _M_start + __elemsbefore;
      __STL_TRY
      {
        if (__elemsbefore >= __n)
        {
          iterator __start_n = _M_start + __n;
          uninitialized_copy(_M_start, __start_n, __new_start);
          _M_start = __new_start;
          copy(__start_n, __pos, __old_start);
          copy(__first, __last, __pos - difference_type(__n));
        }
        else
        {
          const_iterator __mid = __first + (__n - __elemsbefore);
          __uninitialized_copy_copy(_M_start, __pos, __first, __mid,
                                    __new_start);
          _M_start = __new_start;
          copy(__mid, __last, __old_start);
        }
      }
      __STL_UNWIND(_M_destroy_nodes(__new_start._M_node, _M_start._M_node));
    }
    else
    {
      iterator __new_finish = _M_reserve_elements_at_back(__n);
      iterator __old_finish = _M_finish;
      const difference_type __elemsafter = __length - __elemsbefore;
      __pos = _M_finish - __elemsafter;
      __STL_TRY
      {
        if (__elemsafter > __n)
        {
          iterator __finish_n = _M_finish - difference_type(__n);
          uninitialized_copy(__finish_n, _M_finish, _M_finish);
          _M_finish = __new_finish;
          copy_backward(__pos, __finish_n, __old_finish);
          copy(__first, __last, __pos);
        }
        else
        {
          const_iterator __mid = __first + __elemsafter;
          __uninitialized_copy_copy(__mid, __last, __pos, _M_finish, _M_finish);
          _M_finish = __new_finish;
          copy(__first, __mid, __pos);
        }
      }
      __STL_UNWIND(_M_destroy_nodes(_M_finish._M_node + 1,
                                    __new_finish._M_node + 1));
    }
  }

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_new_elements_at_front(size_type __new_elems)
  {
    size_type __new_nodes = (__new_elems + _S_buffer_size() - 1) / _S_buffer_size();
    _M_reserve_map_at_front(__new_nodes);
    size_type __i;
    __STL_TRY
    {
      for (__i = 1; __i <= __new_nodes; ++__i)
        *(_M_start._M_node - __i) = _M_allocate_node();
    }
  #ifdef __STL_USE_EXCEPTIONS
    catch (...)
    {
      for (size_type __j = 1; __j < __i; ++__j)
        _M_deallocate_node(*(_M_start._M_node - __j));
      throw;
    }
  #endif /* __STL_USE_EXCEPTIONS */
  }

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_new_elements_at_back(size_type __new_elems)
  {
    size_type __new_nodes = (__new_elems + _S_buffer_size() - 1) / _S_buffer_size();
    _M_reserve_map_at_back(__new_nodes);
    size_type __i;
    __STL_TRY
    {
      for (__i = 1; __i <= __new_nodes; ++__i)
        *(_M_finish._M_node + __i) = _M_allocate_node();
    }
  #ifdef __STL_USE_EXCEPTIONS
    catch (...)
    {
      for (size_type __j = 1; __j < __i; ++__j)
        _M_deallocate_node(*(_M_finish._M_node + __j));
      throw;
    }
  #endif /* __STL_USE_EXCEPTIONS */
  }

  template <class _Tp, class _Alloc>
  void deque<_Tp, _Alloc>::_M_reallocate_map(size_type __nodes_to_add,
                                            bool __add_at_front)
  {
    size_type __old_num_nodes = _M_finish._M_node - _M_start._M_node + 1;
    size_type __new_num_nodes = __old_num_nodes + __nodes_to_add;

    _Map_pointer __new_nstart;
    if (_M_map_size > 2 * __new_num_nodes)
    {
      __new_nstart = _M_map + (_M_map_size - __new_num_nodes) / 2 + (__add_at_front ? __nodes_to_add : 0);
      if (__new_nstart < _M_start._M_node)
        copy(_M_start._M_node, _M_finish._M_node + 1, __new_nstart);
      else
        copy_backward(_M_start._M_node, _M_finish._M_node + 1,
                      __new_nstart + __old_num_nodes);
    }
    else
    {
      size_type __new_map_size =
          _M_map_size + max(_M_map_size, __nodes_to_add) + 2;

      _Map_pointer __new_map = _M_allocate_map(__new_map_size);
      __new_nstart = __new_map + (__new_map_size - __new_num_nodes) / 2 + (__add_at_front ? __nodes_to_add : 0);
      copy(_M_start._M_node, _M_finish._M_node + 1, __new_nstart);
      _M_deallocate_map(_M_map, _M_map_size);

      _M_map = __new_map;
      _M_map_size = __new_map_size;
    }

    _M_start._M_set_node(__new_nstart);
    _M_finish._M_set_node(__new_nstart + __old_num_nodes - 1);
  }

  // Nonmember functions.

  template <class _Tp, class _Alloc>
  inline bool operator==(const deque<_Tp, _Alloc> &__x,
                        const deque<_Tp, _Alloc> &__y)
  {
    return __x.size() == __y.size() &&
          equal(__x.begin(), __x.end(), __y.begin());
  }

  template <class _Tp, class _Alloc>
  inline bool operator<(const deque<_Tp, _Alloc> &__x,
                        const deque<_Tp, _Alloc> &__y)
  {
    return lexicographical_compare(__x.begin(), __x.end(),
                                  __y.begin(), __y.end());
  }

  ```
  </details>

#### 容器适配器

##### 容器适配器```stack```

  栈```stack```只在尾部对元素做增删操作，满足的底层容器有```vector```、```deque```和```list```。

  [stl_stack.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_stack.h)

  <details>
  <summary>stack</summary>

  ```C++
  //容器适配器 栈
  template <class _Tp,
            class _Sequence = deque<_Tp> > //底层容器_Sequence
  class stack;

  template <class _Tp, class _Seq>
  bool operator==(const stack<_Tp, _Seq> &__x, const stack<_Tp, _Seq> &__y);

  template <class _Tp, class _Seq>
  bool operator<(const stack<_Tp, _Seq> &__x, const stack<_Tp, _Seq> &__y);

  template <class _Tp, class _Sequence>
  class stack
  {

    // requirements:

    __STL_CLASS_REQUIRES(_Tp, _Assignable);
    __STL_CLASS_REQUIRES(_Sequence, _BackInsertionSequence);
    typedef typename _Sequence::value_type _Sequence_value_type;
    __STL_CLASS_REQUIRES_SAME_TYPE(_Tp, _Sequence_value_type);

    friend bool __STD_QUALIFIER
    operator== __STL_NULL_TMPL_ARGS(const stack &, const stack &);
    friend bool __STD_QUALIFIER
    operator<__STL_NULL_TMPL_ARGS(const stack &, const stack &);

  public:
    typedef typename _Sequence::value_type value_type;
    typedef typename _Sequence::size_type size_type;
    typedef _Sequence container_type;

    typedef typename _Sequence::reference reference;
    typedef typename _Sequence::const_reference const_reference;

  protected:
    _Sequence c;

  public:
    stack() : c() {}
    explicit stack(const _Sequence &__s) : c(__s) {}

    //栈的操作都是操作底层容器，容器符合能在尾部增删即可
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type &__x) { c.push_back(__x); }
    void pop() { c.pop_back(); }
  };

  template <class _Tp, class _Seq>
  bool operator==(const stack<_Tp, _Seq> &__x, const stack<_Tp, _Seq> &__y)
  {
    return __x.c == __y.c;
  }

  template <class _Tp, class _Seq>
  bool operator<(const stack<_Tp, _Seq> &__x, const stack<_Tp, _Seq> &__y)
  {
    return __x.c < __y.c;
  }
  ```
  </details>

##### 容器适配器```queue```

  队列```queue```只在尾部对元素做增操作和头部做删除，满足的底层容器有```deque```和```list```。(```vector```没有```pop_front```，即使有也会相当低效！)

  [stl_queue.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_queue.h)

  <details>
  <summary>queue</summary>

  ```C++
  //容器适配器 队列
  template <class _Tp,
            class _Sequence = deque<_Tp> > //依赖底层容器实现
  class queue;

  template <class _Tp, class _Seq>
  inline bool operator==(const queue<_Tp, _Seq> &, const queue<_Tp, _Seq> &);

  template <class _Tp, class _Seq>
  inline bool operator<(const queue<_Tp, _Seq> &, const queue<_Tp, _Seq> &);

  template <class _Tp, class _Sequence>
  class queue
  {

    // requirements:

    __STL_CLASS_REQUIRES(_Tp, _Assignable);
    __STL_CLASS_REQUIRES(_Sequence, _FrontInsertionSequence);
    __STL_CLASS_REQUIRES(_Sequence, _BackInsertionSequence);
    typedef typename _Sequence::value_type _Sequence_value_type;
    __STL_CLASS_REQUIRES_SAME_TYPE(_Tp, _Sequence_value_type);

    friend bool __STD_QUALIFIER
    operator== __STL_NULL_TMPL_ARGS(const queue &, const queue &);
    friend bool __STD_QUALIFIER
    operator<__STL_NULL_TMPL_ARGS(const queue &, const queue &);

  public:
    typedef typename _Sequence::value_type value_type;
    typedef typename _Sequence::size_type size_type;
    typedef _Sequence container_type;

    typedef typename _Sequence::reference reference;
    typedef typename _Sequence::const_reference const_reference;

  protected:
    _Sequence c;

  public:
    queue() : c() {}
    explicit queue(const _Sequence &__c) : c(__c) {}

    //依赖底层容器，容器符合能后进前出即可
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }
    void push(const value_type &__x) { c.push_back(__x); }
    void pop() { c.pop_front(); }
  };

  template <class _Tp, class _Sequence>
  bool operator==(const queue<_Tp, _Sequence> &__x, const queue<_Tp, _Sequence> &__y)
  {
    return __x.c == __y.c;
  }

  template <class _Tp, class _Sequence>
  bool operator<(const queue<_Tp, _Sequence> &__x, const queue<_Tp, _Sequence> &__y)
  {
    return __x.c < __y.c;
  }
  ```
  </details>

##### 容器适配器```priority_queue```

  优先队列```priority_queue```底层依赖```堆```实现。

  [stl_heap.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_heap.h)

  <details>
  <summary>heap</summary>

  ```C++
  //堆 数组模拟完全二叉树实现
  //如果一个结点的位置为k,则它的父节点的位置为k/2,左子节点位置2k,右子结点2k+1

  template <class _RandomAccessIterator, class _Distance, class _Tp>
  void __push_heap(_RandomAccessIterator __first, //起点
                  _Distance __holeIndex,         //终点，插入点
                  _Distance __topIndex,          //根节点 0
                  _Tp __value)
  {
    //计算插入点的父节点
    _Distance __parent = (__holeIndex - 1) / 2;
    //“🕳️”节点在根节点后 并且 父节点值小于插入值 代表需要交换节点
    while (__holeIndex > __topIndex && *(__first + __parent) < __value)
    {
      //“🕳️”值放入父节点的值
      *(__first + __holeIndex) = *(__first + __parent);
      //“🕳️”位移动到父节点
      __holeIndex = __parent;
      //父节点更新
      __parent = (__holeIndex - 1) / 2;
    }
    //”🕳️“值插入
    *(__first + __holeIndex) = __value;
  }

  template <class _RandomAccessIterator, class _Distance, class _Tp>
  inline void
  __push_heap_aux(_RandomAccessIterator __first,
                  _RandomAccessIterator __last, _Distance *, _Tp *)
  {
    //在堆[first, last-1)中插入*(last-1)
    __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0),
                _Tp(*(__last - 1)));
  }

  //在堆[first, last-1)中插入*(last-1)
  template <class _RandomAccessIterator>
  inline void
  push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
  {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type,
                  _LessThanComparable);
    __push_heap_aux(__first, __last,
                    __DISTANCE_TYPE(__first), __VALUE_TYPE(__first));
  }

  //在堆[first, last-1)中插入*(last-1) 自定义比较方法版
  template <class _RandomAccessIterator, class _Distance, class _Tp,
            class _Compare>
  void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex,
                  _Distance __topIndex, _Tp __value, _Compare __comp)
  {
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __value))
    {
      *(__first + __holeIndex) = *(__first + __parent);
      __holeIndex = __parent;
      __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __value;
  }

  template <class _RandomAccessIterator, class _Compare,
            class _Distance, class _Tp>
  inline void
  __push_heap_aux(_RandomAccessIterator __first,
                  _RandomAccessIterator __last, _Compare __comp,
                  _Distance *, _Tp *)
  {
    __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0),
                _Tp(*(__last - 1)), __comp);
  }

  template <class _RandomAccessIterator, class _Compare>
  inline void
  push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
            _Compare __comp)
  {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __push_heap_aux(__first, __last, __comp,
                    __DISTANCE_TYPE(__first), __VALUE_TYPE(__first));
  }

  //下沉根节点
  template <class _RandomAccessIterator, class _Distance, class _Tp>
  void __adjust_heap(_RandomAccessIterator __first, // 0
                    _Distance __holeIndex,         // 0
                    _Distance __len, _Tp __value)
  {
    //根节点
    _Distance __topIndex = __holeIndex;
    //“🕳️”点的右子节点
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) //右子节点有效
    {
      //取左右子节点的大值
      if (*(__first + __secondChild) < *(__first + (__secondChild - 1)))
        __secondChild--;
      //“🕳️”点取子节点的大值
      *(__first + __holeIndex) = *(__first + __secondChild);
      //“🕳️”点下沉
      __holeIndex = __secondChild;
      //更新子节点
      __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) // fix 节点数为len-1
    {
      //“🕳️”点交换左子节点
      *(__first + __holeIndex) = *(__first + (__secondChild - 1));
      __holeIndex = __secondChild - 1;
    }
    //插入value，调整堆
    __push_heap(__first, __holeIndex, __topIndex, __value);
  }

  template <class _RandomAccessIterator, class _Tp, class _Distance>
  inline void
  __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
            _RandomAccessIterator __result, _Tp __value, _Distance *)
  {
    //取根
    *__result = *__first;
    //下沉根节点
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __value);
  }

  template <class _RandomAccessIterator, class _Tp>
  inline void
  __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last,
                _Tp *)
  {
    // pop根，实际移动到last-1并且重新调整堆
    __pop_heap(__first, __last - 1, __last - 1,
              _Tp(*(__last - 1)), __DISTANCE_TYPE(__first));
  }

  // pop根
  template <class _RandomAccessIterator>
  inline void pop_heap(_RandomAccessIterator __first,
                      _RandomAccessIterator __last)
  {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type,
                  _LessThanComparable);
    __pop_heap_aux(__first, __last, __VALUE_TYPE(__first));
  }

  template <class _RandomAccessIterator, class _Distance,
            class _Tp, class _Compare>
  void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
                    _Distance __len, _Tp __value, _Compare __comp)
  {
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len)
    {
      if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
        __secondChild--;
      *(__first + __holeIndex) = *(__first + __secondChild);
      __holeIndex = __secondChild;
      __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len)
    {
      *(__first + __holeIndex) = *(__first + (__secondChild - 1));
      __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __value, __comp);
  }

  template <class _RandomAccessIterator, class _Tp, class _Compare,
            class _Distance>
  inline void
  __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
            _RandomAccessIterator __result, _Tp __value, _Compare __comp,
            _Distance *)
  {
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first),
                  __value, __comp);
  }

  template <class _RandomAccessIterator, class _Tp, class _Compare>
  inline void
  __pop_heap_aux(_RandomAccessIterator __first,
                _RandomAccessIterator __last, _Tp *, _Compare __comp)
  {
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp,
              __DISTANCE_TYPE(__first));
  }

  //下沉根节点
  template <class _RandomAccessIterator, class _Compare>
  inline void
  pop_heap(_RandomAccessIterator __first,
          _RandomAccessIterator __last, _Compare __comp)
  {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __pop_heap_aux(__first, __last, __VALUE_TYPE(__first), __comp);
  }

  template <class _RandomAccessIterator, class _Tp, class _Distance>
  void __make_heap(_RandomAccessIterator __first,
                  _RandomAccessIterator __last, _Tp *, _Distance *)
  {
    if (__last - __first < 2)
      return;
    //堆长度
    _Distance __len = __last - __first;
    //父节点
    _Distance __parent = (__len - 2) / 2;

    while (true)
    {
      __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)));
      if (__parent == 0)
        return;
      __parent--;
    }
  }

  //生成堆
  template <class _RandomAccessIterator>
  inline void
  make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
  {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type,
                  _LessThanComparable);
    __make_heap(__first, __last,
                __VALUE_TYPE(__first), __DISTANCE_TYPE(__first));
  }

  template <class _RandomAccessIterator, class _Compare,
            class _Tp, class _Distance>
  void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                  _Compare __comp, _Tp *, _Distance *)
  {
    if (__last - __first < 2)
      return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    while (true)
    {
      __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)),
                    __comp);
      if (__parent == 0)
        return;
      __parent--;
    }
  }

  template <class _RandomAccessIterator, class _Compare>
  inline void
  make_heap(_RandomAccessIterator __first,
            _RandomAccessIterator __last, _Compare __comp)
  {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __make_heap(__first, __last, __comp,
                __VALUE_TYPE(__first), __DISTANCE_TYPE(__first));
  }

  //堆排序，循环pop取出最大值并放置到后面实现
  template <class _RandomAccessIterator>
  void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
  {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    __STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type,
                  _LessThanComparable);
    while (__last - __first > 1)
      pop_heap(__first, __last--);
  }

  template <class _RandomAccessIterator, class _Compare>
  void sort_heap(_RandomAccessIterator __first,
                _RandomAccessIterator __last, _Compare __comp)
  {
    __STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
    while (__last - __first > 1)
      pop_heap(__first, __last--, __comp);
  }
  ```
  </details>

  [stl_queue.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_queue.h)

  <details>
  <summary>priority_queue</summary>

  ```C++
  //优先队列
  template <class _Tp,
            class _Sequence __STL_DEPENDENT_DEFAULT_TMPL(vector<_Tp>), //默认使用vector作为底层容器
            class _Compare
                __STL_DEPENDENT_DEFAULT_TMPL(less<typename _Sequence::value_type>)> //比较方法
  class priority_queue
  {
  public:
    // requirements:
    __STL_CLASS_REQUIRES(_Tp, _Assignable);
    __STL_CLASS_REQUIRES(_Sequence, _Sequence);
    __STL_CLASS_REQUIRES(_Sequence, _RandomAccessContainer);
    typedef typename _Sequence::value_type _Sequence_value_type;
    __STL_CLASS_REQUIRES_SAME_TYPE(_Tp, _Sequence_value_type);
    __STL_CLASS_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);

    typedef typename _Sequence::value_type value_type;
    typedef typename _Sequence::size_type size_type;
    typedef _Sequence container_type;

    typedef typename _Sequence::reference reference;
    typedef typename _Sequence::const_reference const_reference;

  protected:
    _Sequence c;   //底层容器
    _Compare comp; //比较方法

  public:
    priority_queue() : c() {}
    explicit priority_queue(const _Compare &__x) : c(), comp(__x) {}
    priority_queue(const _Compare &__x, const _Sequence &__s)
        : c(__s), comp(__x)
    {
      //生成堆
      make_heap(c.begin(), c.end(), comp);
    }

    priority_queue(const value_type *__first, const value_type *__last)
        : c(__first, __last)
    {
      make_heap(c.begin(), c.end(), comp);
    }

    priority_queue(const value_type *__first, const value_type *__last,
                  const _Compare &__x)
        : c(__first, __last), comp(__x)
    {
      make_heap(c.begin(), c.end(), comp);
    }

    priority_queue(const value_type *__first, const value_type *__last,
                  const _Compare &__x, const _Sequence &__c)
        : c(__c), comp(__x)
    {
      c.insert(c.end(), __first, __last);
      make_heap(c.begin(), c.end(), comp);
    }

    bool empty() const
    {
      return c.empty();
    }
    size_type size() const { return c.size(); }
    //获取顶部最“大”值
    const_reference top() const { return c.front(); }
    //入堆
    void push(const value_type &__x)
    {
      __STL_TRY
      {
        //先添加到底层容器中
        c.push_back(__x);
        //调整堆，内部执行插入最后值
        push_heap(c.begin(), c.end(), comp);
      }
      __STL_UNWIND(c.clear());
    }
    void pop()
    {
      __STL_TRY
      {
        //将最“大”值(根，第一个元素)出堆，内部是移动到底层容器最后，之后重新调整堆
        pop_heap(c.begin(), c.end(), comp);
        //弹出最后的最“大”值
        c.pop_back();
      }
      __STL_UNWIND(c.clear());
    }
  };
  ```
  </details>

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
