# 容器container

- [容器container](#容器container)
  - [序列式容器sequence container](#序列式容器sequence-container)
    - [容器base\_string](#容器base_string)
    - [容器vector](#容器vector)
    - [容器list](#容器list)
    - [容器deque](#容器deque)
  - [关联式容器associative container](#关联式容器associative-container)
    - [容器set和multiset](#容器set和multiset)
    - [容器map和multimap](#容器map和multimap)
    - [容器hash\_set、hash\_multiset、hash\_map和hash\_multimap](#容器hash_sethash_multisethash_map和hash_multimap)

## 序列式容器sequence container

### 容器[base_string](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/basic_string.h#L103)

- base_string在数据区前增加了`Rep`结构，Rep包括len(数据区已使用长度)、cap(数据区最大容量)、refcount(引用计数)。

  ```C++
  // _Rep: string representation
  //   Invariants:
  //   1. String really contains _M_length + 1 characters: due to 21.3.4
  //      must be kept null-terminated.
  //   2. _M_capacity >= _M_length
  //      Allocated memory is always (_M_capacity + 1) * sizeof(_CharT).
  //   3. _M_refcount has three states:
  //      -1: leaked, one reference, no ref-copies allowed, non-const.
  //       0: one reference, non-const.
  //     n>0: n + 1 references, operations require a lock, const.
  //   4. All fields==0 is an empty string, given the extra storage
  //      beyond-the-end for a null terminator; thus, the shared
  //      empty string representation needs no constructor.

  struct _Rep_base // 基础描述结构
  {
    size_type _M_length;      // 字符串有效长度
    size_type _M_capacity;    // 内存容量
    _Atomic_word _M_refcount; // 引用计数
  };
  ```

- base_string使用写时拷贝，共享数据并延迟数据的拷贝。

### 容器[vector](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/stl_vector.h#L167)

![容器vector](https://github.com/gongluck/images/blob/main/stl/容器vector.png)

- 根据不同的实现，vector数据成员至少包含3个T*类型的指针。分别为start、finish、end_of_storage。

  ```C++
  // namespace _Vector_impl
  typename _Tp_alloc_type::pointer _M_start;          // 内存开始
  typename _Tp_alloc_type::pointer _M_finish;         // 数据结束
  typename _Tp_alloc_type::pointer _M_end_of_storage; // 内存结束
  ```

- vector迭代器是以元素指针特化的常规迭代器。一些旧版本可能会直接用指针实现。

  ```C++
  // namespace _Vector_base
  typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;
  // namespace vector
  typedef _Vector_base<_Tp, _Alloc> _Base;
  typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
  typedef typename _Tp_alloc_type::pointer pointer;
  typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
  ```

### 容器[list](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/stl_list.h#L399)

![容器list](https://github.com/gongluck/images/blob/main/stl/容器list.png)
![容器list迭代器](https://github.com/gongluck/images/blob/main/stl/容器list迭代器.png)

- 列表节点由前后指针和数据成员组成。

  ```C++
  // namespace _List_node_base
  _List_node_base *_M_next; // next
  _List_node_base *_M_prev; // prev
  // namespace _List_node
  _Tp _M_data; // 用户数据
  ```

- 列表包含一个节点指针指向链表`哨兵`节点。

  ```C++
  // namespace _List_impl
  _List_node_base _M_node; // 头节点
  ```

- 列表迭代器包含一个节点指针指向链表节点。

  ```C++
  // namespace list
  typedef _List_iterator<_Tp> iterator;
  // namespace _List_iterator
  _List_node_base *_M_node; // 列表节点
  ```

### 容器[deque](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/stl_deque.h#L600)

![容器deque](https://github.com/gongluck/images/blob/main/stl/容器deque.png)
![容器deque迭代器](https://github.com/gongluck/images/blob/main/stl/容器deque迭代器.png)

- 双端队列包含指向管控中心的指针、控制中心的大小和分别指向控制中心开始和结束的两个迭代器。

  ```C++
  // namespace _Deque_impl
  _Tp **_M_map;       // 分块管理结构
  size_t _M_map_size; // 总大小
  iterator _M_start;  // 开始
  iterator _M_finish; // 结束
  ```

- 双端队列迭代器包含3个指针指向直接内存的位置和1个指针指向管控中心的节点位置。

  ```C++
  // namespace _Deque_iterator
  typedef _Tp **_Map_pointer;
  _Tp *_M_cur;          // 指向当前元素
  _Tp *_M_first;        // 当前控制块的开始
  _Tp *_M_last;         // 当前控制块的结束
  _Map_pointer _M_node; // 控制块组
  ```

## 关联式容器associative container

### 容器set和multiset

  ```set```和```multiset```底层使用红黑树实现，```key == value```。

  [stl_set.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_set.h)

  <details>
  <summary>set</summary>

  ```C++
  //集合
  template <class Key, class Compare = less<Key>, class Alloc = alloc>
  class set
  {
  public:
    // typedefs:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;   // key比较方法
    typedef Compare value_compare; // value比较方法

  private:
    // set底层使用红黑树实现
    typedef rb_tree<key_type, value_type,
                    identity<value_type>, key_compare, Alloc>
        rep_type;
    rep_type t; // red-black tree representing set
  public:
    // set元素不支持修改，所以以下类型使用const
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::const_reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // allocation/deallocation

    set() : t(Compare()) {}
    explicit set(const Compare &comp) : t(comp) {}

    template <class InputIterator>
    set(InputIterator first, InputIterator last)
        : t(Compare())
    {
      t.insert_unique(first, last);
    }

    template <class InputIterator>
    set(InputIterator first, InputIterator last, const Compare &comp)
        : t(comp) { t.insert_unique(first, last); }

    set(const set<Key, Compare, Alloc> &x) : t(x.t)
    {
    }
    set<Key, Compare, Alloc> &operator=(const set<Key, Compare, Alloc> &x)
    {
      t = x.t;
      return *this;
    }

    // accessors:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return t.key_comp(); }
    iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    reverse_iterator rbegin() const { return t.rbegin(); }
    reverse_iterator rend() const { return t.rend(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    void swap(set<Key, Compare, Alloc> &x) { t.swap(x.t); }

    // insert/erase
    typedef pair<iterator, bool> pair_iterator_bool;
    pair<iterator, bool> insert(const value_type &x)
    {
      pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
      return pair<iterator, bool>(p.first, p.second);
    }
    iterator insert(iterator position, const value_type &x)
    {
      typedef typename rep_type::iterator rep_iterator;
      return t.insert_unique((rep_iterator &)position, x);
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
      t.insert_unique(first, last);
    }

    void erase(iterator position)
    {
      typedef typename rep_type::iterator rep_iterator;
      t.erase((rep_iterator &)position);
    }
    size_type erase(const key_type &x)
    {
      return t.erase(x);
    }
    void erase(iterator first, iterator last)
    {
      typedef typename rep_type::iterator rep_iterator;
      t.erase((rep_iterator &)first, (rep_iterator &)last);
    }
    void clear() { t.clear(); }

    // set operations:
    iterator find(const key_type &x) const { return t.find(x); }
    size_type count(const key_type &x) const { return t.count(x); }
    iterator lower_bound(const key_type &x) const
    {
      return t.lower_bound(x);
    }
    iterator upper_bound(const key_type &x) const
    {
      return t.upper_bound(x);
    }
    pair<iterator, iterator> equal_range(const key_type &x) const
    {
      return t.equal_range(x);
    }
    friend bool operator== __STL_NULL_TMPL_ARGS(const set &, const set &);
    friend bool operator<__STL_NULL_TMPL_ARGS(const set &, const set &);
  };

  template <class Key, class Compare, class Alloc>
  inline bool operator==(const set<Key, Compare, Alloc> &x,
                        const set<Key, Compare, Alloc> &y)
  {
    return x.t == y.t;
  }

  template <class Key, class Compare, class Alloc>
  inline bool operator<(const set<Key, Compare, Alloc> &x,
                        const set<Key, Compare, Alloc> &y)
  {
    return x.t < y.t;
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class Key, class Compare, class Alloc>
  inline void swap(set<Key, Compare, Alloc> &x,
                  set<Key, Compare, Alloc> &y)
  {
    x.swap(y);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */
  ```
  </details>

  [stl_multiset.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_multiset.h)

  <details>
  <summary>multiset</summary>

  ```C++
  //可重复集合
  template <class Key, class Compare = less<Key>, class Alloc = alloc>
  class multiset
  {
  public:
    // typedefs:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;

  private:
    typedef rb_tree<key_type, value_type,
                    identity<value_type>, key_compare, Alloc>
        rep_type;
    rep_type t; // red-black tree representing multiset
  public:
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::const_reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // allocation/deallocation

    multiset() : t(Compare()) {}
    explicit multiset(const Compare &comp) : t(comp) {}

    template <class InputIterator>
    multiset(InputIterator first, InputIterator last)
        : t(Compare())
    {
      t.insert_equal(first, last);
    }
    template <class InputIterator>
    multiset(InputIterator first, InputIterator last, const Compare &comp)
        : t(comp) { t.insert_equal(first, last); }

    multiset(const multiset<Key, Compare, Alloc> &x) : t(x.t)
    {
    }
    multiset<Key, Compare, Alloc> &
    operator=(const multiset<Key, Compare, Alloc> &x)
    {
      t = x.t;
      return *this;
    }

    // accessors:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return t.key_comp(); }
    iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    reverse_iterator rbegin() const { return t.rbegin(); }
    reverse_iterator rend() const { return t.rend(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    void swap(multiset<Key, Compare, Alloc> &x) { t.swap(x.t); }

    // insert/erase
    iterator insert(const value_type &x)
    {
      return t.insert_equal(x);
    }
    iterator insert(iterator position, const value_type &x)
    {
      typedef typename rep_type::iterator rep_iterator;
      return t.insert_equal((rep_iterator &)position, x);
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
      t.insert_equal(first, last);
    }

    void erase(iterator position)
    {
      typedef typename rep_type::iterator rep_iterator;
      t.erase((rep_iterator &)position);
    }
    size_type erase(const key_type &x)
    {
      return t.erase(x);
    }
    void erase(iterator first, iterator last)
    {
      typedef typename rep_type::iterator rep_iterator;
      t.erase((rep_iterator &)first, (rep_iterator &)last);
    }
    void clear() { t.clear(); }

    // multiset operations:
    iterator find(const key_type &x) const { return t.find(x); }
    size_type count(const key_type &x) const { return t.count(x); }
    iterator lower_bound(const key_type &x) const
    {
      return t.lower_bound(x);
    }
    iterator upper_bound(const key_type &x) const
    {
      return t.upper_bound(x);
    }
    pair<iterator, iterator> equal_range(const key_type &x) const
    {
      return t.equal_range(x);
    }
    friend bool operator== __STL_NULL_TMPL_ARGS(const multiset &,
                                                const multiset &);
    friend bool operator<__STL_NULL_TMPL_ARGS(const multiset &,
                                              const multiset &);
  };

  template <class Key, class Compare, class Alloc>
  inline bool operator==(const multiset<Key, Compare, Alloc> &x,
                        const multiset<Key, Compare, Alloc> &y)
  {
    return x.t == y.t;
  }

  template <class Key, class Compare, class Alloc>
  inline bool operator<(const multiset<Key, Compare, Alloc> &x,
                        const multiset<Key, Compare, Alloc> &y)
  {
    return x.t < y.t;
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class Key, class Compare, class Alloc>
  inline void swap(multiset<Key, Compare, Alloc> &x,
                  multiset<Key, Compare, Alloc> &y)
  {
    x.swap(y);
  }
  ```
  </details>

### 容器map和multimap

  ```map```和```multimap```底层使用红黑树实现，```key != value```。

  [stl_map.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_map.h)

  <details>
  <summary>map</summary>

  ```C++
  //映射
  template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
  class map
  {
  public:
    // typedefs:
    typedef Key key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type; // map的value_type是key+data
    typedef Compare key_compare;           // key比较方法

    class value_compare
        : public binary_function<value_type, value_type, bool>
    {
      friend class map<Key, T, Compare, Alloc>;

    protected:
      Compare comp;
      value_compare(Compare c) : comp(c) {}

    public:
      bool operator()(const value_type &x, const value_type &y) const
      {
        return comp(x.first, y.first);
      }
    };

  private:
    // map底层使用红黑树实现
    typedef rb_tree<key_type, value_type,
                    select1st<value_type>, key_compare, Alloc>
        rep_type;
    rep_type t; // red-black tree representing map
  public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // allocation/deallocation

    map() : t(Compare()) {}
    explicit map(const Compare &comp) : t(comp) {}

    template <class InputIterator>
    map(InputIterator first, InputIterator last)
        : t(Compare())
    {
      t.insert_unique(first, last);
    }

    template <class InputIterator>
    map(InputIterator first, InputIterator last, const Compare &comp)
        : t(comp) { t.insert_unique(first, last); }

    map(const map<Key, T, Compare, Alloc> &x) : t(x.t)
    {
    }
    map<Key, T, Compare, Alloc> &operator=(const map<Key, T, Compare, Alloc> &x)
    {
      t = x.t;
      return *this;
    }

    // accessors:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }
    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const { return t.end(); }
    reverse_iterator rbegin() { return t.rbegin(); }
    const_reverse_iterator rbegin() const { return t.rbegin(); }
    reverse_iterator rend() { return t.rend(); }
    const_reverse_iterator rend() const { return t.rend(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    T &operator[](const key_type &k)
    {
      return (*((insert(value_type(k, T()))).first)).second;
    }
    void swap(map<Key, T, Compare, Alloc> &x) { t.swap(x.t); }

    // insert/erase
    pair<iterator, bool> insert(const value_type &x) { return t.insert_unique(x); }
    iterator insert(iterator position, const value_type &x)
    {
      return t.insert_unique(position, x);
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
      t.insert_unique(first, last);
    }

    void erase(iterator position)
    {
      t.erase(position);
    }
    size_type erase(const key_type &x) { return t.erase(x); }
    void erase(iterator first, iterator last) { t.erase(first, last); }
    void clear() { t.clear(); }

    // map operations:
    iterator find(const key_type &x) { return t.find(x); }
    const_iterator find(const key_type &x) const { return t.find(x); }
    size_type count(const key_type &x) const { return t.count(x); }
    iterator lower_bound(const key_type &x) { return t.lower_bound(x); }
    const_iterator lower_bound(const key_type &x) const
    {
      return t.lower_bound(x);
    }
    iterator upper_bound(const key_type &x) { return t.upper_bound(x); }
    const_iterator upper_bound(const key_type &x) const
    {
      return t.upper_bound(x);
    }

    pair<iterator, iterator> equal_range(const key_type &x)
    {
      return t.equal_range(x);
    }
    pair<const_iterator, const_iterator> equal_range(const key_type &x) const
    {
      return t.equal_range(x);
    }
    friend bool operator== __STL_NULL_TMPL_ARGS(const map &, const map &);
    friend bool operator<__STL_NULL_TMPL_ARGS(const map &, const map &);
  };

  template <class Key, class T, class Compare, class Alloc>
  inline bool operator==(const map<Key, T, Compare, Alloc> &x,
                        const map<Key, T, Compare, Alloc> &y)
  {
    return x.t == y.t;
  }

  template <class Key, class T, class Compare, class Alloc>
  inline bool operator<(const map<Key, T, Compare, Alloc> &x,
                        const map<Key, T, Compare, Alloc> &y)
  {
    return x.t < y.t;
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class Key, class T, class Compare, class Alloc>
  inline void swap(map<Key, T, Compare, Alloc> &x,
                  map<Key, T, Compare, Alloc> &y)
  {
    x.swap(y);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */
  ```
  </details>

  [stl_multimap.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_multimap.h)

  <details>
  <summary>multimap</summary>

  ```C++
  //可重复映射
  template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
  class multimap
  {
  public:
    // typedefs:
    typedef Key key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type;
    typedef Compare key_compare;

    class value_compare : public binary_function<value_type, value_type, bool>
    {
      friend class multimap<Key, T, Compare, Alloc>;

    protected:
      Compare comp;
      value_compare(Compare c) : comp(c) {}

    public:
      bool operator()(const value_type &x, const value_type &y) const
      {
        return comp(x.first, y.first);
      }
    };

  private:
    typedef rb_tree<key_type, value_type,
                    select1st<value_type>, key_compare, Alloc>
        rep_type;
    rep_type t; // red-black tree representing multimap
  public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // allocation/deallocation
    multimap() : t(Compare()) {}
    explicit multimap(const Compare &comp) : t(comp) {}

    template <class InputIterator>
    multimap(InputIterator first, InputIterator last)
        : t(Compare())
    {
      t.insert_equal(first, last);
    }

    template <class InputIterator>
    multimap(InputIterator first, InputIterator last, const Compare &comp)
        : t(comp) { t.insert_equal(first, last); }

    multimap(const multimap<Key, T, Compare, Alloc> &x) : t(x.t)
    {
    }
    multimap<Key, T, Compare, Alloc> &
    operator=(const multimap<Key, T, Compare, Alloc> &x)
    {
      t = x.t;
      return *this;
    }

    // accessors:

    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }
    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const { return t.end(); }
    reverse_iterator rbegin() { return t.rbegin(); }
    const_reverse_iterator rbegin() const { return t.rbegin(); }
    reverse_iterator rend() { return t.rend(); }
    const_reverse_iterator rend() const { return t.rend(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    void swap(multimap<Key, T, Compare, Alloc> &x) { t.swap(x.t); }

    // insert/erase
    iterator insert(const value_type &x) { return t.insert_equal(x); }
    iterator insert(iterator position, const value_type &x)
    {
      return t.insert_equal(position, x);
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
      t.insert_equal(first, last);
    }

    void erase(iterator position)
    {
      t.erase(position);
    }
    size_type erase(const key_type &x) { return t.erase(x); }
    void erase(iterator first, iterator last) { t.erase(first, last); }
    void clear() { t.clear(); }

    // multimap operations:
    iterator find(const key_type &x) { return t.find(x); }
    const_iterator find(const key_type &x) const { return t.find(x); }
    size_type count(const key_type &x) const { return t.count(x); }
    iterator lower_bound(const key_type &x) { return t.lower_bound(x); }
    const_iterator lower_bound(const key_type &x) const
    {
      return t.lower_bound(x);
    }
    iterator upper_bound(const key_type &x) { return t.upper_bound(x); }
    const_iterator upper_bound(const key_type &x) const
    {
      return t.upper_bound(x);
    }
    pair<iterator, iterator> equal_range(const key_type &x)
    {
      return t.equal_range(x);
    }
    pair<const_iterator, const_iterator> equal_range(const key_type &x) const
    {
      return t.equal_range(x);
    }
    friend bool operator== __STL_NULL_TMPL_ARGS(const multimap &,
                                                const multimap &);
    friend bool operator<__STL_NULL_TMPL_ARGS(const multimap &,
                                              const multimap &);
  };

  template <class Key, class T, class Compare, class Alloc>
  inline bool operator==(const multimap<Key, T, Compare, Alloc> &x,
                        const multimap<Key, T, Compare, Alloc> &y)
  {
    return x.t == y.t;
  }

  template <class Key, class T, class Compare, class Alloc>
  inline bool operator<(const multimap<Key, T, Compare, Alloc> &x,
                        const multimap<Key, T, Compare, Alloc> &y)
  {
    return x.t < y.t;
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class Key, class T, class Compare, class Alloc>
  inline void swap(multimap<Key, T, Compare, Alloc> &x,
                  multimap<Key, T, Compare, Alloc> &y)
  {
    x.swap(y);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */
  ```
  </details>

### 容器hash_set、hash_multiset、hash_map和hash_multimap

  底层用哈希表实现的无序集合。

  [stl_hash_set.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_hash_set.h)

  <details>
  <summary>hash_set</summary>

  ```C++
  //哈希集合
  template <class Value, class HashFcn = hash<Value>,
            class EqualKey = equal_to<Value>,
            class Alloc = alloc>
  class hash_set
  {
  private:
    //底层使用哈希表
    typedef hashtable<Value, Value, HashFcn, identity<Value>,
                      EqualKey, Alloc>
        ht;
    ht rep;

  public:
    typedef typename ht::key_type key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;

    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;
    typedef typename ht::const_pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::const_reference reference;
    typedef typename ht::const_reference const_reference;

    // hash_set不支持修改元素，使用常量迭代器
    typedef typename ht::const_iterator iterator;
    typedef typename ht::const_iterator const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }

  public:
    hash_set() : rep(100, hasher(), key_equal()) {}
    explicit hash_set(size_type n) : rep(n, hasher(), key_equal()) {}
    hash_set(size_type n, const hasher &hf) : rep(n, hf, key_equal()) {}
    hash_set(size_type n, const hasher &hf, const key_equal &eql)
        : rep(n, hf, eql) {}

    template <class InputIterator>
    hash_set(InputIterator f, InputIterator l)
        : rep(100, hasher(), key_equal())
    {
      rep.insert_unique(f, l);
    }
    template <class InputIterator>
    hash_set(InputIterator f, InputIterator l, size_type n)
        : rep(n, hasher(), key_equal()) { rep.insert_unique(f, l); }
    template <class InputIterator>
    hash_set(InputIterator f, InputIterator l, size_type n,
            const hasher &hf)
        : rep(n, hf, key_equal()) { rep.insert_unique(f, l); }
    template <class InputIterator>
    hash_set(InputIterator f, InputIterator l, size_type n,
            const hasher &hf, const key_equal &eql)
        : rep(n, hf, eql) { rep.insert_unique(f, l); }

  public:
    //各个操作转而使用底层哈希表
    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    void swap(hash_set &hs) { rep.swap(hs.rep); }
    friend bool operator== __STL_NULL_TMPL_ARGS(const hash_set &,
                                                const hash_set &);

    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }

  public:
    pair<iterator, bool> insert(const value_type &obj)
    {
      // hash_set不支持元素重复
      pair<typename ht::iterator, bool> p = rep.insert_unique(obj);
      return pair<iterator, bool>(p.first, p.second);
    }

    template <class InputIterator>
    void insert(InputIterator f, InputIterator l)
    {
      rep.insert_unique(f, l);
    }

    pair<iterator, bool> insert_noresize(const value_type &obj)
    {
      pair<typename ht::iterator, bool> p = rep.insert_unique_noresize(obj);
      return pair<iterator, bool>(p.first, p.second);
    }

    iterator find(const key_type &key) const { return rep.find(key); }

    size_type count(const key_type &key) const { return rep.count(key); }

    pair<iterator, iterator> equal_range(const key_type &key) const
    {
      return rep.equal_range(key);
    }

    size_type erase(const key_type &key) { return rep.erase(key); }
    void erase(iterator it) { rep.erase(it); }
    void erase(iterator f, iterator l) { rep.erase(f, l); }
    void clear() { rep.clear(); }

  public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const
    {
      return rep.elems_in_bucket(n);
    }
  };

  template <class Value, class HashFcn, class EqualKey, class Alloc>
  inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc> &hs1,
                        const hash_set<Value, HashFcn, EqualKey, Alloc> &hs2)
  {
    return hs1.rep == hs2.rep;
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class Val, class HashFcn, class EqualKey, class Alloc>
  inline void swap(hash_set<Val, HashFcn, EqualKey, Alloc> &hs1,
                  hash_set<Val, HashFcn, EqualKey, Alloc> &hs2)
  {
    hs1.swap(hs2);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

  //可重复哈希集合
  template <class Value, class HashFcn = hash<Value>,
            class EqualKey = equal_to<Value>,
            class Alloc = alloc>
  class hash_multiset
  {
  private:
    typedef hashtable<Value, Value, HashFcn, identity<Value>,
                      EqualKey, Alloc>
        ht;
    ht rep;

  public:
    typedef typename ht::key_type key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;

    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;
    typedef typename ht::const_pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::const_reference reference;
    typedef typename ht::const_reference const_reference;

    typedef typename ht::const_iterator iterator;
    typedef typename ht::const_iterator const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }

  public:
    hash_multiset() : rep(100, hasher(), key_equal()) {}
    explicit hash_multiset(size_type n) : rep(n, hasher(), key_equal()) {}
    hash_multiset(size_type n, const hasher &hf) : rep(n, hf, key_equal()) {}
    hash_multiset(size_type n, const hasher &hf, const key_equal &eql)
        : rep(n, hf, eql) {}

    template <class InputIterator>
    hash_multiset(InputIterator f, InputIterator l)
        : rep(100, hasher(), key_equal())
    {
      rep.insert_equal(f, l);
    }
    template <class InputIterator>
    hash_multiset(InputIterator f, InputIterator l, size_type n)
        : rep(n, hasher(), key_equal()) { rep.insert_equal(f, l); }
    template <class InputIterator>
    hash_multiset(InputIterator f, InputIterator l, size_type n,
                  const hasher &hf)
        : rep(n, hf, key_equal()) { rep.insert_equal(f, l); }
    template <class InputIterator>
    hash_multiset(InputIterator f, InputIterator l, size_type n,
                  const hasher &hf, const key_equal &eql)
        : rep(n, hf, eql) { rep.insert_equal(f, l); }

  public:
    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    void swap(hash_multiset &hs) { rep.swap(hs.rep); }
    friend bool operator== __STL_NULL_TMPL_ARGS(const hash_multiset &,
                                                const hash_multiset &);

    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }

  public:
    iterator insert(const value_type &obj) { return rep.insert_equal(obj); }

    template <class InputIterator>
    void insert(InputIterator f, InputIterator l)
    {
      rep.insert_equal(f, l);
    }

    iterator insert_noresize(const value_type &obj)
    {
      return rep.insert_equal_noresize(obj);
    }

    iterator find(const key_type &key) const { return rep.find(key); }

    size_type count(const key_type &key) const { return rep.count(key); }

    pair<iterator, iterator> equal_range(const key_type &key) const
    {
      return rep.equal_range(key);
    }

    size_type erase(const key_type &key) { return rep.erase(key); }
    void erase(iterator it) { rep.erase(it); }
    void erase(iterator f, iterator l) { rep.erase(f, l); }
    void clear() { rep.clear(); }

  public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const
    {
      return rep.elems_in_bucket(n);
    }
  };

  template <class Val, class HashFcn, class EqualKey, class Alloc>
  inline bool operator==(const hash_multiset<Val, HashFcn, EqualKey, Alloc> &hs1,
                        const hash_multiset<Val, HashFcn, EqualKey, Alloc> &hs2)
  {
    return hs1.rep == hs2.rep;
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class Val, class HashFcn, class EqualKey, class Alloc>
  inline void swap(hash_multiset<Val, HashFcn, EqualKey, Alloc> &hs1,
                  hash_multiset<Val, HashFcn, EqualKey, Alloc> &hs2)
  {
    hs1.swap(hs2);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */
  ```
  </details>

  [stl_hash_map.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_hash_map.h)

  <details>
  <summary>hash_map</summary>

  ```C++
  //哈希映射
  template <class Key, class T, class HashFcn = hash<Key>,
            class EqualKey = equal_to<Key>,
            class Alloc = alloc>
  class hash_map
  {
  private:
    //底层使用哈希表
    typedef hashtable<pair<const Key, T>, Key, HashFcn,
                      select1st<pair<const Key, T>>, EqualKey, Alloc>
        ht;
    ht rep;

  public:
    typedef typename ht::key_type key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;

    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;
    typedef typename ht::pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::reference reference;
    typedef typename ht::const_reference const_reference;

    typedef typename ht::iterator iterator;
    typedef typename ht::const_iterator const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }

  public:
    hash_map() : rep(100, hasher(), key_equal()) {}
    explicit hash_map(size_type n) : rep(n, hasher(), key_equal()) {}
    hash_map(size_type n, const hasher &hf) : rep(n, hf, key_equal()) {}
    hash_map(size_type n, const hasher &hf, const key_equal &eql)
        : rep(n, hf, eql) {}

    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l)
        : rep(100, hasher(), key_equal())
    {
      rep.insert_unique(f, l);
    }
    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n)
        : rep(n, hasher(), key_equal()) { rep.insert_unique(f, l); }
    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n,
            const hasher &hf)
        : rep(n, hf, key_equal()) { rep.insert_unique(f, l); }
    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n,
            const hasher &hf, const key_equal &eql)
        : rep(n, hf, eql) { rep.insert_unique(f, l); }

  public:
    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    void swap(hash_map &hs) { rep.swap(hs.rep); }
    friend bool
    operator== __STL_NULL_TMPL_ARGS(const hash_map &, const hash_map &);

    iterator begin() { return rep.begin(); }
    iterator end() { return rep.end(); }
    const_iterator begin() const { return rep.begin(); }
    const_iterator end() const { return rep.end(); }

  public:
    pair<iterator, bool> insert(const value_type &obj)
    {
      return rep.insert_unique(obj);
    }

    template <class InputIterator>
    void insert(InputIterator f, InputIterator l)
    {
      rep.insert_unique(f, l);
    }

    pair<iterator, bool> insert_noresize(const value_type &obj)
    {
      return rep.insert_unique_noresize(obj);
    }

    iterator find(const key_type &key) { return rep.find(key); }
    const_iterator find(const key_type &key) const { return rep.find(key); }

    T &operator[](const key_type &key)
    {
      return rep.find_or_insert(value_type(key, T())).second;
    }

    size_type count(const key_type &key) const { return rep.count(key); }

    pair<iterator, iterator> equal_range(const key_type &key)
    {
      return rep.equal_range(key);
    }
    pair<const_iterator, const_iterator> equal_range(const key_type &key) const
    {
      return rep.equal_range(key);
    }

    size_type erase(const key_type &key) { return rep.erase(key); }
    void erase(iterator it) { rep.erase(it); }
    void erase(iterator f, iterator l) { rep.erase(f, l); }
    void clear() { rep.clear(); }

  public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const
    {
      return rep.elems_in_bucket(n);
    }
  };

  template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
  inline bool operator==(const hash_map<Key, T, HashFcn, EqualKey, Alloc> &hm1,
                        const hash_map<Key, T, HashFcn, EqualKey, Alloc> &hm2)
  {
    return hm1.rep == hm2.rep;
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
  inline void swap(hash_map<Key, T, HashFcn, EqualKey, Alloc> &hm1,
                  hash_map<Key, T, HashFcn, EqualKey, Alloc> &hm2)
  {
    hm1.swap(hm2);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

  //可重复哈希映射
  template <class Key, class T, class HashFcn = hash<Key>,
            class EqualKey = equal_to<Key>,
            class Alloc = alloc>
  class hash_multimap
  {
  private:
    typedef hashtable<pair<const Key, T>, Key, HashFcn,
                      select1st<pair<const Key, T>>, EqualKey, Alloc>
        ht;
    ht rep;

  public:
    typedef typename ht::key_type key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;

    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;
    typedef typename ht::pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::reference reference;
    typedef typename ht::const_reference const_reference;

    typedef typename ht::iterator iterator;
    typedef typename ht::const_iterator const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }

  public:
    hash_multimap() : rep(100, hasher(), key_equal()) {}
    explicit hash_multimap(size_type n) : rep(n, hasher(), key_equal()) {}
    hash_multimap(size_type n, const hasher &hf) : rep(n, hf, key_equal()) {}
    hash_multimap(size_type n, const hasher &hf, const key_equal &eql)
        : rep(n, hf, eql) {}

    template <class InputIterator>
    hash_multimap(InputIterator f, InputIterator l)
        : rep(100, hasher(), key_equal())
    {
      rep.insert_equal(f, l);
    }
    template <class InputIterator>
    hash_multimap(InputIterator f, InputIterator l, size_type n)
        : rep(n, hasher(), key_equal()) { rep.insert_equal(f, l); }
    template <class InputIterator>
    hash_multimap(InputIterator f, InputIterator l, size_type n,
                  const hasher &hf)
        : rep(n, hf, key_equal()) { rep.insert_equal(f, l); }
    template <class InputIterator>
    hash_multimap(InputIterator f, InputIterator l, size_type n,
                  const hasher &hf, const key_equal &eql)
        : rep(n, hf, eql) { rep.insert_equal(f, l); }

  public:
    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    void swap(hash_multimap &hs) { rep.swap(hs.rep); }
    friend bool
    operator== __STL_NULL_TMPL_ARGS(const hash_multimap &, const hash_multimap &);

    iterator begin() { return rep.begin(); }
    iterator end() { return rep.end(); }
    const_iterator begin() const { return rep.begin(); }
    const_iterator end() const { return rep.end(); }

  public:
    iterator insert(const value_type &obj) { return rep.insert_equal(obj); }

    template <class InputIterator>
    void insert(InputIterator f, InputIterator l)
    {
      rep.insert_equal(f, l);
    }

    iterator insert_noresize(const value_type &obj)
    {
      return rep.insert_equal_noresize(obj);
    }

    iterator find(const key_type &key) { return rep.find(key); }
    const_iterator find(const key_type &key) const { return rep.find(key); }

    size_type count(const key_type &key) const { return rep.count(key); }

    pair<iterator, iterator> equal_range(const key_type &key)
    {
      return rep.equal_range(key);
    }
    pair<const_iterator, const_iterator> equal_range(const key_type &key) const
    {
      return rep.equal_range(key);
    }

    size_type erase(const key_type &key) { return rep.erase(key); }
    void erase(iterator it) { rep.erase(it); }
    void erase(iterator f, iterator l) { rep.erase(f, l); }
    void clear() { rep.clear(); }

  public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const
    {
      return rep.elems_in_bucket(n);
    }
  };

  template <class Key, class T, class HF, class EqKey, class Alloc>
  inline bool operator==(const hash_multimap<Key, T, HF, EqKey, Alloc> &hm1,
                        const hash_multimap<Key, T, HF, EqKey, Alloc> &hm2)
  {
    return hm1.rep == hm2.rep;
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
  inline void swap(hash_multimap<Key, T, HashFcn, EqualKey, Alloc> &hm1,
                  hash_multimap<Key, T, HashFcn, EqualKey, Alloc> &hm2)
  {
    hm1.swap(hm2);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */
  ```
  </details>
