# STL标准模板库

- [STL标准模板库](#stl标准模板库)
  - [容器```container```](#容器container)
    - [序列式容器```sequence container```](#序列式容器sequence-container)
      - [容器适配器](#容器适配器)
        - [容器适配器```stack```](#容器适配器stack)
        - [容器适配器```queue```](#容器适配器queue)
        - [容器适配器```priority_queue```](#容器适配器priority_queue)
    - [关联式容器 ``associative container``](#关联式容器-associative-container)
      - [容器 ``set`` ``multiset``](#容器-set-multiset)
      - [容器 ``map`` ``multimap``](#容器-map-multimap)
      - [容器 ``unordered_set`` ``unordered_multiset`` ``unordered_map`` ``unordered_multimap``](#容器-unordered_set-unordered_multiset-unordered_map-unordered_multimap)

![STL六大模块](https://github.com/gongluck/images/blob/main/stl/STL六大模块.png)

## 容器```container```

### 序列式容器```sequence container```

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
