# 容器```container```

- [容器```container```](#容器container)
  - [序列式容器```sequence container```](#序列式容器sequence-container)
    - [容器```vector```](#容器vector)
    - [容器```list```](#容器list)
    - [容器```slist```](#容器slist)
    - [容器```deque```](#容器deque)

## 序列式容器```sequence container```

### 容器```vector```

  ![容器vector](https://github.com/gongluck/images/blob/main/stl/容器vector.png)

  根据不同的实现，```vector```数据成员至少包含```3```个```T*```类型的指针。分别为```start```、```finish```、```end_of_storage```。```vector::iterator```数据成员至少包含```1```个```T*```类型的指针。

  [stl_vector.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_vector.h)

  <details>
  <summary>vector</summary>

  ```C++
  //向量
  template <class T, class Alloc = alloc>
  class vector
  {
  public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

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
    typedef simple_alloc<value_type, Alloc> data_allocator;
    iterator start;          //开始位置
    iterator finish;         //结束位置
    iterator end_of_storage; //内存结束
    void insert_aux(iterator position, const T &x);
    //销毁内存
    void deallocate()
    {
      if (start)
        data_allocator::deallocate(start, end_of_storage - start);
    }

    //初始化并填充初值
    void fill_initialize(size_type n, const T &value)
    {
      start = allocate_and_fill(n, value);
      finish = start + n;
      end_of_storage = finish;
    }

  public:
    iterator begin() { return start; }
    const_iterator begin() const { return start; }
    iterator end() { return finish; }
    const_iterator end() const { return finish; }
    //因为反向迭代器的operator*是先--再*this，所以将end()初始化反向迭代器作为rbegin()是正确的
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }
    size_type size() const { return size_type(end() - begin()); }
    size_type max_size() const { return size_type(-1) / sizeof(T); }
    size_type capacity() const { return size_type(end_of_storage - begin()); }
    bool empty() const { return begin() == end(); }
    reference operator[](size_type n) { return *(begin() + n); }
    const_reference operator[](size_type n) const { return *(begin() + n); }

    vector() : start(0), finish(0), end_of_storage(0) {}
    vector(size_type n, const T &value) { fill_initialize(n, value); }
    vector(int n, const T &value) { fill_initialize(n, value); }
    vector(long n, const T &value) { fill_initialize(n, value); }
    explicit vector(size_type n) { fill_initialize(n, T()); }

    vector(const vector<T, Alloc> &x)
    {
      start = allocate_and_copy(x.end() - x.begin(), x.begin(), x.end());
      finish = start + (x.end() - x.begin());
      end_of_storage = finish;
    }

    vector(const_iterator first, const_iterator last)
    {
      size_type n = 0;
      distance(first, last, n);
      start = allocate_and_copy(n, first, last);
      finish = start + n;
      end_of_storage = finish;
    }

    ~vector()
    {
      destroy(start, finish);
      deallocate();
    }
    vector<T, Alloc> &operator=(const vector<T, Alloc> &x);
    //调整大小
    void reserve(size_type n)
    {
      //扩容
      if (capacity() < n)
      {
        const size_type old_size = size();
        iterator tmp = allocate_and_copy(n, start, finish);
        //析构旧元素
        destroy(start, finish);
        //销毁旧空间
        deallocate();
        start = tmp;
        finish = tmp + old_size;
        end_of_storage = start + n;
      }
    }
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }
    void push_back(const T &x)
    {
      if (finish != end_of_storage)
      {
        //在备用空间原地构造
        construct(finish, x);
        ++finish;
      }
      else
        //尾部插入
        insert_aux(end(), x);
    }
    void swap(vector<T, Alloc> &x)
    {
      __STD::swap(start, x.start);
      __STD::swap(finish, x.finish);
      __STD::swap(end_of_storage, x.end_of_storage);
    }
    iterator insert(iterator position, const T &x)
    {
      size_type n = position - begin();
      if (finish != end_of_storage && position == end())
      {
        construct(finish, x);
        ++finish;
      }
      else
        insert_aux(position, x);
      return begin() + n;
    }
    iterator insert(iterator position) { return insert(position, T()); }

    void insert(iterator position,
                const_iterator first, const_iterator last);

    void insert(iterator pos, size_type n, const T &x);
    void insert(iterator pos, int n, const T &x)
    {
      insert(pos, (size_type)n, x);
    }
    void insert(iterator pos, long n, const T &x)
    {
      insert(pos, (size_type)n, x);
    }

    void pop_back()
    {
      --finish;
      destroy(finish);
    }
    iterator erase(iterator position)
    {
      //非尾部最后一个元素，删除点后的元素全部前移一位
      if (position + 1 != end())
        copy(position + 1, finish, position);
      --finish;
      //析构新放入备用区的元素
      destroy(finish);
      return position;
    }
    iterator erase(iterator first, iterator last)
    {
      iterator i = copy(last, finish, first);
      destroy(i, finish);
      finish = finish - (last - first);
      return first;
    }
    void resize(size_type new_size, const T &x)
    {
      if (new_size < size())
        erase(begin() + new_size, end());
      else
        insert(end(), new_size - size(), x);
    }
    void resize(size_type new_size) { resize(new_size, T()); }
    void clear() { erase(begin(), end()); }

  protected:
    iterator allocate_and_fill(size_type n, const T &x)
    {
      iterator result = data_allocator::allocate(n);
      __STL_TRY
      {
        uninitialized_fill_n(result, n, x);
        return result;
      }
      __STL_UNWIND(data_allocator::deallocate(result, n));
    }

    iterator allocate_and_copy(size_type n,
                              const_iterator first, const_iterator last)
    {
      iterator result = data_allocator::allocate(n);
      __STL_TRY
      {
        uninitialized_copy(first, last, result);
        return result;
      }
      __STL_UNWIND(data_allocator::deallocate(result, n));
    }
  };

  template <class T, class Alloc>
  inline bool operator==(const vector<T, Alloc> &x, const vector<T, Alloc> &y)
  {
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
  }

  template <class T, class Alloc>
  inline bool operator<(const vector<T, Alloc> &x, const vector<T, Alloc> &y)
  {
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
  }

  template <class T, class Alloc>
  vector<T, Alloc> &vector<T, Alloc>::operator=(const vector<T, Alloc> &x)
  {
    if (&x != this)
    {
      if (x.size() > capacity())
      {
        iterator tmp = allocate_and_copy(x.end() - x.begin(),
                                        x.begin(), x.end());
        destroy(start, finish);
        deallocate();
        start = tmp;
        end_of_storage = start + (x.end() - x.begin());
      }
      else if (size() >= x.size())
      {
        iterator i = copy(x.begin(), x.end(), begin());
        destroy(i, finish);
      }
      // x.size() <= capacity() && x.size() > size()
      else
      {
        //拷贝新元素填充整个size()区域，operator=
        copy(x.begin(), x.begin() + size(), start);
        //在剩余区域使用原地构造
        uninitialized_copy(x.begin() + size(), x.end(), finish);
      }
      finish = start + x.size();
    }
    return *this;
  }

  template <class T, class Alloc>
  void vector<T, Alloc>::insert_aux(iterator position, const T &x)
  {
    if (finish != end_of_storage)
    {
      //此时finish是未初始化的，需要先使用构造
      construct(finish, *(finish - 1));
      ++finish;
      T x_copy = x;
      //元素后移，copy_backward(position, finish-1, finish)更好理解。但最后的元素已经正确赋值，所以用下面的方式移动
      copy_backward(position, finish - 2, finish - 1);
      *position = x_copy;
    }
    else
    {
      const size_type old_size = size();
      const size_type len = old_size != 0 ? 2 * old_size : 1;
      iterator new_start = data_allocator::allocate(len);
      iterator new_finish = new_start;
      __STL_TRY
      {
        new_finish = uninitialized_copy(start, position, new_start);
        construct(new_finish, x);
        ++new_finish;
        new_finish = uninitialized_copy(position, finish, new_finish);
      }

  #ifdef __STL_USE_EXCEPTIONS
      catch (...)
      {
        destroy(new_start, new_finish);
        data_allocator::deallocate(new_start, len);
        throw;
      }
  #endif /* __STL_USE_EXCEPTIONS */
      destroy(begin(), end());
      deallocate();
      start = new_start;
      finish = new_finish;
      end_of_storage = new_start + len;
    }
  }

  template <class T, class Alloc>
  void vector<T, Alloc>::insert(iterator position, size_type n, const T &x)
  {
    if (n != 0)
    {
      //备用空间足够
      if (size_type(end_of_storage - finish) >= n)
      {
        T x_copy = x;
        //插入点后的元素个数
        const size_type elems_after = finish - position;
        iterator old_finish = finish;
        if (elems_after > n)
        {
          //后n个元素，拷贝(构造)到原结束点后
          uninitialized_copy(finish - n, finish, finish);
          //新结束点
          finish += n;
          //将插入点后未移动的元素，后移动n个位置
          copy_backward(position, old_finish - n, old_finish);
          //插入点后n个元素赋值
          fill(position, position + n, x_copy);
        }
        else
        {
          uninitialized_fill_n(finish, n - elems_after, x_copy);
          finish += n - elems_after;
          uninitialized_copy(position, old_finish, finish);
          finish += elems_after;
          fill(position, old_finish, x_copy);
        }
      }
      //备用空间不足
      else
      {
        const size_type old_size = size();
        const size_type len = old_size + max(old_size, n);
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        __STL_TRY
        {
          new_finish = uninitialized_copy(start, position, new_start);
          new_finish = uninitialized_fill_n(new_finish, n, x);
          new_finish = uninitialized_copy(position, finish, new_finish);
        }
  #ifdef __STL_USE_EXCEPTIONS
        catch (...)
        {
          destroy(new_start, new_finish);
          data_allocator::deallocate(new_start, len);
          throw;
        }
  #endif /* __STL_USE_EXCEPTIONS */
        destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
      }
    }
  }

  template <class T, class Alloc>
  void vector<T, Alloc>::insert(iterator position,
                                const_iterator first,
                                const_iterator last)
  {
    if (first != last)
    {
      size_type n = 0;
      distance(first, last, n);
      if (size_type(end_of_storage - finish) >= n)
      {
        const size_type elems_after = finish - position;
        iterator old_finish = finish;
        if (elems_after > n)
        {
          uninitialized_copy(finish - n, finish, finish);
          finish += n;
          copy_backward(position, old_finish - n, old_finish);
          copy(first, last, position);
        }
        else
        {
          uninitialized_copy(first + elems_after, last, finish);
          finish += n - elems_after;
          uninitialized_copy(position, old_finish, finish);
          finish += elems_after;
          copy(first, first + elems_after, position);
        }
      }
      else
      {
        const size_type old_size = size();
        const size_type len = old_size + max(old_size, n);
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        __STL_TRY
        {
          new_finish = uninitialized_copy(start, position, new_start);
          new_finish = uninitialized_copy(first, last, new_finish);
          new_finish = uninitialized_copy(position, finish, new_finish);
        }
  #ifdef __STL_USE_EXCEPTIONS
        catch (...)
        {
          destroy(new_start, new_finish);
          data_allocator::deallocate(new_start, len);
          throw;
        }
  #endif /* __STL_USE_EXCEPTIONS */
        destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
      }
    }
  }
  ```
  </details>

### 容器```list```

  ![容器list](https://github.com/gongluck/images/blob/main/stl/容器list.png)
  ![容器list迭代器](https://github.com/gongluck/images/blob/main/stl/容器list迭代器.png)

  ```node```节点由前后指针和数据成员```T```组成。
  ```list```包含一个```node```指针指向链表```哨兵```节点。
  ```list::iterator```包含一个```node```指针指向链表节点。

  [stl_list.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_list.h)

  <details>
  <summary>list</summary>

  ```C++
  //链表节点
  template <class T>
  struct __list_node
  {
    typedef void *void_pointer;
    void_pointer next; //指向下一个节点
    void_pointer prev; //指向上一个节点
    T data;            //节点数据
  };

  //链表迭代器
  template <class T, class Ref, class Ptr>
  struct __list_iterator
  {
    typedef __list_iterator<T, T &, T *> iterator;
    typedef __list_iterator<T, const T &, const T *> const_iterator;
    typedef __list_iterator<T, Ref, Ptr> self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __list_node<T> *link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    link_type node; //指向节点

    __list_iterator(link_type x) : node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator &x) : node(x.node) {}

    bool operator==(const self &x) const { return node == x.node; }
    bool operator!=(const self &x) const { return node != x.node; }
    reference operator*() const { return (*node).data; }

  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const
    {
      return &(operator*());
    }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */

    self &operator++()
    {
      node = (link_type)((*node).next);
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      ++*this;
      return tmp;
    }
    self &operator--()
    {
      node = (link_type)((*node).prev);
      return *this;
    }
    self operator--(int)
    {
      self tmp = *this;
      --*this;
      return tmp;
    }
  };

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  //链表迭代器属于双向迭代器类别
  template <class T, class Ref, class Ptr>
  inline bidirectional_iterator_tag
  iterator_category(const __list_iterator<T, Ref, Ptr> &)
  {
    return bidirectional_iterator_tag();
  }

  template <class T, class Ref, class Ptr>
  inline T *
  value_type(const __list_iterator<T, Ref, Ptr> &)
  {
    return 0;
  }

  template <class T, class Ref, class Ptr>
  inline ptrdiff_t *
  distance_type(const __list_iterator<T, Ref, Ptr> &)
  {
    return 0;
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  //链表
  template <class T, class Alloc = alloc>
  class list
  {
  protected:
    typedef void *void_pointer;
    typedef __list_node<T> list_node;
    typedef simple_alloc<list_node, Alloc> list_node_allocator;

  public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef list_node *link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  public:
    typedef __list_iterator<T, T &, T *> iterator;
    typedef __list_iterator<T, const T &, const T *> const_iterator;

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
    //空间分配和释放
    link_type get_node() { return list_node_allocator::allocate(); }
    void put_node(link_type p) { list_node_allocator::deallocate(p); }

    link_type create_node(const T &x)
    {
      link_type p = get_node();
      __STL_TRY
      {
        construct(&p->data, x);
      }
      __STL_UNWIND(put_node(p));
      return p;
    }
    void destroy_node(link_type p)
    {
      destroy(&p->data);
      put_node(p);
    }

  protected:
    void empty_initialize()
    {
      //分配哨兵节点空间
      node = get_node();
      // next和prev都指向哨兵节点代表空链表
      node->next = node;
      node->prev = node;
    }

    void fill_initialize(size_type n, const T &value)
    {
      empty_initialize();
      __STL_TRY
      {
        insert(begin(), n, value);
      }
      __STL_UNWIND(clear(); put_node(node));
    }

    void range_initialize(const T *first, const T *last)
    {
      empty_initialize();
      __STL_TRY
      {
        insert(begin(), first, last);
      }
      __STL_UNWIND(clear(); put_node(node));
    }
    void range_initialize(const_iterator first, const_iterator last)
    {
      empty_initialize();
      __STL_TRY
      {
        insert(begin(), first, last);
      }
      __STL_UNWIND(clear(); put_node(node));
    }

  protected:
    link_type node; //头节点，哨兵

  public:
    list() { empty_initialize(); }

    iterator begin() { return (link_type)((*node).next); }
    const_iterator begin() const { return (link_type)((*node).next); }
    iterator end() { return node; }
    const_iterator end() const { return node; }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }
    bool empty() const { return node->next == node; }
    size_type size() const
    {
      size_type result = 0;
      //链表迭代器是双向迭代器类别，distance使用了遍历方式
      distance(begin(), end(), result);
      return result;
    }
    size_type max_size() const { return size_type(-1); }
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }
    void swap(list<T, Alloc> &x) { __STD::swap(node, x.node); }
    iterator insert(iterator position, const T &x)
    {
      link_type tmp = create_node(x);
      tmp->next = position.node;
      tmp->prev = position.node->prev;
      //头插法
      (link_type(position.node->prev))->next = tmp;
      position.node->prev = tmp;
      return tmp;
    }
    iterator insert(iterator position) { return insert(position, T()); }
  #ifdef __STL_MEMBER_TEMPLATES
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);
  #else  /* __STL_MEMBER_TEMPLATES */
    void insert(iterator position, const T *first, const T *last);
    void insert(iterator position,
                const_iterator first, const_iterator last);
  #endif /* __STL_MEMBER_TEMPLATES */
    void insert(iterator pos, size_type n, const T &x);
    void insert(iterator pos, int n, const T &x)
    {
      insert(pos, (size_type)n, x);
    }
    void insert(iterator pos, long n, const T &x)
    {
      insert(pos, (size_type)n, x);
    }

    void push_front(const T &x) { insert(begin(), x); }
    void push_back(const T &x) { insert(end(), x); }
    iterator erase(iterator position)
    {
      link_type next_node = link_type(position.node->next);
      link_type prev_node = link_type(position.node->prev);
      prev_node->next = next_node;
      next_node->prev = prev_node;
      destroy_node(position.node);
      return iterator(next_node);
    }
    iterator erase(iterator first, iterator last);
    void resize(size_type new_size, const T &x);
    void resize(size_type new_size) { resize(new_size, T()); }
    void clear();

    void pop_front() { erase(begin()); }
    void pop_back()
    {
      iterator tmp = end();
      erase(--tmp);
    }
    list(size_type n, const T &value) { fill_initialize(n, value); }
    list(int n, const T &value) { fill_initialize(n, value); }
    list(long n, const T &value) { fill_initialize(n, value); }
    explicit list(size_type n) { fill_initialize(n, T()); }

    list(const T *first, const T *last)
    {
      range_initialize(first, last);
    }
    list(const_iterator first, const_iterator last)
    {
      range_initialize(first, last);
    }

    list(const list<T, Alloc> &x)
    {
      range_initialize(x.begin(), x.end());
    }
    ~list()
    {
      clear();
      put_node(node);
    }
    list<T, Alloc> &operator=(const list<T, Alloc> &x);

  protected:
    //[first, last)迁移到position前
    void transfer(iterator position, iterator first, iterator last)
    {
      if (position != last)
      {
        (*(link_type((*last.node).prev))).next = position.node;
        (*(link_type((*first.node).prev))).next = last.node;
        (*(link_type((*position.node).prev))).next = first.node;
        link_type tmp = link_type((*position.node).prev);
        (*position.node).prev = (*last.node).prev;
        (*last.node).prev = (*first.node).prev;
        (*first.node).prev = tmp;
      }
    }

  public:
    void splice(iterator position, list &x)
    {
      if (!x.empty())
        transfer(position, x.begin(), x.end());
    }
    void splice(iterator position, list &, iterator i)
    {
      iterator j = i;
      ++j;
      if (position == i || position == j)
        return;
      transfer(position, i, j);
    }
    void splice(iterator position, list &, iterator first, iterator last)
    {
      if (first != last)
        transfer(position, first, last);
    }
    void remove(const T &value);
    void unique();
    void merge(list &x);
    void reverse();
    void sort();

    friend bool operator== __STL_NULL_TMPL_ARGS(const list &x, const list &y);
  };

  template <class T, class Alloc>
  inline bool operator==(const list<T, Alloc> &x, const list<T, Alloc> &y)
  {
    typedef typename list<T, Alloc>::link_type link_type;
    link_type e1 = x.node;
    link_type e2 = y.node;
    link_type n1 = (link_type)e1->next;
    link_type n2 = (link_type)e2->next;
    for (; n1 != e1 && n2 != e2;
        n1 = (link_type)n1->next, n2 = (link_type)n2->next)
      if (n1->data != n2->data)
        return false;
    return n1 == e1 && n2 == e2;
  }

  template <class T, class Alloc>
  inline bool operator<(const list<T, Alloc> &x, const list<T, Alloc> &y)
  {
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class T, class Alloc>
  inline void swap(list<T, Alloc> &x, list<T, Alloc> &y)
  {
    x.swap(y);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

  template <class T, class Alloc>
  void list<T, Alloc>::insert(iterator position, const T *first, const T *last)
  {
    for (; first != last; ++first)
      insert(position, *first);
  }

  template <class T, class Alloc>
  void list<T, Alloc>::insert(iterator position,
                              const_iterator first, const_iterator last)
  {
    for (; first != last; ++first)
      insert(position, *first);
  }

  template <class T, class Alloc>
  void list<T, Alloc>::insert(iterator position, size_type n, const T &x)
  {
    for (; n > 0; --n)
      insert(position, x);
  }

  template <class T, class Alloc>
  list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last)
  {
    while (first != last)
      erase(first++);
    return last;
  }

  template <class T, class Alloc>
  void list<T, Alloc>::resize(size_type new_size, const T &x)
  {
    iterator i = begin();
    size_type len = 0;
    for (; i != end() && len < new_size; ++i, ++len)
      ;
    if (len == new_size)
      erase(i, end());
    else // i == end()
      insert(end(), new_size - len, x);
  }

  template <class T, class Alloc>
  void list<T, Alloc>::clear()
  {
    link_type cur = (link_type)node->next;
    while (cur != node)
    {
      link_type tmp = cur;
      cur = (link_type)cur->next;
      destroy_node(tmp);
    }
    node->next = node;
    node->prev = node;
  }

  template <class T, class Alloc>
  list<T, Alloc> &list<T, Alloc>::operator=(const list<T, Alloc> &x)
  {
    if (this != &x)
    {
      iterator first1 = begin();
      iterator last1 = end();
      const_iterator first2 = x.begin();
      const_iterator last2 = x.end();
      while (first1 != last1 && first2 != last2)
        *first1++ = *first2++;
      if (first2 == last2)
        erase(first1, last1);
      else
        insert(last1, first2, last2);
    }
    return *this;
  }

  template <class T, class Alloc>
  void list<T, Alloc>::remove(const T &value)
  {
    iterator first = begin();
    iterator last = end();
    while (first != last)
    {
      iterator next = first;
      ++next;
      if (*first == value)
        erase(first);
      first = next;
    }
  }

  template <class T, class Alloc>
  void list<T, Alloc>::unique()
  {
    iterator first = begin();
    iterator last = end();
    if (first == last)
      return;
    iterator next = first;
    while (++next != last)
    {
      if (*first == *next)
        erase(next);
      else
        first = next;
      next = first;
    }
  }

  template <class T, class Alloc>
  void list<T, Alloc>::merge(list<T, Alloc> &x)
  {
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();
    while (first1 != last1 && first2 != last2)
      if (*first2 < *first1)
      {
        iterator next = first2;
        transfer(first1, first2, ++next);
        first2 = next;
      }
      else
        ++first1;
    if (first2 != last2)
      transfer(last1, first2, last2);
  }

  template <class T, class Alloc>
  void list<T, Alloc>::reverse()
  {
    if (node->next == node || link_type(node->next)->next == node)
      return;
    iterator first = begin();
    ++first;
    while (first != end())
    {
      iterator old = first;
      ++first;
      transfer(begin(), old, first);
    }
  }

  template <class T, class Alloc>
  void list<T, Alloc>::sort()
  {
    if (node->next == node || link_type(node->next)->next == node)
      return;
    list<T, Alloc> carry;
    list<T, Alloc> counter[64];
    int fill = 0;
    while (!empty())
    {
      carry.splice(carry.begin(), *this, begin());
      int i = 0;
      while (i < fill && !counter[i].empty())
      {
        counter[i].merge(carry);
        carry.swap(counter[i++]);
      }
      carry.swap(counter[i]);
      if (i == fill)
        ++fill;
    }

    for (int i = 1; i < fill; ++i)
      counter[i].merge(counter[i - 1]);
    swap(counter[fill - 1]);
  }
  ```
  </details>

### 容器```slist```

  ```node```节点由后指针和数据成员```T```组成。
  ```slist```包含一个```node```指针指向单向链表头节点(哨兵)。
  ```slist::iterator```包含一个```node```指针指向链表节点。

  [stl_slist.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_slist.h)

  <details>
  <summary>slist</summary>

  ```C++
  //单向链表节点基类
  struct __slist_node_base
  {
    __slist_node_base *next; //指向下一个节点
  };

  // prev -> new
  inline __slist_node_base *__slist_make_link(__slist_node_base *prev_node,
                                              __slist_node_base *new_node)
  {
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    return new_node;
  }

  //遍历计算出prev节点
  inline __slist_node_base *__slist_previous(__slist_node_base *head,
                                            const __slist_node_base *node)
  {
    while (head && head->next != node)
      head = head->next;
    return head;
  }

  inline const __slist_node_base *__slist_previous(const __slist_node_base *head,
                                                  const __slist_node_base *node)
  {
    while (head && head->next != node)
      head = head->next;
    return head;
  }

  // pos后插入(first, last]
  inline void __slist_splice_after(__slist_node_base *pos,
                                  __slist_node_base *before_first,
                                  __slist_node_base *before_last)
  {
    if (pos != before_first && pos != before_last)
    {
      __slist_node_base *first = before_first->next;
      __slist_node_base *after = pos->next;
      before_first->next = before_last->next;
      pos->next = first;
      before_last->next = after;
    }
  }

  //反转单链表
  inline __slist_node_base *__slist_reverse(__slist_node_base *node)
  {
    __slist_node_base *result = node;
    node = node->next;
    result->next = 0;
    while (node)
    {
      __slist_node_base *next = node->next;
      node->next = result;
      result = node;
      node = next;
    }
    return result;
  }

  //单链表节点
  template <class T>
  struct __slist_node : public __slist_node_base
  {
    T data; //数据
  };

  //单链表迭代器基类
  struct __slist_iterator_base
  {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef forward_iterator_tag iterator_category;

    __slist_node_base *node; //指向单链表节点

    __slist_iterator_base(__slist_node_base *x) : node(x) {}
    void incr() { node = node->next; }

    bool operator==(const __slist_iterator_base &x) const
    {
      return node == x.node;
    }
    bool operator!=(const __slist_iterator_base &x) const
    {
      return node != x.node;
    }
  };

  //单链表迭代器
  template <class T, class Ref, class Ptr>
  struct __slist_iterator : public __slist_iterator_base
  {
    typedef __slist_iterator<T, T &, T *> iterator;
    typedef __slist_iterator<T, const T &, const T *> const_iterator;
    typedef __slist_iterator<T, Ref, Ptr> self;

    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __slist_node<T> list_node;

    __slist_iterator(list_node *x) : __slist_iterator_base(x) {}
    __slist_iterator() : __slist_iterator_base(0) {}
    __slist_iterator(const iterator &x) : __slist_iterator_base(x.node) {}

    reference operator*() const { return ((list_node *)node)->data; }
  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const
    {
      return &(operator*());
    }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */

    self &operator++()
    {
      incr();
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      incr();
      return tmp;
    }
  };

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  inline ptrdiff_t *
  distance_type(const __slist_iterator_base &)
  {
    return 0;
  }

  //单链表迭代器属于可遍历迭代器类别
  inline forward_iterator_tag
  iterator_category(const __slist_iterator_base &)
  {
    return forward_iterator_tag();
  }

  template <class T, class Ref, class Ptr>
  inline T *
  value_type(const __slist_iterator<T, Ref, Ptr> &)
  {
    return 0;
  }

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  inline size_t __slist_size(__slist_node_base *node)
  {
    size_t result = 0;
    for (; node != 0; node = node->next)
      ++result;
    return result;
  }

  //单链表
  template <class T, class Alloc = alloc>
  class slist
  {
  public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef __slist_iterator<T, T &, T *> iterator;
    typedef __slist_iterator<T, const T &, const T *> const_iterator;

  private:
    typedef __slist_node<T> list_node;
    typedef __slist_node_base list_node_base;
    typedef __slist_iterator_base iterator_base;
    typedef simple_alloc<list_node, Alloc> list_node_allocator;

    //创建节点
    static list_node *create_node(const value_type &x)
    {
      list_node *node = list_node_allocator::allocate();
      __STL_TRY
      {
        construct(&node->data, x);
        node->next = 0;
      }
      __STL_UNWIND(list_node_allocator::deallocate(node));
      return node;
    }

    //销毁节点
    static void destroy_node(list_node *node)
    {
      destroy(&node->data);
      list_node_allocator::deallocate(node);
    }

    void fill_initialize(size_type n, const value_type &x)
    {
      head.next = 0;
      __STL_TRY
      {
        _insert_after_fill(&head, n, x);
      }
      __STL_UNWIND(clear());
    }

    void range_initialize(const value_type *first, const value_type *last)
    {
      head.next = 0;
      __STL_TRY
      {
        _insert_after_range(&head, first, last);
      }
      __STL_UNWIND(clear());
    }
    void range_initialize(const_iterator first, const_iterator last)
    {
      head.next = 0;
      __STL_TRY
      {
        _insert_after_range(&head, first, last);
      }
      __STL_UNWIND(clear());
    }

  private:
    list_node_base head; //头节点，哨兵

  public:
    slist() { head.next = 0; }

    slist(size_type n, const value_type &x) { fill_initialize(n, x); }
    slist(int n, const value_type &x) { fill_initialize(n, x); }
    slist(long n, const value_type &x) { fill_initialize(n, x); }
    explicit slist(size_type n) { fill_initialize(n, value_type()); }

    slist(const_iterator first, const_iterator last)
    {
      range_initialize(first, last);
    }
    slist(const value_type *first, const value_type *last)
    {
      range_initialize(first, last);
    }

    slist(const slist &L)
    {
      range_initialize(L.begin(), L.end());
    }

    slist &operator=(const slist &L);

    ~slist() { clear(); }

  public:
    iterator begin() { return iterator((list_node *)head.next); }
    const_iterator begin() const { return const_iterator((list_node *)head.next); }

    iterator end() { return iterator(0); }
    const_iterator end() const { return const_iterator(0); }

    size_type size() const { return __slist_size(head.next); }

    size_type max_size() const { return size_type(-1); }

    bool empty() const { return head.next == 0; }

    void swap(slist &L)
    {
      list_node_base *tmp = head.next;
      head.next = L.head.next;
      L.head.next = tmp;
    }

  public:
    friend bool operator== __STL_NULL_TMPL_ARGS(const slist<T, Alloc> &L1,
                                                const slist<T, Alloc> &L2);

  public:
    reference front() { return ((list_node *)head.next)->data; }
    const_reference front() const { return ((list_node *)head.next)->data; }
    void push_front(const value_type &x)
    {
      __slist_make_link(&head, create_node(x));
    }
    void pop_front()
    {
      list_node *node = (list_node *)head.next;
      head.next = node->next;
      destroy_node(node);
    }

    iterator previous(const_iterator pos)
    {
      return iterator((list_node *)__slist_previous(&head, pos.node));
    }
    const_iterator previous(const_iterator pos) const
    {
      return const_iterator((list_node *)__slist_previous(&head, pos.node));
    }

  private:
    list_node *_insert_after(list_node_base *pos, const value_type &x)
    {
      return (list_node *)(__slist_make_link(pos, create_node(x)));
    }

    void _insert_after_fill(list_node_base *pos,
                            size_type n, const value_type &x)
    {
      for (size_type i = 0; i < n; ++i)
        pos = __slist_make_link(pos, create_node(x));
    }

    void _insert_after_range(list_node_base *pos,
                            const_iterator first, const_iterator last)
    {
      while (first != last)
      {
        pos = __slist_make_link(pos, create_node(*first));
        ++first;
      }
    }
    void _insert_after_range(list_node_base *pos,
                            const value_type *first, const value_type *last)
    {
      while (first != last)
      {
        pos = __slist_make_link(pos, create_node(*first));
        ++first;
      }
    }

    list_node_base *erase_after(list_node_base *pos)
    {
      list_node *next = (list_node *)(pos->next);
      list_node_base *next_next = next->next;
      pos->next = next_next;
      destroy_node(next);
      return next_next;
    }

    list_node_base *erase_after(list_node_base *before_first,
                                list_node_base *last_node)
    {
      list_node *cur = (list_node *)(before_first->next);
      while (cur != last_node)
      {
        list_node *tmp = cur;
        cur = (list_node *)cur->next;
        destroy_node(tmp);
      }
      before_first->next = last_node;
      return last_node;
    }

  public:
    iterator insert_after(iterator pos, const value_type &x)
    {
      return iterator(_insert_after(pos.node, x));
    }

    iterator insert_after(iterator pos)
    {
      return insert_after(pos, value_type());
    }

    void insert_after(iterator pos, size_type n, const value_type &x)
    {
      _insert_after_fill(pos.node, n, x);
    }
    void insert_after(iterator pos, int n, const value_type &x)
    {
      _insert_after_fill(pos.node, (size_type)n, x);
    }
    void insert_after(iterator pos, long n, const value_type &x)
    {
      _insert_after_fill(pos.node, (size_type)n, x);
    }

    void insert_after(iterator pos, const_iterator first, const_iterator last)
    {
      _insert_after_range(pos.node, first, last);
    }
    void insert_after(iterator pos,
                      const value_type *first, const value_type *last)
    {
      _insert_after_range(pos.node, first, last);
    }

    iterator insert(iterator pos, const value_type &x)
    {
      return iterator(_insert_after(__slist_previous(&head, pos.node), x));
    }

    iterator insert(iterator pos)
    {
      return iterator(_insert_after(__slist_previous(&head, pos.node),
                                    value_type()));
    }

    void insert(iterator pos, size_type n, const value_type &x)
    {
      _insert_after_fill(__slist_previous(&head, pos.node), n, x);
    }
    void insert(iterator pos, int n, const value_type &x)
    {
      _insert_after_fill(__slist_previous(&head, pos.node), (size_type)n, x);
    }
    void insert(iterator pos, long n, const value_type &x)
    {
      _insert_after_fill(__slist_previous(&head, pos.node), (size_type)n, x);
    }

    void insert(iterator pos, const_iterator first, const_iterator last)
    {
      _insert_after_range(__slist_previous(&head, pos.node), first, last);
    }
    void insert(iterator pos, const value_type *first, const value_type *last)
    {
      _insert_after_range(__slist_previous(&head, pos.node), first, last);
    }

  public:
    iterator erase_after(iterator pos)
    {
      return iterator((list_node *)erase_after(pos.node));
    }
    iterator erase_after(iterator before_first, iterator last)
    {
      return iterator((list_node *)erase_after(before_first.node, last.node));
    }

    iterator erase(iterator pos)
    {
      return (list_node *)erase_after(__slist_previous(&head, pos.node));
    }
    iterator erase(iterator first, iterator last)
    {
      return (list_node *)erase_after(__slist_previous(&head, first.node),
                                      last.node);
    }

    void resize(size_type new_size, const T &x);
    void resize(size_type new_size) { resize(new_size, T()); }
    void clear() { erase_after(&head, 0); }

  public:
    // Moves the range [before_first + 1, before_last + 1) to *this,
    //  inserting it immediately after pos.  This is constant time.
    void splice_after(iterator pos,
                      iterator before_first, iterator before_last)
    {
      if (before_first != before_last)
        __slist_splice_after(pos.node, before_first.node, before_last.node);
    }

    // Moves the element that follows prev to *this, inserting it immediately
    //  after pos.  This is constant time.
    void splice_after(iterator pos, iterator prev)
    {
      __slist_splice_after(pos.node, prev.node, prev.node->next);
    }

    // Linear in distance(begin(), pos), and linear in L.size().
    void splice(iterator pos, slist &L)
    {
      if (L.head.next)
        __slist_splice_after(__slist_previous(&head, pos.node),
                            &L.head,
                            __slist_previous(&L.head, 0));
    }

    // Linear in distance(begin(), pos), and in distance(L.begin(), i).
    void splice(iterator pos, slist &L, iterator i)
    {
      __slist_splice_after(__slist_previous(&head, pos.node),
                          __slist_previous(&L.head, i.node),
                          i.node);
    }

    // Linear in distance(begin(), pos), in distance(L.begin(), first),
    // and in distance(first, last).
    void splice(iterator pos, slist &L, iterator first, iterator last)
    {
      if (first != last)
        __slist_splice_after(__slist_previous(&head, pos.node),
                            __slist_previous(&L.head, first.node),
                            __slist_previous(first.node, last.node));
    }

  public:
    void reverse()
    {
      if (head.next)
        head.next = __slist_reverse(head.next);
    }

    void remove(const T &val);
    void unique();
    void merge(slist &L);
    void sort();
  };

  template <class T, class Alloc>
  slist<T, Alloc> &slist<T, Alloc>::operator=(const slist<T, Alloc> &L)
  {
    if (&L != this)
    {
      list_node_base *p1 = &head;
      list_node *n1 = (list_node *)head.next;
      const list_node *n2 = (const list_node *)L.head.next;
      while (n1 && n2)
      {
        n1->data = n2->data;
        p1 = n1;
        n1 = (list_node *)n1->next;
        n2 = (const list_node *)n2->next;
      }
      if (n2 == 0)
        erase_after(p1, 0);
      else
        _insert_after_range(p1,
                            const_iterator((list_node *)n2), const_iterator(0));
    }
    return *this;
  }

  template <class T, class Alloc>
  bool operator==(const slist<T, Alloc> &L1, const slist<T, Alloc> &L2)
  {
    typedef typename slist<T, Alloc>::list_node list_node;
    list_node *n1 = (list_node *)L1.head.next;
    list_node *n2 = (list_node *)L2.head.next;
    while (n1 && n2 && n1->data == n2->data)
    {
      n1 = (list_node *)n1->next;
      n2 = (list_node *)n2->next;
    }
    return n1 == 0 && n2 == 0;
  }

  template <class T, class Alloc>
  inline bool operator<(const slist<T, Alloc> &L1, const slist<T, Alloc> &L2)
  {
    return lexicographical_compare(L1.begin(), L1.end(), L2.begin(), L2.end());
  }

  #ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

  template <class T, class Alloc>
  inline void swap(slist<T, Alloc> &x, slist<T, Alloc> &y)
  {
    x.swap(y);
  }

  #endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

  template <class T, class Alloc>
  void slist<T, Alloc>::resize(size_type len, const T &x)
  {
    list_node_base *cur = &head;
    while (cur->next != 0 && len > 0)
    {
      --len;
      cur = cur->next;
    }
    if (cur->next)
      //删除多余节点
      erase_after(cur, 0);
    else
      //填充缺少的节点
      _insert_after_fill(cur, len, x);
  }

  template <class T, class Alloc>
  void slist<T, Alloc>::remove(const T &val)
  {
    list_node_base *cur = &head;
    while (cur && cur->next)
    {
      if (((list_node *)cur->next)->data == val)
        erase_after(cur);
      else
        cur = cur->next;
    }
  }

  template <class T, class Alloc>
  void slist<T, Alloc>::unique()
  {
    list_node_base *cur = head.next;
    if (cur)
    {
      while (cur->next)
      {
        if (((list_node *)cur)->data == ((list_node *)(cur->next))->data)
          erase_after(cur);
        else
          cur = cur->next;
      }
    }
  }

  template <class T, class Alloc>
  void slist<T, Alloc>::merge(slist<T, Alloc> &L)
  {
    list_node_base *n1 = &head;
    while (n1->next && L.head.next)
    {
      if (((list_node *)L.head.next)->data < ((list_node *)n1->next)->data)
        __slist_splice_after(n1, &L.head, L.head.next);
      n1 = n1->next;
    }
    if (L.head.next)
    {
      n1->next = L.head.next;
      L.head.next = 0;
    }
  }

  template <class T, class Alloc>
  void slist<T, Alloc>::sort()
  {
    if (head.next && head.next->next)
    {
      slist carry;
      slist counter[64];
      int fill = 0;
      while (!empty())
      {
        __slist_splice_after(&carry.head, &head, head.next);
        int i = 0;
        while (i < fill && !counter[i].empty())
        {
          counter[i].merge(carry);
          carry.swap(counter[i]);
          ++i;
        }
        carry.swap(counter[i]);
        if (i == fill)
          ++fill;
      }

      for (int i = 1; i < fill; ++i)
        counter[i].merge(counter[i - 1]);
      this->swap(counter[fill - 1]);
    }
  }

  ```
  </details>

### 容器```deque```

  ![容器deque](https://github.com/gongluck/images/blob/main/stl/容器deque.png)
  ![容器deque迭代器](https://github.com/gongluck/images/blob/main/stl/容器deque迭代器.png)

  ```deque```包含指向管控中心的指针```m_map```、控制中心的大小和分别指向控制中心开始和结束的两个```迭代器```。
  ```deque::iterator```包含```3```个```T*```类型的指针和```1```个```T**```类型指针。分别为```cur```、```first```、 ```last```指向直接内存的位置，```m_node```指向管控中心的节点位置。

  [stl_deque.h](https://github.com/gongluck/sourcecode/blob/main/stl/stl_deque.h)

  <details>
  <summary>deque</summary>

  ```C++
  // Note: this function is simply a kludge to work around several compilers'
  //  bugs in handling constant expressions.
  inline size_t __deque_buf_size(size_t n, size_t sz)
  {
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
  }

  //双向队列迭代器
  #ifndef __STL_NON_TYPE_TMPL_PARAM_BUG
  template <class T, class Ref, class Ptr, size_t BufSiz>
  struct __deque_iterator
  {
    typedef __deque_iterator<T, T &, T *, BufSiz> iterator;
    typedef __deque_iterator<T, const T &, const T *, BufSiz> const_iterator;
    static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }
  #else /* __STL_NON_TYPE_TMPL_PARAM_BUG */
  template <class T, class Ref, class Ptr>
  struct __deque_iterator
  {
    typedef __deque_iterator<T, T &, T *> iterator;
    typedef __deque_iterator<T, const T &, const T *> const_iterator;
    static size_t buffer_size() { return __deque_buf_size(0, sizeof(T)); }
  #endif

    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T **map_pointer;

    typedef __deque_iterator self;

    T *cur;           //指向当前元素
    T *first;         //指向当前元素所在的内存块开始
    T *last;          //指向当前元素所在的内存块结束
    map_pointer node; //中继器

    __deque_iterator(T *x, map_pointer y)
        : cur(x), first(*y), last(*y + buffer_size()), node(y) {}
    __deque_iterator() : cur(0), first(0), last(0), node(0) {}
    __deque_iterator(const iterator &x)
        : cur(x.cur), first(x.first), last(x.last), node(x.node) {}

    reference operator*() const { return *cur; }
  #ifndef __SGI_STL_NO_ARROW_OPERATOR
    pointer operator->() const
    {
      return &(operator*());
    }
  #endif /* __SGI_STL_NO_ARROW_OPERATOR */

    difference_type operator-(const self &x) const
    {
      return difference_type(buffer_size()) * (node - x.node - 1) +
            (cur - first) + (x.last - x.cur);
    }

    self &operator++()
    {
      ++cur;
      //抵达内存块的最后
      if (cur == last)
      {
        //使用后面的内存块
        set_node(node + 1);
        cur = first;
      }
      return *this;
    }
    self operator++(int)
    {
      self tmp = *this;
      ++*this;
      return tmp;
    }

    self &operator--()
    {
      //抵达内存块的最前
      if (cur == first)
      {
        //使用前面的内存块
        set_node(node - 1);
        cur = last;
      }
      --cur;
      return *this;
    }
    self operator--(int)
    {
      self tmp = *this;
      --*this;
      return tmp;
    }

    self &operator+=(difference_type n)
    {
      //计算从当前内存块开始的偏移
      difference_type offset = n + (cur - first);
      //偏移不超出当前内存块
      if (offset >= 0 && offset < difference_type(buffer_size()))
        //移动当前指向即可
        cur += n;
      //偏移超过当前内存块
      else
      {
        //计算内存块在中继器的偏移
        difference_type node_offset =
            offset > 0 ? offset / difference_type(buffer_size())
                      : -difference_type((-offset - 1) / buffer_size()) - 1;
        //设置新的内存块
        set_node(node + node_offset);
        cur = first + (offset - node_offset * difference_type(buffer_size()));
      }
      return *this;
    }

    self operator+(difference_type n) const
    {
      self tmp = *this;
      return tmp += n;
    }

    self &operator-=(difference_type n) { return *this += -n; }

    self operator-(difference_type n) const
    {
      self tmp = *this;
      return tmp -= n;
    }

    reference operator[](difference_type n) const { return *(*this + n); }

    bool operator==(const self &x) const { return cur == x.cur; }
    bool operator!=(const self &x) const { return !(*this == x); }
    bool operator<(const self &x) const
    {
      return (node == x.node) ? (cur < x.cur) : (node < x.node);
    }

    //设置使用内存块
    void set_node(map_pointer new_node)
    {
      node = new_node;
      first = *new_node;
      last = first + difference_type(buffer_size());
    }
  };

  #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

  #ifndef __STL_NON_TYPE_TMPL_PARAM_BUG

  //双向队列迭代器属于随机迭代器类别
  template <class T, class Ref, class Ptr, size_t BufSiz>
  inline random_access_iterator_tag
  iterator_category(const __deque_iterator<T, Ref, Ptr, BufSiz> &)
  {
    return random_access_iterator_tag();
  }

  template <class T, class Ref, class Ptr, size_t BufSiz>
  inline T *value_type(const __deque_iterator<T, Ref, Ptr, BufSiz> &)
  {
    return 0;
  }

  template <class T, class Ref, class Ptr, size_t BufSiz>
  inline ptrdiff_t *distance_type(const __deque_iterator<T, Ref, Ptr, BufSiz> &)
  {
    return 0;
  }

  #else /* __STL_NON_TYPE_TMPL_PARAM_BUG */

  template <class T, class Ref, class Ptr>
  inline random_access_iterator_tag
  iterator_category(const __deque_iterator<T, Ref, Ptr> &)
  {
    return random_access_iterator_tag();
  }

  template <class T, class Ref, class Ptr>
  inline T *value_type(const __deque_iterator<T, Ref, Ptr> &) { return 0; }

  template <class T, class Ref, class Ptr>
  inline ptrdiff_t *distance_type(const __deque_iterator<T, Ref, Ptr> &)
  {
    return 0;
  }

  #endif /* __STL_NON_TYPE_TMPL_PARAM_BUG */

  #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

  // See __deque_buf_size().  The only reason that the default value is 0
  //  is as a workaround for bugs in the way that some compilers handle
  //  constant expressions.

  //双向队列
  template <class T, class Alloc = alloc, size_t BufSiz = 0>
  class deque
  {
  public: // Basic types
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  public: // Iterators
  #ifndef __STL_NON_TYPE_TMPL_PARAM_BUG
    typedef __deque_iterator<T, T &, T *, BufSiz> iterator;
    typedef __deque_iterator<T, const T &, const T &, BufSiz> const_iterator;
  #else  /* __STL_NON_TYPE_TMPL_PARAM_BUG */
    typedef __deque_iterator<T, T &, T *> iterator;
    typedef __deque_iterator<T, const T &, const T *> const_iterator;
  #endif /* __STL_NON_TYPE_TMPL_PARAM_BUG */

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
    typedef pointer *map_pointer;
    typedef simple_alloc<value_type, Alloc> data_allocator;
    typedef simple_alloc<pointer, Alloc> map_allocator;

    static size_type buffer_size()
    {
      return __deque_buf_size(BufSiz, sizeof(value_type));
    }
    static size_type initial_map_size() { return 8; }

  protected:         // Data members
    iterator start;  // deque开始
    iterator finish; // deque结束

    map_pointer map;    //中继器
    size_type map_size; //中继器大小

  public: // Basic accessors
    iterator begin() { return start; }
    iterator end() { return finish; }
    const_iterator begin() const { return start; }
    const_iterator end() const { return finish; }

    reverse_iterator rbegin() { return reverse_iterator(finish); }
    reverse_iterator rend() { return reverse_iterator(start); }
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(finish);
    }
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(start);
    }

    reference operator[](size_type n) { return start[difference_type(n)]; }
    const_reference operator[](size_type n) const
    {
      return start[difference_type(n)];
    }

    reference front() { return *start; }
    reference back()
    {
      iterator tmp = finish;
      --tmp;
      return *tmp;
    }
    const_reference front() const { return *start; }
    const_reference back() const
    {
      const_iterator tmp = finish;
      --tmp;
      return *tmp;
    }

    size_type size() const
    {
      return finish - start;
      ;
    }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return finish == start; }

  public: // Constructor, destructor.
    deque()
        : start(), finish(), map(0), map_size(0)
    {
      create_map_and_nodes(0);
    }

    deque(const deque &x)
        : start(), finish(), map(0), map_size(0)
    {
      create_map_and_nodes(x.size());
      __STL_TRY
      {
        uninitialized_copy(x.begin(), x.end(), start);
      }
      __STL_UNWIND(destroy_map_and_nodes());
    }

    deque(size_type n, const value_type &value)
        : start(), finish(), map(0), map_size(0)
    {
      fill_initialize(n, value);
    }

    deque(int n, const value_type &value)
        : start(), finish(), map(0), map_size(0)
    {
      fill_initialize(n, value);
    }

    deque(long n, const value_type &value)
        : start(), finish(), map(0), map_size(0)
    {
      fill_initialize(n, value);
    }

    explicit deque(size_type n)
        : start(), finish(), map(0), map_size(0)
    {
      fill_initialize(n, value_type());
    }

    deque(const value_type *first, const value_type *last)
        : start(), finish(), map(0), map_size(0)
    {
      create_map_and_nodes(last - first);
      __STL_TRY
      {
        uninitialized_copy(first, last, start);
      }
      __STL_UNWIND(destroy_map_and_nodes());
    }

    deque(const_iterator first, const_iterator last)
        : start(), finish(), map(0), map_size(0)
    {
      create_map_and_nodes(last - first);
      __STL_TRY
      {
        uninitialized_copy(first, last, start);
      }
      __STL_UNWIND(destroy_map_and_nodes());
    }

    ~deque()
    {
      destroy(start, finish);
      destroy_map_and_nodes();
    }

    deque &operator=(const deque &x)
    {
      const size_type len = size();
      if (&x != this)
      {
        if (len >= x.size())
          erase(copy(x.begin(), x.end(), start), finish);
        else
        {
          const_iterator mid = x.begin() + difference_type(len);
          copy(x.begin(), mid, start);
          insert(finish, mid, x.end());
        }
      }
      return *this;
    }

    void swap(deque &x)
    {
      __STD::swap(start, x.start);
      __STD::swap(finish, x.finish);
      __STD::swap(map, x.map);
      __STD::swap(map_size, x.map_size);
    }

  public: // push_* and pop_*
    void push_back(const value_type &t)
    {
      if (finish.cur != finish.last - 1)
      {
        construct(finish.cur, t);
        ++finish.cur;
      }
      else
        push_back_aux(t);
    }

    void push_front(const value_type &t)
    {
      if (start.cur != start.first)
      {
        construct(start.cur - 1, t);
        --start.cur;
      }
      else
        push_front_aux(t);
    }

    void pop_back()
    {
      if (finish.cur != finish.first)
      {
        --finish.cur;
        destroy(finish.cur);
      }
      else
        pop_back_aux();
    }

    void pop_front()
    {
      if (start.cur != start.last - 1)
      {
        destroy(start.cur);
        ++start.cur;
      }
      else
        pop_front_aux();
    }

  public: // Insert
    iterator insert(iterator position, const value_type &x)
    {
      if (position.cur == start.cur)
      {
        push_front(x);
        return start;
      }
      else if (position.cur == finish.cur)
      {
        push_back(x);
        iterator tmp = finish;
        --tmp;
        return tmp;
      }
      else
      {
        return insert_aux(position, x);
      }
    }

    iterator insert(iterator position) { return insert(position, value_type()); }

    void insert(iterator pos, size_type n, const value_type &x);

    void insert(iterator pos, int n, const value_type &x)
    {
      insert(pos, (size_type)n, x);
    }
    void insert(iterator pos, long n, const value_type &x)
    {
      insert(pos, (size_type)n, x);
    }

    void insert(iterator pos, const value_type *first, const value_type *last);
    void insert(iterator pos, const_iterator first, const_iterator last);

    void resize(size_type new_size, const value_type &x)
    {
      const size_type len = size();
      if (new_size < len)
        erase(start + new_size, finish);
      else
        insert(finish, new_size - len, x);
    }

    void resize(size_type new_size) { resize(new_size, value_type()); }

  public: // Erase
    iterator erase(iterator pos)
    {
      iterator next = pos;
      ++next;
      //计算删除点到起点的距离
      difference_type index = pos - start;
      //删除点离起点近
      if (index < (size() >> 1))
      {
        //移动前部元素
        copy_backward(start, pos, next);
        // pop最前面多余元素
        pop_front();
      }
      //删除点离起点远
      else
      {
        //移动后部元素
        copy(next, finish, pos);
        // pop最后面多余元素
        pop_back();
      }
      return start + index;
    }

    iterator erase(iterator first, iterator last);
    void clear();

  protected: // Internal construction/destruction
    void create_map_and_nodes(size_type num_elements);
    void destroy_map_and_nodes();
    void fill_initialize(size_type n, const value_type &value);

  protected: // Internal push_* and pop_*
    void push_back_aux(const value_type &t);
    void push_front_aux(const value_type &t);
    void pop_back_aux();
    void pop_front_aux();

  protected: // Internal insert functions
    iterator insert_aux(iterator pos, const value_type &x);
    void insert_aux(iterator pos, size_type n, const value_type &x);

    void insert_aux(iterator pos,
                    const value_type *first, const value_type *last,
                    size_type n);

    void insert_aux(iterator pos, const_iterator first, const_iterator last,
                    size_type n);

    iterator reserve_elements_at_front(size_type n)
    {
      size_type vacancies = start.cur - start.first;
      if (n > vacancies)
        new_elements_at_front(n - vacancies);
      return start - difference_type(n);
    }

    iterator reserve_elements_at_back(size_type n)
    {
      size_type vacancies = (finish.last - finish.cur) - 1;
      if (n > vacancies)
        new_elements_at_back(n - vacancies);
      return finish + difference_type(n);
    }

    void new_elements_at_front(size_type new_elements);
    void new_elements_at_back(size_type new_elements);

    void destroy_nodes_at_front(iterator before_start);
    void destroy_nodes_at_back(iterator after_finish);

  protected: // Allocation of map and nodes
    // Makes sure the map has space for new nodes.  Does not actually
    //  add the nodes.  Can invalidate map pointers.  (And consequently,
    //  deque iterators.)

    void reserve_map_at_back(size_type nodes_to_add = 1)
    {
      if (nodes_to_add + 1 > map_size - (finish.node - map))
        reallocate_map(nodes_to_add, false);
    }

    void reserve_map_at_front(size_type nodes_to_add = 1)
    {
      if (nodes_to_add > start.node - map)
        reallocate_map(nodes_to_add, true);
    }

    void reallocate_map(size_type nodes_to_add, bool add_at_front);

    pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
    void deallocate_node(pointer n)
    {
      data_allocator::deallocate(n, buffer_size());
    }

  #ifdef __STL_NON_TYPE_TMPL_PARAM_BUG
  public:
    bool operator==(const deque<T, Alloc, 0> &x) const
    {
      return size() == x.size() && equal(begin(), end(), x.begin());
    }
    bool operator!=(const deque<T, Alloc, 0> &x) const
    {
      return size() != x.size() || !equal(begin(), end(), x.begin());
    }
    bool operator<(const deque<T, Alloc, 0> &x) const
    {
      return lexicographical_compare(begin(), end(), x.begin(), x.end());
    }
  #endif /* __STL_NON_TYPE_TMPL_PARAM_BUG */
  };

  // Non-inline member functions

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::insert(iterator pos,
                                        size_type n, const value_type &x)
  {
    if (pos.cur == start.cur)
    {
      //从头部腾出空间
      iterator new_start = reserve_elements_at_front(n);
      //构造初始化元素
      uninitialized_fill(new_start, start, x);
      start = new_start;
    }
    else if (pos.cur == finish.cur)
    {
      //从尾部腾出空间
      iterator new_finish = reserve_elements_at_back(n);
      //构造初始化元素
      uninitialized_fill(finish, new_finish, x);
      finish = new_finish;
    }
    else
      insert_aux(pos, n, x);
  }

  #ifndef __STL_MEMBER_TEMPLATES

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::insert(iterator pos,
                                        const value_type *first,
                                        const value_type *last)
  {
    size_type n = last - first;
    if (pos.cur == start.cur)
    {
      iterator new_start = reserve_elements_at_front(n);
      __STL_TRY
      {
        uninitialized_copy(first, last, new_start);
        start = new_start;
      }
      __STL_UNWIND(destroy_nodes_at_front(new_start));
    }
    else if (pos.cur == finish.cur)
    {
      iterator new_finish = reserve_elements_at_back(n);
      __STL_TRY
      {
        uninitialized_copy(first, last, finish);
        finish = new_finish;
      }
      __STL_UNWIND(destroy_nodes_at_back(new_finish));
    }
    else
      insert_aux(pos, first, last, n);
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::insert(iterator pos,
                                        const_iterator first,
                                        const_iterator last)
  {
    size_type n = last - first;
    if (pos.cur == start.cur)
    {
      iterator new_start = reserve_elements_at_front(n);
      __STL_TRY
      {
        uninitialized_copy(first, last, new_start);
        start = new_start;
      }
      __STL_UNWIND(destroy_nodes_at_front(new_start));
    }
    else if (pos.cur == finish.cur)
    {
      iterator new_finish = reserve_elements_at_back(n);
      __STL_TRY
      {
        uninitialized_copy(first, last, finish);
        finish = new_finish;
      }
      __STL_UNWIND(destroy_nodes_at_back(new_finish));
    }
    else
      insert_aux(pos, first, last, n);
  }

  #endif /* __STL_MEMBER_TEMPLATES */

  template <class T, class Alloc, size_t BufSize>
  deque<T, Alloc, BufSize>::iterator
  deque<T, Alloc, BufSize>::erase(iterator first, iterator last)
  {
    if (first == start && last == finish)
    {
      clear();
      return finish;
    }
    else
    {
      difference_type n = last - first;
      difference_type elems_before = first - start;
      if (elems_before < (size() - n) / 2)
      {
        copy_backward(start, first, last);
        iterator new_start = start + n;
        destroy(start, new_start);
        for (map_pointer cur = start.node; cur < new_start.node; ++cur)
          data_allocator::deallocate(*cur, buffer_size());
        start = new_start;
      }
      else
      {
        copy(last, finish, first);
        iterator new_finish = finish - n;
        destroy(new_finish, finish);
        for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
          data_allocator::deallocate(*cur, buffer_size());
        finish = new_finish;
      }
      return start + elems_before;
    }
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::clear()
  {
    //处理头尾以外的缓冲区，这些缓冲区必然是全部被使用的
    for (map_pointer node = start.node + 1; node < finish.node; ++node)
    {
      //析构元素
      destroy(*node, *node + buffer_size());
      //销毁元素空间
      data_allocator::deallocate(*node, buffer_size());
    }

    if (start.node != finish.node)
    {
      destroy(start.cur, start.last);
      destroy(finish.first, finish.cur);
      //头部的缓冲区保留，所以只释放尾部缓冲区
      data_allocator::deallocate(finish.first, buffer_size());
    }
    else
      destroy(start.cur, finish.cur);

    finish = start;
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements)
  {
    //以下各种边界调整，保证中继器有充足空间
    size_type num_nodes = num_elements / buffer_size() + 1;
    map_size = max(initial_map_size(), num_nodes + 2);
    map = map_allocator::allocate(map_size);

    //先使用中继器中间部分空间
    map_pointer nstart = map + (map_size - num_nodes) / 2;
    map_pointer nfinish = nstart + num_nodes - 1;

    map_pointer cur;
    __STL_TRY
    {
      for (cur = nstart; cur <= nfinish; ++cur)
        *cur = allocate_node();
    }
  #ifdef __STL_USE_EXCEPTIONS
    catch (...)
    {
      for (map_pointer n = nstart; n < cur; ++n)
        deallocate_node(*n);
      map_allocator::deallocate(map, map_size);
      throw;
    }
  #endif /* __STL_USE_EXCEPTIONS */

    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    finish.cur = finish.first + num_elements % buffer_size();
  }

  // This is only used as a cleanup function in catch clauses.
  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::destroy_map_and_nodes()
  {
    for (map_pointer cur = start.node; cur <= finish.node; ++cur)
      deallocate_node(*cur);
    map_allocator::deallocate(map, map_size);
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::fill_initialize(size_type n,
                                                const value_type &value)
  {
    create_map_and_nodes(n);
    map_pointer cur;
    __STL_TRY
    {
      for (cur = start.node; cur < finish.node; ++cur)
        uninitialized_fill(*cur, *cur + buffer_size(), value);
      uninitialized_fill(finish.first, finish.cur, value);
    }
  #ifdef __STL_USE_EXCEPTIONS
    catch (...)
    {
      for (map_pointer n = start.node; n < cur; ++n)
        destroy(*n, *n + buffer_size());
      destroy_map_and_nodes();
      throw;
    }
  #endif /* __STL_USE_EXCEPTIONS */
  }

  // Called only if finish.cur == finish.last - 1.
  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::push_back_aux(const value_type &t)
  {
    value_type t_copy = t;
    reserve_map_at_back();
    *(finish.node + 1) = allocate_node();
    __STL_TRY
    {
      construct(finish.cur, t_copy);
      finish.set_node(finish.node + 1);
      finish.cur = finish.first;
    }
    __STL_UNWIND(deallocate_node(*(finish.node + 1)));
  }

  // Called only if start.cur == start.first.
  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::push_front_aux(const value_type &t)
  {
    value_type t_copy = t;
    reserve_map_at_front();
    *(start.node - 1) = allocate_node();
    __STL_TRY
    {
      start.set_node(start.node - 1);
      start.cur = start.last - 1;
      construct(start.cur, t_copy);
    }
  #ifdef __STL_USE_EXCEPTIONS
    catch (...)
    {
      start.set_node(start.node + 1);
      start.cur = start.first;
      deallocate_node(*(start.node - 1));
      throw;
    }
  #endif /* __STL_USE_EXCEPTIONS */
  }

  // Called only if finish.cur == finish.first.
  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::pop_back_aux()
  {
    deallocate_node(finish.first);
    finish.set_node(finish.node - 1);
    finish.cur = finish.last - 1;
    destroy(finish.cur);
  }

  // Called only if start.cur == start.last - 1.  Note that if the deque
  //  has at least one element (a necessary precondition for this member
  //  function), and if start.cur == start.last, then the deque must have
  //  at least two nodes.
  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::pop_front_aux()
  {
    destroy(start.cur);
    deallocate_node(start.first);
    start.set_node(start.node + 1);
    start.cur = start.first;
  }

  template <class T, class Alloc, size_t BufSize>
  typename deque<T, Alloc, BufSize>::iterator
  deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type &x)
  {
    difference_type index = pos - start;
    value_type x_copy = x;
    //插入点靠近头部
    if (index < size() / 2)
    {
      //头部扩充一个节点
      push_front(front());
      iterator front1 = start;
      ++front1;
      iterator front2 = front1;
      ++front2;
      pos = start + index;
      iterator pos1 = pos;
      ++pos1;
      //元素迁移
      copy(front2, pos1, front1);
    }
    //插入点靠近尾部
    else
    {
      //尾部扩充一个节点
      push_back(back());
      iterator back1 = finish;
      --back1;
      iterator back2 = back1;
      --back2;
      pos = start + index;
      //元素后移
      copy_backward(pos, back2, back1);
    }
    //插入
    *pos = x_copy;
    return pos;
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::insert_aux(iterator pos,
                                            size_type n, const value_type &x)
  {
    const difference_type elems_before = pos - start;
    size_type length = size();
    value_type x_copy = x;
    if (elems_before < length / 2)
    {
      iterator new_start = reserve_elements_at_front(n);
      iterator old_start = start;
      pos = start + elems_before;
      __STL_TRY
      {
        if (elems_before >= difference_type(n))
        {
          iterator start_n = start + difference_type(n);
          uninitialized_copy(start, start_n, new_start);
          start = new_start;
          copy(start_n, pos, old_start);
          fill(pos - difference_type(n), pos, x_copy);
        }
        else
        {
          __uninitialized_copy_fill(start, pos, new_start, start, x_copy);
          start = new_start;
          fill(old_start, pos, x_copy);
        }
      }
      __STL_UNWIND(destroy_nodes_at_front(new_start));
    }
    else
    {
      iterator new_finish = reserve_elements_at_back(n);
      iterator old_finish = finish;
      const difference_type elems_after = difference_type(length) - elems_before;
      pos = finish - elems_after;
      __STL_TRY
      {
        if (elems_after > difference_type(n))
        {
          iterator finish_n = finish - difference_type(n);
          uninitialized_copy(finish_n, finish, finish);
          finish = new_finish;
          copy_backward(pos, finish_n, old_finish);
          fill(pos, pos + difference_type(n), x_copy);
        }
        else
        {
          __uninitialized_fill_copy(finish, pos + difference_type(n),
                                    x_copy,
                                    pos, finish);
          finish = new_finish;
          fill(pos, old_finish, x_copy);
        }
      }
      __STL_UNWIND(destroy_nodes_at_back(new_finish));
    }
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::insert_aux(iterator pos,
                                            const value_type *first,
                                            const value_type *last,
                                            size_type n)
  {
    const difference_type elems_before = pos - start;
    size_type length = size();
    if (elems_before < length / 2)
    {
      iterator new_start = reserve_elements_at_front(n);
      iterator old_start = start;
      pos = start + elems_before;
      __STL_TRY
      {
        if (elems_before >= difference_type(n))
        {
          iterator start_n = start + difference_type(n);
          uninitialized_copy(start, start_n, new_start);
          start = new_start;
          copy(start_n, pos, old_start);
          copy(first, last, pos - difference_type(n));
        }
        else
        {
          const value_type *mid = first + (difference_type(n) - elems_before);
          __uninitialized_copy_copy(start, pos, first, mid, new_start);
          start = new_start;
          copy(mid, last, old_start);
        }
      }
      __STL_UNWIND(destroy_nodes_at_front(new_start));
    }
    else
    {
      iterator new_finish = reserve_elements_at_back(n);
      iterator old_finish = finish;
      const difference_type elems_after = difference_type(length) - elems_before;
      pos = finish - elems_after;
      __STL_TRY
      {
        if (elems_after > difference_type(n))
        {
          iterator finish_n = finish - difference_type(n);
          uninitialized_copy(finish_n, finish, finish);
          finish = new_finish;
          copy_backward(pos, finish_n, old_finish);
          copy(first, last, pos);
        }
        else
        {
          const value_type *mid = first + elems_after;
          __uninitialized_copy_copy(mid, last, pos, finish, finish);
          finish = new_finish;
          copy(first, mid, pos);
        }
      }
      __STL_UNWIND(destroy_nodes_at_back(new_finish));
    }
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::insert_aux(iterator pos,
                                            const_iterator first,
                                            const_iterator last,
                                            size_type n)
  {
    const difference_type elems_before = pos - start;
    size_type length = size();
    if (elems_before < length / 2)
    {
      iterator new_start = reserve_elements_at_front(n);
      iterator old_start = start;
      pos = start + elems_before;
      __STL_TRY
      {
        if (elems_before >= n)
        {
          iterator start_n = start + n;
          uninitialized_copy(start, start_n, new_start);
          start = new_start;
          copy(start_n, pos, old_start);
          copy(first, last, pos - difference_type(n));
        }
        else
        {
          const_iterator mid = first + (n - elems_before);
          __uninitialized_copy_copy(start, pos, first, mid, new_start);
          start = new_start;
          copy(mid, last, old_start);
        }
      }
      __STL_UNWIND(destroy_nodes_at_front(new_start));
    }
    else
    {
      iterator new_finish = reserve_elements_at_back(n);
      iterator old_finish = finish;
      const difference_type elems_after = length - elems_before;
      pos = finish - elems_after;
      __STL_TRY
      {
        if (elems_after > n)
        {
          iterator finish_n = finish - difference_type(n);
          uninitialized_copy(finish_n, finish, finish);
          finish = new_finish;
          copy_backward(pos, finish_n, old_finish);
          copy(first, last, pos);
        }
        else
        {
          const_iterator mid = first + elems_after;
          __uninitialized_copy_copy(mid, last, pos, finish, finish);
          finish = new_finish;
          copy(first, mid, pos);
        }
      }
      __STL_UNWIND(destroy_nodes_at_back(new_finish));
    }
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::new_elements_at_front(size_type new_elements)
  {
    size_type new_nodes = (new_elements + buffer_size() - 1) / buffer_size();
    reserve_map_at_front(new_nodes);
    size_type i;
    __STL_TRY
    {
      for (i = 1; i <= new_nodes; ++i)
        *(start.node - i) = allocate_node();
    }
  #ifdef __STL_USE_EXCEPTIONS
    catch (...)
    {
      for (size_type j = 1; j < i; ++j)
        deallocate_node(*(start.node - j));
      throw;
    }
  #endif /* __STL_USE_EXCEPTIONS */
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::new_elements_at_back(size_type new_elements)
  {
    size_type new_nodes = (new_elements + buffer_size() - 1) / buffer_size();
    reserve_map_at_back(new_nodes);
    size_type i;
    __STL_TRY
    {
      for (i = 1; i <= new_nodes; ++i)
        *(finish.node + i) = allocate_node();
    }
  #ifdef __STL_USE_EXCEPTIONS
    catch (...)
    {
      for (size_type j = 1; j < i; ++j)
        deallocate_node(*(finish.node + j));
      throw;
    }
  #endif /* __STL_USE_EXCEPTIONS */
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::destroy_nodes_at_front(iterator before_start)
  {
    for (map_pointer n = before_start.node; n < start.node; ++n)
      deallocate_node(*n);
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::destroy_nodes_at_back(iterator after_finish)
  {
    for (map_pointer n = after_finish.node; n > finish.node; --n)
      deallocate_node(*n);
  }

  template <class T, class Alloc, size_t BufSize>
  void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add,
                                                bool add_at_front)
  {
    size_type old_num_nodes = finish.node - start.node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;

    map_pointer new_nstart;
    if (map_size > 2 * new_num_nodes)
    {
      new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
      if (new_nstart < start.node)
        copy(start.node, finish.node + 1, new_nstart);
      else
        copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
    }
    else
    {
      size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;

      map_pointer new_map = map_allocator::allocate(new_map_size);
      new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
      copy(start.node, finish.node + 1, new_nstart);
      map_allocator::deallocate(map, map_size);

      map = new_map;
      map_size = new_map_size;
    }

    start.set_node(new_nstart);
    finish.set_node(new_nstart + old_num_nodes - 1);
  }

  // Nonmember functions.

  #ifndef __STL_NON_TYPE_TMPL_PARAM_BUG

  template <class T, class Alloc, size_t BufSiz>
  bool operator==(const deque<T, Alloc, BufSiz> &x,
                  const deque<T, Alloc, BufSiz> &y)
  {
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
  }

  template <class T, class Alloc, size_t BufSiz>
  bool operator<(const deque<T, Alloc, BufSiz> &x,
                const deque<T, Alloc, BufSiz> &y)
  {
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
  }

  #endif /* __STL_NON_TYPE_TMPL_PARAM_BUG */
  ```
  </details>
