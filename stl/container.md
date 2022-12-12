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
    - [容器unordered\_set、unordered\_multiset 和unordered\_map、unordered\_multimap ](#容器unordered_setunordered_multiset-和unordered_mapunordered_multimap-)

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

### 容器[set](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/stl_set.h#L85)和[multiset](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/stl_multiset.h#L82)

- set和multiset底层使用红黑树实现，`key==value`。

### 容器[map](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/stl_map.h#L84)和[multimap](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/stl_multimap.h#L83)

- map和multimap底层使用红黑树实现，value通过key计算。

### 容器[unordered_set](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/tr1_impl/unordered_set#L117)、[unordered_multiset ](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/tr1_impl/unordered_set#L179)和[unordered_map](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/tr1_impl/unordered_map#L120)、[unordered_multimap ](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/tr1_impl/unordered_map#L185)

- 底层用哈希表实现的无序集合。
