# 分配器```allocator```

- [分配器```allocator```](#分配器allocator)
  - [标准分配器](#标准分配器)
  - [```SGI```特殊分配器](#sgi特殊分配器)
    - [构造和析构工具](#构造和析构工具)
    - [内存分配和释放](#内存分配和释放)
  - [内存基本处理工具](#内存基本处理工具)

## 标准分配器

  包装```::operator new```和```::operator delete```。

  ![标准分配器](https://github.com/gongluck/images/blob/main/stl/标准分配器.png)

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

## ```SGI```特殊分配器

### 构造和析构工具

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

### 内存分配和释放

  ![内存池分配器](https://github.com/gongluck/images/blob/main/stl/内存池分配器.png)

  [stl_alloc.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_alloc.h)

  ```SGI```设计了双层策略。

  ![第一级配置器](https://github.com/gongluck/images/blob/main/stl/第一级配置器.png)

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

  ![第二级配置器](https://github.com/gongluck/images/blob/main/stl/第二级配置器.png)

  第二级配置器```__default_alloc_template```使用了```内存池```策略，使用```free list```实现。第二级配置器的内存池实现是```双缓冲```，内存池不足时先从侯备缓存中分配，当侯备缓存也不能分配才从系统中申请。

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

  ![封装分配器](https://github.com/gongluck/images/blob/main/stl/封装分配器.png)

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

## 内存基本处理工具

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