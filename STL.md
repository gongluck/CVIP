# STL标准模板库

- [STL标准模板库](#stl标准模板库)
  - [分配器``allocator``](#分配器allocator)
    - [标准分配器](#标准分配器)
    - [``SGI``特殊分配器](#sgi特殊分配器)
      - [构造和析构工具](#构造和析构工具)
      - [内存分配和释放](#内存分配和释放)
    - [内存基本处理工具](#内存基本处理工具)
  - [迭代器 ``iterator``](#迭代器-iterator)
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

## 分配器``allocator``

### 标准分配器

  包装``::operator new``和``::operator delete``。

  [defalloc.h](https://github.com/gongluck/sourcecode/blob/main/stl/defalloc.h)

### ``SGI``特殊分配器

#### 构造和析构工具

  ``construct``利用``placement new``实现。
  ``destroy``调用析构函数或者利用``萃取特性``进而调用遍历析构或者特化的版本。

  [stl_construct.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_construct.h)

#### 内存分配和释放

  ![内存池分配器](https://github.com/gongluck/images/blob/main/内存池分配器.png)

  ``SGI``设计了双层策略。
  第一级配置器``__malloc_alloc_template``简单包装了``malloc``和``free``，并在内存分配失败时调用失败处理函数。
  第二级配置器``__default_alloc_template``使用了``内存池``策略，使用``free list``实现。

  [stl_alloc.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_alloc.h)

### 内存基本处理工具

  ``uninitialized_copy``、``uninitialized_fill``和``uninitialized_fill_n``在目标内存调用拷贝构造函数或者内存拷贝函数。

  [stl_uninitialized.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_uninitialized.h)

## 迭代器 ``iterator``

```C++
// 迭代器必需定义类型
typedef typename _Iterator::iterator_category iterator_category;
typedef typename _Iterator::value_type value_type;
typedef typename _Iterator::difference_type difference_type;
typedef typename _Iterator::pointer pointer;
typedef typename _Iterator::reference reference;
```

<iframe src="https://github.com/gongluck/sourcecode/blob/main/stl/stl_iterator_base_types.h" />

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
