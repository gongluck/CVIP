# 迭代器iterator

- [迭代器iterator](#迭代器iterator)
  - [迭代器](#迭代器)
  - [迭代器失效](#迭代器失效)

## [迭代器](https://github.com/gongluck/sourcecode/blob/main/gcc-4.4.0/libstdc++-v3/include/bits/stl_iterator.h#L652)

- 迭代器提供了对容器元素的访问方法，实现桥接容器和算法的方式。
- 迭代器一般由容器自生内部实现，以达到隐藏容器细节的作用。

## 迭代器失效

- 数组型容器(vector、deque)使用了连续分配的内存，删除当前的迭代器或者插入某个迭代器会使后面所有元素的迭代器都失效。
- 列表型容器(list)使用了不连续分配的内存，删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器。
- 树形型容器(map、set、multimap、multiset)删除当前的迭代器，仅仅会使当前的迭代器失效，插入、删除一个结点不会对其他结点造成影响。
