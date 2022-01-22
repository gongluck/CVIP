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
  template <class T>
  inline T *allocate(ptrdiff_t size, T *)
  {
      set_new_handler(0);
      T *tmp = (T *)(::operator new((size_t)(size * sizeof(T))));
      if (tmp == 0)
      {
          cerr << "out of memory" << endl;
          exit(1);
      }
      return tmp;
  }

  //包装::operator delete
  template <class T>
  inline void deallocate(T *buffer)
  {
      ::operator delete(buffer);
  }

  //分配器
  template <class T>
  class allocator
  {
  public:
      //分配n个T的内存
      pointer allocate(size_type n)
      {
          return ::allocate((difference_type)n, (pointer)0);
      }
      void deallocate(pointer p) { ::deallocate(p); }
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
  //销毁实例 调用析构
  template <class T>
  inline void destroy(T *pointer)
  {
    pointer->~T();
  }

  //创建实例 调用构造
  template <class T1, class T2>
  inline void construct(T1 *p, const T2 &value)
  {
    // placement new
    new (p) T1(value);
  }

  template <class ForwardIterator>
  inline void
  __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
  {
    //遍历区间的所有实例 调用析构
    for (; first < last; ++first)
      destroy(&*first);
  }

  template <class ForwardIterator>
  inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}

  template <class ForwardIterator, class T>
  inline void __destroy(ForwardIterator first, ForwardIterator last, T *)
  {
    //萃取trivial_destructor特性
    // trivial_destructor不重要的析构函数
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
  }

  //范围destroy
  template <class ForwardIterator>
  inline void destroy(ForwardIterator first, ForwardIterator last)
  {
    __destroy(first, last, value_type(first));
  }
  //范围destroy的特化版本
  inline void destroy(char *, char *) {}
  inline void destroy(wchar_t *, wchar_t *) {}
  ```
  </details>

### 内存分配和释放

  ![内存池分配器](https://github.com/gongluck/images/blob/main/stl/内存池分配器.png)

  [stl_alloc.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_alloc.h)

  ```SGI```设计了双层策略。
  第一级配置器```__malloc_alloc_template```简单包装了```malloc```和```free```，并在内存分配失败时调用失败处理函数。

  <details>
  <summary>第一级配置器</summary>

  ```C++
  //第一级分配器 包装malloc和free
  template <int inst>
  class __malloc_alloc_template
  {
  public:
    static void *allocate(size_t n)
    {
      void *result = malloc(n);
      if (0 == result)
        result = oom_malloc(n);
      return result;
    }

    static void deallocate(void *p, size_t /* n */)
    {
      free(p);
    }
  };
  ```
  </details>

  第二级配置器```__default_alloc_template```使用了```内存池```策略，使用```free list```实现。第二级配置器的内存池实现是```双缓冲```，内存池不足时先从侯备缓存中分配，当侯备缓存也不能分配才从系统中申请。

  <details>
  <summary>第二级配置器</summary>

  ```C++
  //第二级分配器 使用内存池优化 现代malloc库和操作系统的内存api已经存在类似的内存池优化了，所以使用STL标准分配器即可。
  template <bool threads, int inst>
  class __default_alloc_template
  {

  private:
    //对输入边界对齐到8的倍数
    static size_t ROUND_UP(size_t bytes)
    {
      return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1));
    }
    //使用联合体合理使用内存
    __PRIVATE : union obj
    {
      union obj *free_list_link;
      char client_data[1]; /* The client sees this.        */
    };

  private:
  #ifdef __SUNPRO_CC
    static obj *__VOLATILE free_list[];
    // Specifying a size results in duplicate def for 4.1
  #else
    static obj *__VOLATILE free_list[__NFREELISTS];
  #endif
    //查找bytes大小的内存块落在哪段自由链表中
    static size_t FREELIST_INDEX(size_t bytes)
    {
      return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
    }

  public:
    /* n must be > 0      */
    static void *allocate(size_t n)
    {
      obj *__VOLATILE *my_free_list;
      obj *__RESTRICT result;

      //大内存使用malloc分配
      if (n > (size_t)__MAX_BYTES)
      {
        return (malloc_alloc::allocate(n));
      }
      //查找内存块落在哪段自由链表中
      my_free_list = free_list + FREELIST_INDEX(n);

      //获取自由链表的头节点
      result = *my_free_list;
      //如果头节点为空
      if (result == 0)
      {
        //增加内存
        void *r = refill(ROUND_UP(n));
        return r;
      }
      //取出头节点 头节点的后继成为新的头节点
      *my_free_list = result->free_list_link;
      return (result);
    };

    /* p may not be 0 */
    static void deallocate(void *p, size_t n)
    {
      obj *q = (obj *)p;
      obj *__VOLATILE *my_free_list;

      //大内存使用free释放
      if (n > (size_t)__MAX_BYTES)
      {
        malloc_alloc::deallocate(p, n);
        return;
      }
      //查找内存块落在哪段自由链表中
      my_free_list = free_list + FREELIST_INDEX(n);

      //头插法插入p
      q->free_list_link = *my_free_list;
      *my_free_list = q;
    }
  };
  ```
  </details>

  <details>
  <summary>封装分配器</summary>

  ```C++
  //对分配器简单封装，以元素字节大小为单位分配内存
  template <class T, class Alloc>
  class simple_alloc
  {
  public:
    static T *allocate(size_t n)
    {
      return 0 == n ? 0 : (T *)Alloc::allocate(n * sizeof(T));
    }
    static T *allocate(void)
    {
      return (T *)Alloc::allocate(sizeof(T));
    }
    static void deallocate(T *p, size_t n)
    {
      if (0 != n)
        Alloc::deallocate(p, n * sizeof(T));
    }
    static void deallocate(T *p)
    {
      Alloc::deallocate(p, sizeof(T));
    }
  };
  ```
  </details>

## 内存基本处理工具

  ```uninitialized_copy```、```uninitialized_fill```在目标内存调用拷贝构造函数或者内存拷贝函数。

  [stl_uninitialized.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_uninitialized.h)

  <details>
  <summary>uninitialized_copy</summary>

  ```C++
  // Valid if copy construction is equivalent to assignment, and if the
  //  destructor is trivial.
  template <class InputIterator, class ForwardIterator>
  inline ForwardIterator
  __uninitialized_copy_aux(InputIterator first, InputIterator last,
                          ForwardIterator result,
                          __true_type)
  {
    //标量类型 数据拷贝
    return copy(first, last, result);
  }

  template <class InputIterator, class ForwardIterator>
  ForwardIterator
  __uninitialized_copy_aux(InputIterator first, InputIterator last,
                          ForwardIterator result,
                          __false_type)
  {
    ForwardIterator cur = result;
    __STL_TRY
    {
      //非标量类型 遍历调用构造
      for (; first != last; ++first, ++cur)
        construct(&*cur, *first);
      return cur;
    }
    __STL_UNWIND(destroy(result, cur));
  }

  template <class InputIterator, class ForwardIterator, class T>
  inline ForwardIterator
  __uninitialized_copy(InputIterator first, InputIterator last,
                      ForwardIterator result, T *)
  {
    typedef typename __type_traits<T>::is_POD_type is_POD; //萃取是否标量类型
    return __uninitialized_copy_aux(first, last, result, is_POD());
  }

  // uninitialized_copy模板
  template <class InputIterator, class ForwardIterator>
  inline ForwardIterator
  uninitialized_copy(InputIterator first, InputIterator last,
                    ForwardIterator result)
  {
    return __uninitialized_copy(first, last, result, value_type(result));
  }

  // uninitialized_copy模板特化
  inline char *uninitialized_copy(const char *first, const char *last,
                                  char *result)
  {
    memmove(result, first, last - first);
    return result + (last - first);
  }
  inline wchar_t *uninitialized_copy(const wchar_t *first, const wchar_t *last,
                                    wchar_t *result)
  {
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
  }
  ```
  </details>

  <details>
  <summary>uninitialized_fill</summary>

  ```C++
  // Valid if copy construction is equivalent to assignment, and if the
  //  destructor is trivial.
  template <class ForwardIterator, class T>
  inline void
  __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                          const T &x, __true_type)
  {
    //标量类型 fill
    fill(first, last, x);
  }

  template <class ForwardIterator, class T>
  void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                const T &x, __false_type)
  {
    ForwardIterator cur = first;
    __STL_TRY
    {
      //非标量类型 遍历构造
      for (; cur != last; ++cur)
        construct(&*cur, x);
    }
    __STL_UNWIND(destroy(first, cur));
  }

  template <class ForwardIterator, class T, class T1>
  inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
                                  const T &x, T1 *)
  {
    typedef typename __type_traits<T1>::is_POD_type is_POD; //萃取是否标量类型
    __uninitialized_fill_aux(first, last, x, is_POD());
  }

  // uninitialized_fill模板
  template <class ForwardIterator, class T>
  inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,
                                const T &x)
  {
    __uninitialized_fill(first, last, x, value_type(first));
  }
  ```
  </details>
