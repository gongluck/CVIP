# STL标准模板库

- [STL标准模板库](#stl标准模板库)
  - [分配器 ``allocator``](#分配器-allocator)

[STL六大模块](https://github.com/gongluck/images/blob/main/STL六大模块.png)

## 分配器 ``allocator``

  <details>
  <summary>defalloc.h</summary>

  ```C++
  template <class T>
  inline T* allocate(ptrdiff_t size, T*) {
      set_new_handler(0);
      T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
      if (tmp == 0) {
        cerr << "out of memory" << endl; 
        exit(1);
      }
      return tmp;
  }

  template <class T>
  inline void deallocate(T* buffer) {
      ::operator delete(buffer);
  }

  template <class T>
  class allocator {
  public:
      typedef T value_type;
      typedef T* pointer;
      typedef const T* const_pointer;
      typedef T& reference;
      typedef const T& const_reference;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      pointer allocate(size_type n) { 
        return ::allocate((difference_type)n, (pointer)0);
      }
      void deallocate(pointer p) { ::deallocate(p); }
      pointer address(reference x) { return (pointer)&x; }
      const_pointer const_address(const_reference x) { 
        return (const_pointer)&x; 
      }
      size_type init_page_size() { 
        return max(size_type(1), size_type(4096/sizeof(T))); 
      }
      size_type max_size() const { 
        return max(size_type(1), size_type(UINT_MAX/sizeof(T))); 
      }
  };

  class allocator<void> {
  public:
      typedef void* pointer;
  };
  ```
  </details>