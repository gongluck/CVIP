## 一、算法与设计模式专栏

### 1.查找与排序/KMP算法，栈/队列

![排序比较](./images/排序比较.png?raw=true)

#### 1.1 希尔排序

- 希尔排序，也称递减增量排序算法，是插入排序的一种更高效的改进版本。但希尔排序是非稳定排序算法。

  希尔排序是基于插入排序的以下两点性质而提出改进方法的：

  - 插入排序在对几乎已经排好序的数据操作时，效率高，即可以达到线性排序的效率；
  - 但插入排序一般来说是低效的，因为插入排序每次只能将数据移动一位；

  希尔排序的基本思想是：先将整个待排序的记录序列分割成为若干子序列分别进行直接插入排序，待整个序列中的记录"基本有序"时，再对全体记录进行依次直接插入排序。

- 算法步骤
  - 选择一个增量序列 t1，t2，……，tk，其中 ti > tj, tk = 1；
  - 按增量序列个数 k，对序列进行 k 趟排序；
  - 每趟排序，根据对应的增量 ti，将待排序列分割成若干长度为 m 的子序列，分别对各子表进行直接插入排序。仅增量因子为 1 时，整个序列作为一个表来处理，表长度即为整个序列的长度。

- [代码](./code/sort/sort.c)

  <details>
  <summary>希尔排序</summary>

  ```C
  int shell_sort(int *data, int length)
  {
      int gap = 0;
      int i = 0, j = 0;
      int temp = 0;
  
      // 外层分组
      for (gap = length / 2; gap >= 1; gap /= 2)
      {
          // 插入排序
          for (i = gap; i < length; ++i)
          {
              temp = data[i];
              for (j = i - gap; j >= 0 && temp < data[j]; j -= gap)
              {
                  // 将data[j]右移
                  data[j + gap] = data[j];
              }
              // 将temp(开始的data[i])放到循环跳出的地方
              data[j + gap] = temp;
          }
      }
      return 0;
  }
  ```
  </details>
  
#### 1.2 归并排序

- 归并排序（Merge sort）是建立在归并操作上的一种有效的排序算法。该算法是采用分治法（Divide and Conquer）的一个非常典型的应用。

- 作为一种典型的分而治之思想的算法应用，归并排序的实现由两种方法：

  - 自上而下的递归（所有递归的方法都可以用迭代重写，所以就有了第 2 种方法）；

  - 自下而上的迭代；
  
- 和选择排序一样，归并排序的性能不受输入数据的影响，但表现比选择排序好的多，因为始终都是 O(nlogn) 的时间复杂度。代价是需要额外的内存空间。

- 算法步骤
  - 申请空间，使其大小为两个已经排序序列之和，该空间用来存放合并后的序列；
  - 设定两个指针，最初位置分别为两个已经排序序列的起始位置；
  - 比较两个指针所指向的元素，选择相对小的元素放入到合并空间，并移动指针到下一位置；
  - 重复步骤 3 直到某一指针达到序列尾；
  - 将另一序列剩下的所有元素直接复制到合并序列尾。

- [代码](./code/sort/sort.c)

  <details>
  <summary>归并排序</summary>

  ```C
  void merge(int *data, int *temp, int start, int middle, int end)
  {
      int i = start, j = middle + 1, k = start;
      while (i <= middle && j <= end)
      {
          if (data[i] > data[j])
          {
              temp[k++] = data[j++];
          }
          else
          {
              temp[k++] = data[i++];
          }
      }
  
      while (i <= middle)
      {
          temp[k++] = data[i++];
      }
      while (j <= end)
      {
          temp[k++] = data[j++];
      }
      
      for (i = start; i <= end; i++)
      {
          data[i] = temp[i];
      }
  }
  
  int merge_sort(int *data, int *temp, int start, int end)
  {
      int middle;
      if (start < end)
      {
          middle = start + (end - start) / 2;
          merge_sort(data, temp, start, middle);
          merge_sort(data, temp, middle + 1, end);
          merge(data, temp, start, middle, end);
      }
  }
  ```
  </details>

#### 1.3 快速排序

- 快速排序是由东尼·霍尔所发展的一种排序算法。在平均状况下，排序 n 个项目要 Ο(nlogn) 次比较。在最坏状况下则需要 Ο(n2) 次比较，但这种状况并不常见。事实上，快速排序通常明显比其他 Ο(nlogn) 算法更快，因为它的内部循环（inner loop）可以在大部分的架构上很有效率地被实现出来。
- 快速排序使用分治法（Divide and conquer）策略来把一个串行（list）分为两个子串行（sub-lists）。
- 快速排序又是一种分而治之思想在排序算法上的典型应用。本质上来看，快速排序应该算是在冒泡排序基础上的递归分治法。
- 快速排序的名字起的是简单粗暴，因为一听到这个名字你就知道它存在的意义，就是快，而且效率高！它是处理大数据最快的排序算法之一了。虽然 Worst Case 的时间复杂度达到了 O(n²)，但是人家就是优秀，在大多数情况下都比平均时间复杂度为 O(n logn) 的排序算法表现要更好。
- 算法步骤
  - 从数列中挑出一个元素，称为 "基准"（pivot）;
  - 重新排序数列，所有元素比基准值小的摆放在基准前面，所有元素比基准值大的摆在基准的后面（相同的数可以到任一边）。在这个分区退出之后，该基准就处于数列的中间位置。这个称为分区（partition）操作；
  - 递归地（recursive）把小于基准值元素的子数列和大于基准值元素的子数列排序；

- [代码](./code/sort/sort.c)

  <details>
  <summary>快速排序</summary>
  
  ```C
  void quick(int *data, int left, int right)
  {
      if (left >= right)
          return;
  
      int i = left;
      int j = right;
      // 哨兵
      int key = data[left];
      while (i < j)
      {
          while (i < j && key <= data[j])
          {
              j--;
          }
          data[i] = data[j];
  
          while (i < j && key >= data[i])
          {
              i++;
          }
          data[j] = data[i];
      }
  
      data[i] = key;
      quick(data, left, i - 1);
      quick(data, i + 1, right);
  }
  
  int quick_sort(int *data, int length)
  {
      quick(data, 0, length - 1);
  }
  ```
  </details>

#### 1.4 KMP算法

- **Knuth-Morris-Pratt字符串查找算法**（简称为**KMP算法**）可在一个**主文本字符串**`S`内查找一个**词**`W`的出现位置。此算法通过运用对这个词在不匹配时本身就包含足够的信息来确定下一个匹配将在哪里开始的发现，从而避免重新检查先前匹配的字符。

- **部分匹配表**，又称为**失配函数**，作用是让算法无需多次匹配`S`中的任何字符。能够实现线性时间搜索的关键是在主串的一些字段中检查模式串的*初始字段*，我们可以确切地知道在当前位置之前的一个潜在匹配的位置。换句话说，在不错过任何潜在匹配的情况下，我们"预搜索"这个模式串本身并将其译成一个包含所有可能失配的位置对应可以绕过最多无效字符的列表。

- 对于`W`中的任何位置，我们都希望能够查询那个位置前（不包括那个位置）有可能的`W`的最长初始字段的长度，而不是从`W[0]`开始失配的整个字段，这长度就是我们查找下一个匹配时回退的距离。因此`T[i]`是`W`的可能的*适当*初始字段同时也是结束于`W[i - 1]`的子串的最大长度。我们使空串长度是0。当一个失配出现在模式串的最开始，这是特殊情况（无法回退），我们设置`T[0] = -1`。

- [代码](./code/search/kmp.c)

  <details>
  <summary>KMP算法</summary>
  
  ```C
  void make_next(const char *pattern, int *next)
  {
      int q, k;// k前缀下标，q后缀下标
      int m = strlen(pattern);
  
      next[0] = 0;
      for (q = 1, k = 0; q < m; q++)
      {
          while (k > 0 && pattern[q] != pattern[k])
              k = next[k - 1];
  
          if (pattern[q] == pattern[k])
          {
              k++;
          }
  
          next[q] = k;
      }
  
      // abcabc
      // next[0] = 0;
      // q=1, k=0, pattern[q]:pattern[k] = b:a, next[1] = 0;
      // q=2, k=0, pattern[q]:pattern[k] = c:a, next[2] = 0;
      // q=3, k=0, pattern[q]:pattern[k] = a:a, k++, next[3] = 1;
      // q=4, k=1, pattern[q]:pattern[k] = b:b, k++, next[4] = 2;
      // q=5, k=2, pattern[q]:pattern[k] = c:c, k++, next[5] = 3;
      // q=6, k=3, pattern[q]:pattern[k] = d:a, k=next[k-1] -> k=0; next[6] = 0;
  }
  
  int kmp(const char *text, const char *pattern, int *next)
  {
      int n = strlen(text);
      int m = strlen(pattern);
      make_next(pattern, next);
  
      int i, q;
      for (i = 0, q = 0; i < n; i++)
      {
          while (q > 0 && pattern[q] != text[i])
          {
              q = next[q - 1];
          }
  
          if (pattern[q] == text[i])
          {
              q++;
          }
  
          if (q == m)
          {
              break;
          }
      }
  
      return i - q + 1;
  }
  ```
  </details>

### 2.二叉树与红黑树

#### 2.1 二叉树

- **二叉树**是n(n>=0)个结点的有限集合，该集合或者为空集（称为空二叉树），或者由一个根结点和两棵互不相交的、分别称为根结点的左子树和右子树组成。
- 二叉树特点
  - 每个结点最多有两颗子树，所以二叉树中不存在度大于2的结点。
  - 左子树和右子树是有顺序的，次序不能任意颠倒。
  - 即使树中某结点只有一棵子树，也要区分它是左子树还是右子树。

- [代码](./code/tree/btree.c)

  <details>
  <summary>二叉树</summary>

  ```C
  typedef int KEY_VALUE;
  
  #define BSTREE_ENTRY(name, type) \
      struct name                  \
      {                            \
          struct type *left;       \
          struct type *right;      \
      }
  
  struct bstree_node
  {
      KEY_VALUE data;
      BSTREE_ENTRY(, bstree_node)
      bst;
  };
  
  struct bstree
  {
      struct bstree_node *root;
  };
  
  struct bstree_node *bstree_create_node(KEY_VALUE key)
  {
      struct bstree_node *node = (struct bstree_node *)malloc(sizeof(struct bstree_node));
      if (node == NULL)
      {
          assert(0);
      }
      node->data = key;
      node->bst.left = node->bst.right = NULL;
  
      return node;
  }
  
  int bstree_insert(struct bstree *T, int key)
  {
      assert(T != NULL);
      if (T->root == NULL)
      {
          T->root = bstree_create_node(key);
          return 0;
      }
  
      struct bstree_node *node = T->root;
      struct bstree_node *tmp = T->root;//保存父节点
      while (node != NULL)
      {
          tmp = node;
          if (key < node->data)
          {
              node = node->bst.left;
          }
          else
          {
              node = node->bst.right;
          }
      }
  
      if (key < tmp->data)
      {
          tmp->bst.left = bstree_create_node(key);
      }
      else
      {
          tmp->bst.right = bstree_create_node(key);
      }
  
      return 0;
  }
  
  int bstree_traversal(struct bstree_node *node)
  {
      if (node == NULL)
          return 0;
  
      bstree_traversal(node->bst.left);
      printf("%4d ", node->data);
      bstree_traversal(node->bst.right);
  }
  ```
  </details>

#### 2.2 红黑树

![红黑树](./images/红黑树.jpg?raw=true)

-  R-B Tree，全称是Red-Black Tree，又称为“红黑树”，它一种特殊的二叉查找树。红黑树的每个节点上都有存储位表示节点的颜色，可以是红(Red)或黑(Black)。
- 红黑树的特性
  - 每个节点或者是黑色，或者是红色。
  - 根节点是黑色。
  - 每个叶子节点（NIL）是黑色。这里叶子节点，是指为空(NIL或NULL)的叶子节点！
  - 如果一个节点是红色的，则它的子节点必须是黑色的。
  - 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。

- [代码](./code/tree/rbtree.c)

  <details>
  <summary>红黑树</summary>

  ```C
  #define RED 1
  #define BLACK 2
  
  typedef int KEY_TYPE;
  
  typedef struct _rbtree_node
  {
      unsigned char color;
      struct _rbtree_node *right;
      struct _rbtree_node *left;
      struct _rbtree_node *parent;
      KEY_TYPE key;
      void *value;
  } rbtree_node;
  
  typedef struct _rbtree
  {
      rbtree_node *root;
      rbtree_node *nil;
  } rbtree;
  
  rbtree_node *rbtree_mini(rbtree *T, rbtree_node *x)
  {
      while (x->left != T->nil)
      {
          x = x->left;
      }
      return x;
  }
  
  rbtree_node *rbtree_maxi(rbtree *T, rbtree_node *x)
  {
      while (x->right != T->nil)
      {
          x = x->right;
      }
      return x;
  }
  
  rbtree_node *rbtree_successor(rbtree *T, rbtree_node *x)
  {
      rbtree_node *y = x->parent;
  
      if (x->right != T->nil)
      {
          return rbtree_mini(T, x->right);
      }
  
      while ((y != T->nil) && (x == y->right))
      {
          x = y;
          y = y->parent;
      }
      return y;
  }
  
  void rbtree_left_rotate(rbtree *T, rbtree_node *x)
  {
      rbtree_node *y = x->right; // x  --> y  ,  y --> x,   right --> left,  left --> right
  
      x->right = y->left; //1 1
      if (y->left != T->nil)
      { //1 2
          y->left->parent = x;
      }
  
      y->parent = x->parent; //1 3
      if (x->parent == T->nil)
      { //1 4
          T->root = y;
      }
      else if (x == x->parent->left)
      {
          x->parent->left = y;
      }
      else
      {
          x->parent->right = y;
      }
  
      y->left = x;   //1 5
      x->parent = y; //1 6
  }
  
  void rbtree_right_rotate(rbtree *T, rbtree_node *y)
  {
      rbtree_node *x = y->left;
  
      y->left = x->right;
      if (x->right != T->nil)
      {
          x->right->parent = y;
      }
  
      x->parent = y->parent;
      if (y->parent == T->nil)
      {
          T->root = x;
      }
      else if (y == y->parent->right)
      {
          y->parent->right = x;
      }
      else
      {
          y->parent->left = x;
      }
  
      x->right = y;
      y->parent = x;
  }
  
  void rbtree_insert_fixup(rbtree *T, rbtree_node *z)
  {
      while (z->parent->color == RED)
      { //z ---> RED
          if (z->parent == z->parent->parent->left)
          {
              rbtree_node *y = z->parent->parent->right;
              if (y->color == RED)
              {
                  z->parent->color = BLACK;
                  y->color = BLACK;
                  z->parent->parent->color = RED;
  
                  z = z->parent->parent; //z --> RED
              }
              else
              {
                  if (z == z->parent->right)
                  {
                      z = z->parent;
                      rbtree_left_rotate(T, z);
                  }
  
                  z->parent->color = BLACK;
                  z->parent->parent->color = RED;
                  rbtree_right_rotate(T, z->parent->parent);
              }
          }
          else
          {
              rbtree_node *y = z->parent->parent->left;
              if (y->color == RED)
              {
                  z->parent->color = BLACK;
                  y->color = BLACK;
                  z->parent->parent->color = RED;
  
                  z = z->parent->parent; //z --> RED
              }
              else
              {
                  if (z == z->parent->left)
                  {
                      z = z->parent;
                      rbtree_right_rotate(T, z);
                  }
  
                  z->parent->color = BLACK;
                  z->parent->parent->color = RED;
                  rbtree_left_rotate(T, z->parent->parent);
              }
          }
      }
  
      T->root->color = BLACK;
  }
  
  void rbtree_insert(rbtree *T, rbtree_node *z)
  {
      rbtree_node *y = T->nil;
      rbtree_node *x = T->root;
  
      while (x != T->nil)
      {
          y = x;
          if (z->key < x->key)
          {
              x = x->left;
          }
          else if (z->key > x->key)
          {
              x = x->right;
          }
          else
          { //Exist
              return;
          }
      }
  
      z->parent = y;
      if (y == T->nil)
      {
          T->root = z;
      }
      else if (z->key < y->key)
      {
          y->left = z;
      }
      else
      {
          y->right = z;
      }
  
      z->left = T->nil;
      z->right = T->nil;
      z->color = RED;
  
      rbtree_insert_fixup(T, z);
  }
  
  void rbtree_delete_fixup(rbtree *T, rbtree_node *x)
  {
      while ((x != T->root) && (x->color == BLACK))
      {
          if (x == x->parent->left)
          {
              rbtree_node *w = x->parent->right;
              if (w->color == RED)
              {
                  w->color = BLACK;
                  x->parent->color = RED;
  
                  rbtree_left_rotate(T, x->parent);
                  w = x->parent->right;
              }
  
              if ((w->left->color == BLACK) && (w->right->color == BLACK))
              {
                  w->color = RED;
                  x = x->parent;
              }
              else
              {
                  if (w->right->color == BLACK)
                  {
                      w->left->color = BLACK;
                      w->color = RED;
                      rbtree_right_rotate(T, w);
                      w = x->parent->right;
                  }
  
                  w->color = x->parent->color;
                  x->parent->color = BLACK;
                  w->right->color = BLACK;
                  rbtree_left_rotate(T, x->parent);
  
                  x = T->root;
              }
          }
          else
          {
              rbtree_node *w = x->parent->left;
              if (w->color == RED)
              {
                  w->color = BLACK;
                  x->parent->color = RED;
                  rbtree_right_rotate(T, x->parent);
                  w = x->parent->left;
              }
  
              if ((w->left->color == BLACK) && (w->right->color == BLACK))
              {
                  w->color = RED;
                  x = x->parent;
              }
              else
              {
                  if (w->left->color == BLACK)
                  {
                      w->right->color = BLACK;
                      w->color = RED;
                      rbtree_left_rotate(T, w);
                      w = x->parent->left;
                  }
  
                  w->color = x->parent->color;
                  x->parent->color = BLACK;
                  w->left->color = BLACK;
                  rbtree_right_rotate(T, x->parent);
  
                  x = T->root;
              }
          }
      }
  
      x->color = BLACK;
  }
  
  rbtree_node *rbtree_delete(rbtree *T, rbtree_node *z)
  {
      rbtree_node *y = T->nil;
      rbtree_node *x = T->nil;
  
      if ((z->left == T->nil) || (z->right == T->nil))
      {
          y = z;
      }
      else
      {
          y = rbtree_successor(T, z);
      }
  
      if (y->left != T->nil)
      {
          x = y->left;
      }
      else if (y->right != T->nil)
      {
          x = y->right;
      }
  
      x->parent = y->parent;
      if (y->parent == T->nil)
      {
          T->root = x;
      }
      else if (y == y->parent->left)
      {
          y->parent->left = x;
      }
      else
      {
          y->parent->right = x;
      }
  
      if (y != z)
      {
          z->key = y->key;
          z->value = y->value;
      }
  
      if (y->color == BLACK)
      {
          rbtree_delete_fixup(T, x);
      }
  
      return y;
  }
  
  rbtree_node *rbtree_search(rbtree *T, KEY_TYPE key)
  {
      rbtree_node *node = T->root;
      while (node != T->nil)
      {
          if (key < node->key)
          {
              node = node->left;
          }
          else if (key > node->key)
          {
              node = node->right;
          }
          else
          {
              return node;
          }
      }
      return T->nil;
  }
  
  void rbtree_traversal(rbtree *T, rbtree_node *node)
  {
      if (node != T->nil)
      {
          rbtree_traversal(T, node->left);
          printf("key:%d, color:%d\n", node->key, node->color);
          rbtree_traversal(T, node->right);
      }
  }
  ```
  </details>

### 3.B-树

![B树](./images/B树.png?raw=true)

- B树也称B-树，它是一颗多路平衡查找树。我们描述一颗B树时需要指定它的阶数，阶数表示了一个结点最多有多少个孩子结点，一般用字母m表示阶数。当m取2时，就是我们常见的二叉搜索树。
- 一颗m阶的B树定义
  - 每个结点最多有m-1个关键字。
  - 根结点最少可以只有1个关键字。
  - 非根结点至少有Math.ceil(m/2)-1个关键字。
  - 每个结点中的关键字都按照从小到大的顺序排列，每个关键字的左子树中的所有关键字都小于它，而右子树中的所有关键字都大于它。
  - 所有叶子结点都位于同一层，或者说根结点到每个叶子结点的长度都相同。

- [代码](./code/tree/b-tree.c)

  <details>
  <summary>B-树</summary>
  
  ```C
  #define DEGREE 3
  typedef int KEY_VALUE;
  
  typedef struct _btree_node
  {
      KEY_VALUE *keys;
      struct _btree_node **childrens;
      int num;
      int leaf;
  } btree_node;
  
  typedef struct _btree
  {
      btree_node *root;
      int t;
  } btree;
  
  btree_node *btree_create_node(int t, int leaf)
  {
      btree_node *node = (btree_node *)calloc(1, sizeof(btree_node));
      if (node == NULL)
          assert(0);
  
      node->leaf = leaf;
      node->keys = (KEY_VALUE *)calloc(1, (2 * t - 1) * sizeof(KEY_VALUE));
      node->childrens = (btree_node **)calloc(1, (2 * t) * sizeof(btree_node));
      node->num = 0;
  
      return node;
  }
  
  void btree_destroy_node(btree_node *node)
  {
      assert(node);
  
      free(node->childrens);
      free(node->keys);
      free(node);
  }
  
  void btree_create(btree *T, int t)
  {
      T->t = t;
  
      btree_node *x = btree_create_node(t, 1);
      T->root = x;
  }
  
  void btree_split_child(btree *T, btree_node *x, int i)
  {
      int t = T->t;
  
      btree_node *y = x->childrens[i];
      btree_node *z = btree_create_node(t, y->leaf);
  
      z->num = t - 1;
  
      int j = 0;
      for (j = 0; j < t - 1; j++)
      {
          z->keys[j] = y->keys[j + t];
      }
      if (y->leaf == 0)
      {
          for (j = 0; j < t; j++)
          {
              z->childrens[j] = y->childrens[j + t];
          }
      }
  
      y->num = t - 1;
      for (j = x->num; j >= i + 1; j--)
      {
          x->childrens[j + 1] = x->childrens[j];
      }
  
      x->childrens[i + 1] = z;
  
      for (j = x->num - 1; j >= i; j--)
      {
          x->keys[j + 1] = x->keys[j];
      }
      x->keys[i] = y->keys[t - 1];
      x->num += 1;
  }
  
  void btree_insert_nonfull(btree *T, btree_node *x, KEY_VALUE k)
  {
      int i = x->num - 1;
  
      if (x->leaf == 1)
      {
          while (i >= 0 && x->keys[i] > k)
          {
              x->keys[i + 1] = x->keys[i];
              i--;
          }
          x->keys[i + 1] = k;
          x->num += 1;
      }
      else
      {
          while (i >= 0 && x->keys[i] > k)
              i--;
  
          if (x->childrens[i + 1]->num == (2 * (T->t)) - 1)
          {
              btree_split_child(T, x, i + 1);
              if (k > x->keys[i + 1])
                  i++;
          }
  
          btree_insert_nonfull(T, x->childrens[i + 1], k);
      }
  }
  
  void btree_insert(btree *T, KEY_VALUE key)
  {
      //int t = T->t;
      btree_node *r = T->root;
      if (r->num == 2 * T->t - 1)
      {
          btree_node *node = btree_create_node(T->t, 0);
          T->root = node;
  
          node->childrens[0] = r;
  
          btree_split_child(T, node, 0);
  
          int i = 0;
          if (node->keys[0] < key)
              i++;
          btree_insert_nonfull(T, node->childrens[i], key);
      }
      else
      {
          btree_insert_nonfull(T, r, key);
      }
  }
  
  void btree_traverse(btree_node *x)
  {
      int i = 0;
      for (i = 0; i < x->num; i++)
      {
          if (x->leaf == 0)
              btree_traverse(x->childrens[i]);
          printf("%C ", x->keys[i]);
      }
  
      if (x->leaf == 0)
          btree_traverse(x->childrens[i]);
  }
  
  void btree_print(btree *T, btree_node *node, int layer)
  {
      btree_node *p = node;
      int i;
      if (p)
      {
          printf("\nlayer = %d keynum = %d is_leaf = %d\n", layer, p->num, p->leaf);
          for (i = 0; i < node->num; i++)
              printf("%c ", p->keys[i]);
          printf("\n");
  
          layer++;
          for (i = 0; i <= p->num; i++)
              if (p->childrens[i])
                  btree_print(T, p->childrens[i], layer);
      }
      else
          printf("the tree is empty\n");
  }
  
  int btree_bin_search(btree_node *node, int low, int high, KEY_VALUE key)
  {
      int mid;
      if (low > high || low < 0 || high < 0)
      {
          return -1;
      }
  
      while (low <= high)
      {
          mid = (low + high) / 2;
          if (key > node->keys[mid])
          {
              low = mid + 1;
          }
          else
          {
              high = mid - 1;
          }
      }
  
      return low;
  }
  
  //{child[idx], key[idx], child[idx+1]}
  void btree_merge(btree *T, btree_node *node, int idx)
  {
      btree_node *left = node->childrens[idx];
      btree_node *right = node->childrens[idx + 1];
  
      int i = 0;
  
      /////data merge
      left->keys[T->t - 1] = node->keys[idx];
      for (i = 0; i < T->t - 1; i++)
      {
          left->keys[T->t + i] = right->keys[i];
      }
      if (!left->leaf)
      {
          for (i = 0; i < T->t; i++)
          {
              left->childrens[T->t + i] = right->childrens[i];
          }
      }
      left->num += T->t;
  
      //destroy right
      btree_destroy_node(right);
  
      //node
      for (i = idx + 1; i < node->num; i++)
      {
          node->keys[i - 1] = node->keys[i];
          node->childrens[i] = node->childrens[i + 1];
      }
      node->childrens[i + 1] = NULL;
      node->num -= 1;
  
      if (node->num == 0)
      {
          T->root = left;
          btree_destroy_node(node);
      }
  }
  
  void btree_delete_key(btree *T, btree_node *node, KEY_VALUE key)
  {
      if (node == NULL)
          return;
  
      int idx = 0, i;
  
      while (idx < node->num && key > node->keys[idx])
      {
          idx++;
      }
  
      if (idx < node->num && key == node->keys[idx])
      {
          if (node->leaf)
          {
              for (i = idx; i < node->num - 1; i++)
              {
                  node->keys[i] = node->keys[i + 1];
              }
  
              node->keys[node->num - 1] = 0;
              node->num--;
  
              if (node->num == 0)
              { //root
                  free(node);
                  T->root = NULL;
              }
  
              return;
          }
          else if (node->childrens[idx]->num >= T->t)
          {
              btree_node *left = node->childrens[idx];
              node->keys[idx] = left->keys[left->num - 1];
  
              btree_delete_key(T, left, left->keys[left->num - 1]);
          }
          else if (node->childrens[idx + 1]->num >= T->t)
          {
              btree_node *right = node->childrens[idx + 1];
              node->keys[idx] = right->keys[0];
  
              btree_delete_key(T, right, right->keys[0]);
          }
          else
          {
              btree_merge(T, node, idx);
              btree_delete_key(T, node->childrens[idx], key);
          }
      }
      else
      {
          btree_node *child = node->childrens[idx];
          if (child == NULL)
          {
              printf("Cannot del key = %d\n", key);
              return;
          }
  
          if (child->num == T->t - 1)
          {
              btree_node *left = NULL;
              btree_node *right = NULL;
              if (idx - 1 >= 0)
                  left = node->childrens[idx - 1];
              if (idx + 1 <= node->num)
                  right = node->childrens[idx + 1];
  
              if ((left && left->num >= T->t) ||
                  (right && right->num >= T->t))
              {
                  int richR = 0;
                  if (right)
                      richR = 1;
                  if (left && right)
                      richR = (right->num > left->num) ? 1 : 0;
  
                  if (right && right->num >= T->t && richR)
                  { //borrow from next
                      child->keys[child->num] = node->keys[idx];
                      child->childrens[child->num + 1] = right->childrens[0];
                      child->num++;
  
                      node->keys[idx] = right->keys[0];
                      for (i = 0; i < right->num - 1; i++)
                      {
                          right->keys[i] = right->keys[i + 1];
                          right->childrens[i] = right->childrens[i + 1];
                      }
  
                      right->keys[right->num - 1] = 0;
                      right->childrens[right->num - 1] = right->childrens[right->num];
                      right->childrens[right->num] = NULL;
                      right->num--;
                  }
                  else
                  { //borrow from prev
  
                      for (i = child->num; i > 0; i--)
                      {
                          child->keys[i] = child->keys[i - 1];
                          child->childrens[i + 1] = child->childrens[i];
                      }
  
                      child->childrens[1] = child->childrens[0];
                      child->childrens[0] = left->childrens[left->num];
                      child->keys[0] = node->keys[idx - 1];
  
                      child->num++;
  
                      node->keys[idx - 1] = left->keys[left->num - 1];
                      left->keys[left->num - 1] = 0;
                      left->childrens[left->num] = NULL;
                      left->num--;
                  }
              }
              else if ((!left || (left->num == T->t - 1)) && (!right || (right->num == T->t - 1)))
              {
                  if (left && left->num == T->t - 1)
                  {
                      btree_merge(T, node, idx - 1);
                      child = left;
                  }
                  else if (right && right->num == T->t - 1)
                  {
                      btree_merge(T, node, idx);
                  }
              }
          }
  
          btree_delete_key(T, child, key);
      }
  }
  
  int btree_delete(btree *T, KEY_VALUE key)
  {
      if (!T->root)
          return -1;
  
      btree_delete_key(T, T->root, key);
      return 0;
  }
  ```
  </details>

### 4.布隆过滤器

![布隆过滤器](./images/布隆过滤器.png?raw=true)

- bloom算法类似一个hash set，用来判断某个元素（key）是否在某个集合中。和一般的hash set不同的是，这个算法无需存储key的值，对于每个key，只需要k个比特位，每个存储一个标志，用来判断key是否在集合中。
- 算法：

  - 首先需要k个hash函数，每个函数可以把key散列成为1个整数

  - 初始化时，需要一个长度为n比特的数组，每个比特位初始化为0

  - 某个key加入集合时，用k个hash函数计算出k个散列值，并把数组中对应的比特位置为1

  - 判断某个key是否在集合时，用k个hash函数计算出k个散列值，并查询数组中对应的比特位，如果所有的比特位都是1，认为在集合中。
- 优点：不需要存储key，节省空间
- 缺点：
  - 算法判断key在集合中时，有一定的概率key其实不在集合中
  - 无法删除
- 典型的应用场景：
  某些存储系统的设计中，会存在空查询缺陷：当查询一个不存在的key时，需要访问慢设备，导致效率低下。比如一个前端页面的缓存系统，可能这样设计：先查询某个页面在本地是否存在，如果存在就直接返回，如果不存在，就从后端获取。但是当频繁从缓存系统查询一个页面时，缓存系统将会频繁请求后端，把压力导入后端。这时只要增加一个bloom算法的服务，后端插入一个key时，在这个服务中设置一次
  需要查询后端时，先判断key在后端是否存在，这样就能避免后端的压力。

- [代码](./code/bloomfilter/bloomfilter.h)

  <details>
  <summary>布隆过滤器</summary>
  
  ```C
  /**
   *  BloomFilter使用例子：
   *  static BaseBloomFilter stBloomFilter = {0};
   *
   *  初始化BloomFilter(最大100000元素，不超过0.00001的错误率)：
   *      InitBloomFilter(&stBloomFilter, 0, 100000, 0.00001);
   *  重置BloomFilter：
   *      ResetBloomFilter(&stBloomFilter);
   *  释放BloomFilter:
   *      FreeBloomFilter(&stBloomFilter);
   *
   *  向BloomFilter中新增一个数值（0-正常，1-加入数值过多）：
   *      uint32_t dwValue;
   *      iRet = BloomFilter_Add(&stBloomFilter, &dwValue, sizeof(uint32_t));
   *  检查数值是否在BloomFilter内（0-存在，1-不存在）：
   *      iRet = BloomFilter_Check(&stBloomFilter, &dwValue, sizeof(uint32_t));
   *
   *  (1.1新增) 将生成好的BloomFilter写入文件:
   *      iRet = SaveBloomFilterToFile(&stBloomFilter, "dump.bin")
   *  (1.1新增) 从文件读取生成好的BloomFilter:
   *      iRet = LoadBloomFilterFromFile(&stBloomFilter, "dump.bin")
  **/
  
  // 注意，要让Add/Check函数内联，必须使用 -O2 或以上的优化等级
  #define FORCE_INLINE __attribute__((always_inline))
  
  #define BYTE_BITS (8)
  #define MIX_UINT64(v) ((uint32_t)((v >> 32) ^ (v)))
  
  #define SETBIT(filter, n) (filter->pstFilter[n / BYTE_BITS] |= (1 << (n % BYTE_BITS)))
  #define GETBIT(filter, n) (filter->pstFilter[n / BYTE_BITS] & (1 << (n % BYTE_BITS)))
  
  #pragma pack(1)
  
  // BloomFilter结构定义
  typedef struct
  {
      uint8_t cInitFlag; // 初始化标志，为0时的第一次Add()会对stFilter[]做初始化
      uint8_t cResv[3];
  
      uint32_t dwMaxItems;   // n - BloomFilter中最大元素个数 (输入量)
      double dProbFalse;     // p - 假阳概率(误判率) (输入量，比如万分之一：0.00001)
      uint32_t dwFilterBits; // m =  ; - BloomFilter的比特数
      uint32_t dwHashFuncs;  // k = round(log(2.0) * m / n); - 哈希函数个数
  
      uint32_t dwSeed;  // MurmurHash的种子偏移量
      uint32_t dwCount; // Add()的计数，超过MAX_BLOOMFILTER_N则返回失败
  
      uint32_t dwFilterSize;    // dwFilterBits / BYTE_BITS
      unsigned char *pstFilter; // BloomFilter存储指针，使用malloc分配
      uint32_t *pdwHashPos;     // 存储上次hash得到的K个bit位置数组(由bloom_hash填充)
  } BaseBloomFilter;
  
  // BloomFilter文件头部定义
  typedef struct
  {
      uint32_t dwMagicCode; // 文件头部标识，填充 __MGAIC_CODE__
      uint32_t dwSeed;
      uint32_t dwCount;
  
      uint32_t dwMaxItems;   // n - BloomFilter中最大元素个数 (输入量)
      double dProbFalse;     // p - 假阳概率 (输入量，比如万分之一：0.00001)
      uint32_t dwFilterBits; // m = ceil((n * log(p)) / log(1.0 / (pow(2.0, log(2.0))))); - BloomFilter的比特数
      uint32_t dwHashFuncs;  // k = round(log(2.0) * m / n); - 哈希函数个数
  
      uint32_t dwResv[6];
      uint32_t dwFileCrc;    // (未使用)整个文件的校验和
      uint32_t dwFilterSize; // 后面Filter的Buffer长度
  } BloomFileHead;
  
  #pragma pack()
  
  // 计算BloomFilter的参数m,k
  static inline void _CalcBloomFilterParam(uint32_t n, double p, uint32_t *pm, uint32_t *pk)
  {
      /**
       *  n - Number of items in the filter
       *  p - Probability of false positives, float between 0 and 1 or a number indicating 1-in-p
       *  m - Number of bits in the filter
       *  k - Number of hash functions
       *
       *  f = ln(2) × ln(1/2) × m / n = (0.6185) ^ (m/n)
       *  m = -1 * ln(p) × n / 0.6185 , 这里有错误
       *  k = ln(2) × m / n = 0.6931 * m / n
       * darren修正：
       * m = -1*n*ln(p)/((ln(2))^2) = -1*n*ln(p)/(ln(2)*ln(2)) = -1*n*ln(p)/(0.69314718055995*0.69314718055995))
       *   = -1*n*ln(p)/0.4804530139182079271955440025
       * k = ln(2)*m/n
      **/
  
      uint32_t m, k, m2;
  
      //    printf("ln(2):%lf, ln(p):%lf\n", log(2), log(p)); // 用来验证函数正确性
  
      // 计算指定假阳(误差)概率下需要的比特数
      m = (uint32_t)ceil(-1.0 * n * log(p) / 0.480453); //darren 修正
  
      m = (m - m % 64) + 64; // 8字节对齐
                             //    m2 =(uint32_t) ceil(-1 * n * log(p) / 0.480453); //错误写法
      // 计算哈希函数个数
      double double_k = (0.69314 * m / n); // ln(2)*m/n // 这里只是为了debug出来看看具体的浮点数值
      k = round(double_k);                 // 返回x的四舍五入整数值。
      printf("orig_k:%lf, k:%u\n", double_k, k);
  
      *pm = m;
      *pk = k;
      return;
  }
  
  // 根据目标精度和数据个数，初始化BloomFilter结构
  /**
   * @brief 初始化布隆过滤器
   * @param pstBloomfilter 布隆过滤器实例
   * @param dwSeed    hash种子
   * @param dwMaxItems 存储容量
   * @param dProbFalse 允许的误判率
   * @return 返回值
   *      -1 传入的布隆过滤器为空
   *      -2 hash种子错误或误差>=1
   */
  inline int InitBloomFilter(BaseBloomFilter *pstBloomfilter,
                             uint32_t dwSeed,
                             uint32_t dwMaxItems, double dProbFalse)
  {
      if (pstBloomfilter == NULL)
          return -1;
      if ((dProbFalse <= 0) || (dProbFalse >= 1))
          return -2;
  
      // 先检查是否重复Init，释放内存
      if (pstBloomfilter->pstFilter != NULL)
          free(pstBloomfilter->pstFilter);
      if (pstBloomfilter->pdwHashPos != NULL)
          free(pstBloomfilter->pdwHashPos);
  
      memset(pstBloomfilter, 0, sizeof(BaseBloomFilter));
  
      // 初始化内存结构，并计算BloomFilter需要的空间
      pstBloomfilter->dwMaxItems = dwMaxItems; // 最大存储
      pstBloomfilter->dProbFalse = dProbFalse; // 误差
      pstBloomfilter->dwSeed = dwSeed;         // hash种子
  
      // 计算 m, k
      _CalcBloomFilterParam(pstBloomfilter->dwMaxItems, pstBloomfilter->dProbFalse,
                            &pstBloomfilter->dwFilterBits, &pstBloomfilter->dwHashFuncs);
  
      // 分配BloomFilter的存储空间
      pstBloomfilter->dwFilterSize = pstBloomfilter->dwFilterBits / BYTE_BITS;
      pstBloomfilter->pstFilter = (unsigned char *)malloc(pstBloomfilter->dwFilterSize);
      if (NULL == pstBloomfilter->pstFilter)
          return -100;
  
      // 哈希结果数组，每个哈希函数一个
      pstBloomfilter->pdwHashPos = (uint32_t *)malloc(pstBloomfilter->dwHashFuncs * sizeof(uint32_t));
      if (NULL == pstBloomfilter->pdwHashPos)
          return -200;
  
      printf(">>> Init BloomFilter(n=%u, p=%e, m=%u, k=%d), malloc() size=%.6fMB, items:bits=1:%0.1lf\n",
             pstBloomfilter->dwMaxItems, pstBloomfilter->dProbFalse, pstBloomfilter->dwFilterBits,
             pstBloomfilter->dwHashFuncs, (double)pstBloomfilter->dwFilterSize / 1024 / 1024,
             pstBloomfilter->dwFilterBits * 1.0 / pstBloomfilter->dwMaxItems);
  
      // 初始化BloomFilter的内存
      memset(pstBloomfilter->pstFilter, 0, pstBloomfilter->dwFilterSize);
      pstBloomfilter->cInitFlag = 1;
      return 0;
  }
  
  // 释放BloomFilter
  inline int FreeBloomFilter(BaseBloomFilter *pstBloomfilter)
  {
      if (pstBloomfilter == NULL)
          return -1;
  
      pstBloomfilter->cInitFlag = 0;
      pstBloomfilter->dwCount = 0;
  
      free(pstBloomfilter->pstFilter);
      pstBloomfilter->pstFilter = NULL;
      free(pstBloomfilter->pdwHashPos);
      pstBloomfilter->pdwHashPos = NULL;
      return 0;
  }
  
  // 重置BloomFilter
  // 注意: Reset()函数不会立即初始化stFilter，而是当一次Add()时去memset
  inline int ResetBloomFilter(BaseBloomFilter *pstBloomfilter)
  {
      if (pstBloomfilter == NULL)
          return -1;
  
      pstBloomfilter->cInitFlag = 0;
      pstBloomfilter->dwCount = 0;
      return 0;
  }
  
  // 和ResetBloomFilter不同，调用后立即memset内存
  inline int RealResetBloomFilter(BaseBloomFilter *pstBloomfilter)
  {
      if (pstBloomfilter == NULL)
          return -1;
  
      memset(pstBloomfilter->pstFilter, 0, pstBloomfilter->dwFilterSize);
      pstBloomfilter->cInitFlag = 1;
      pstBloomfilter->dwCount = 0;
      return 0;
  }
  
  ///
  ///  函数FORCE_INLINE，加速执行
  ///
  // MurmurHash2, 64-bit versions, by Austin Appleby
  // https://sites.google.com/site/murmurhash/
  FORCE_INLINE uint64_t MurmurHash2_x64(const void *key, int len, uint32_t seed)
  {
      const uint64_t m = 0xc6a4a7935bd1e995;
      const int r = 47;
  
      uint64_t h = seed ^ (len * m);
  
      const uint64_t *data = (const uint64_t *)key;
      const uint64_t *end = data + (len / 8);
  
      while (data != end)
      {
          uint64_t k = *data++;
  
          k *= m;
          k ^= k >> r;
          k *= m;
  
          h ^= k;
          h *= m;
      }
  
      const uint8_t *data2 = (const uint8_t *)data;
  
      switch (len & 7)
      {
      case 7:
          h ^= ((uint64_t)data2[6]) << 48;
      case 6:
          h ^= ((uint64_t)data2[5]) << 40;
      case 5:
          h ^= ((uint64_t)data2[4]) << 32;
      case 4:
          h ^= ((uint64_t)data2[3]) << 24;
      case 3:
          h ^= ((uint64_t)data2[2]) << 16;
      case 2:
          h ^= ((uint64_t)data2[1]) << 8;
      case 1:
          h ^= ((uint64_t)data2[0]);
          h *= m;
      };
  
      h ^= h >> r;
      h *= m;
      h ^= h >> r;
  
      return h;
  }
  
  // 双重散列封装，k个函数函数, 比如要20个
  FORCE_INLINE void bloom_hash(BaseBloomFilter *pstBloomfilter, const void *key, int len)
  {
      //if (pstBloomfilter == NULL) return;
      int i;
      uint32_t dwFilterBits = pstBloomfilter->dwFilterBits;
      uint64_t hash1 = MurmurHash2_x64(key, len, pstBloomfilter->dwSeed);
      uint64_t hash2 = MurmurHash2_x64(key, len, MIX_UINT64(hash1));
  
      for (i = 0; i < (int)pstBloomfilter->dwHashFuncs; i++)
      {
          // k0 = (hash1 + 0*hash2) % dwFilterBits; // dwFilterBits bit向量的长度
          // k1 = (hash1 + 1*hash2) % dwFilterBits;
          pstBloomfilter->pdwHashPos[i] = (hash1 + i * hash2) % dwFilterBits;
      }
  
      return;
  }
  
  // 向BloomFilter中新增一个元素
  // 成功返回0，当添加数据超过限制值时返回1提示用户
  FORCE_INLINE int BloomFilter_Add(BaseBloomFilter *pstBloomfilter, const void *key, int len)
  {
      if ((pstBloomfilter == NULL) || (key == NULL) || (len <= 0))
          return -1;
  
      int i;
  
      if (pstBloomfilter->cInitFlag != 1)
      {
          // Reset后没有初始化，使用前需要memset
          memset(pstBloomfilter->pstFilter, 0, pstBloomfilter->dwFilterSize);
          pstBloomfilter->cInitFlag = 1;
      }
  
      // hash key到bloomfilter中, 为了计算不同hash命中的位置，保存pdwHashPos数组
      bloom_hash(pstBloomfilter, key, len);
      for (i = 0; i < (int)pstBloomfilter->dwHashFuncs; i++)
      {
          // dwHashFuncs[0] = hash0(key)
          // dwHashFuncs[1] = hash1(key)
          // dwHashFuncs[k-1] = hashk-1(key)
          SETBIT(pstBloomfilter, pstBloomfilter->pdwHashPos[i]);
      }
  
      // 增加count数
      pstBloomfilter->dwCount++;
      if (pstBloomfilter->dwCount <= pstBloomfilter->dwMaxItems)
          return 0;
      else
          return 1; // 超过N最大值，可能出现准确率下降等情况
  }
  
  // 检查一个元素是否在bloomfilter中
  // 返回：0-存在，1-不存在，负数表示失败
  FORCE_INLINE int BloomFilter_Check(BaseBloomFilter *pstBloomfilter, const void *key, int len)
  {
      if ((pstBloomfilter == NULL) || (key == NULL) || (len <= 0))
          return -1;
  
      int i;
  
      bloom_hash(pstBloomfilter, key, len);
      for (i = 0; i < (int)pstBloomfilter->dwHashFuncs; i++)
      {
          // 如果有任意bit不为1，说明key不在bloomfilter中
          // 注意: GETBIT()返回不是0|1，高位可能出现128之类的情况
          if (GETBIT(pstBloomfilter, pstBloomfilter->pdwHashPos[i]) == 0)
              return 1;
      }
  
      return 0;
  }
  
  /* 文件相关封装 */
  // 将生成好的BloomFilter写入文件
  inline int SaveBloomFilterToFile(BaseBloomFilter *pstBloomfilter, char *szFileName)
  {
      if ((pstBloomfilter == NULL) || (szFileName == NULL))
          return -1;
  
      int iRet;
      FILE *pFile;
      static BloomFileHead stFileHeader = {0};
  
      pFile = fopen(szFileName, "wb");
      if (pFile == NULL)
      {
          perror("fopen");
          return -11;
      }
  
      // 先写入文件头
      stFileHeader.dwMagicCode = __MGAIC_CODE__;
      stFileHeader.dwSeed = pstBloomfilter->dwSeed;
      stFileHeader.dwCount = pstBloomfilter->dwCount;
      stFileHeader.dwMaxItems = pstBloomfilter->dwMaxItems;
      stFileHeader.dProbFalse = pstBloomfilter->dProbFalse;
      stFileHeader.dwFilterBits = pstBloomfilter->dwFilterBits;
      stFileHeader.dwHashFuncs = pstBloomfilter->dwHashFuncs;
      stFileHeader.dwFilterSize = pstBloomfilter->dwFilterSize;
  
      iRet = fwrite((const void *)&stFileHeader, sizeof(stFileHeader), 1, pFile);
      if (iRet != 1)
      {
          perror("fwrite(head)");
          return -21;
      }
  
      // 接着写入BloomFilter的内容
      iRet = fwrite(pstBloomfilter->pstFilter, 1, pstBloomfilter->dwFilterSize, pFile);
      if ((uint32_t)iRet != pstBloomfilter->dwFilterSize)
      {
          perror("fwrite(data)");
          return -31;
      }
  
      fclose(pFile);
      return 0;
  }
  
  // 从文件读取生成好的BloomFilter
  inline int LoadBloomFilterFromFile(BaseBloomFilter *pstBloomfilter, char *szFileName)
  {
      if ((pstBloomfilter == NULL) || (szFileName == NULL))
          return -1;
  
      int iRet;
      FILE *pFile;
      static BloomFileHead stFileHeader = {0};
  
      if (pstBloomfilter->pstFilter != NULL)
          free(pstBloomfilter->pstFilter);
      if (pstBloomfilter->pdwHashPos != NULL)
          free(pstBloomfilter->pdwHashPos);
  
      //
      pFile = fopen(szFileName, "rb");
      if (pFile == NULL)
      {
          perror("fopen");
          return -11;
      }
  
      // 读取并检查文件头
      iRet = fread((void *)&stFileHeader, sizeof(stFileHeader), 1, pFile);
      if (iRet != 1)
      {
          perror("fread(head)");
          return -21;
      }
  
      if ((stFileHeader.dwMagicCode != __MGAIC_CODE__) || (stFileHeader.dwFilterBits != stFileHeader.dwFilterSize * BYTE_BITS))
          return -50;
  
      // 初始化传入的 BaseBloomFilter 结构
      pstBloomfilter->dwMaxItems = stFileHeader.dwMaxItems;
      pstBloomfilter->dProbFalse = stFileHeader.dProbFalse;
      pstBloomfilter->dwFilterBits = stFileHeader.dwFilterBits;
      pstBloomfilter->dwHashFuncs = stFileHeader.dwHashFuncs;
      pstBloomfilter->dwSeed = stFileHeader.dwSeed;
      pstBloomfilter->dwCount = stFileHeader.dwCount;
      pstBloomfilter->dwFilterSize = stFileHeader.dwFilterSize;
  
      pstBloomfilter->pstFilter = (unsigned char *)malloc(pstBloomfilter->dwFilterSize);
      if (NULL == pstBloomfilter->pstFilter)
          return -100;
      pstBloomfilter->pdwHashPos = (uint32_t *)malloc(pstBloomfilter->dwHashFuncs * sizeof(uint32_t));
      if (NULL == pstBloomfilter->pdwHashPos)
          return -200;
  
      // 将后面的Data部分读入 pstFilter
      iRet = fread((void *)(pstBloomfilter->pstFilter), 1, pstBloomfilter->dwFilterSize, pFile);
      if ((uint32_t)iRet != pstBloomfilter->dwFilterSize)
      {
          perror("fread(data)");
          return -31;
      }
      pstBloomfilter->cInitFlag = 1;
  
      printf(">>> Load BloomFilter(n=%u, p=%f, m=%u, k=%d), malloc() size=%.2fMB\n",
             pstBloomfilter->dwMaxItems, pstBloomfilter->dProbFalse, pstBloomfilter->dwFilterBits,
             pstBloomfilter->dwHashFuncs, (double)pstBloomfilter->dwFilterSize / 1024 / 1024);
  
      fclose(pFile);
      return 0;
  }
  ```
  </details>

### 5.设计模式

#### 5.1 观察者模式

![观察者模式](./images/观察者模式.png?raw=true)

- 定义对象间一种一对多的依赖关系，使得每当一个对象改变状态，则所有依赖于它的对象都会得到通知并被自动更新。

- [代码](./code/designpattern/observer.cpp)

  <details>
  <summary>观察者模式</summary>
  
  ```C++
  // 简单变形示例——区别对待观察者
  /*
  1：范例需求
  这是一个实际系统的简化需求：在一个水质监测系统中有这样一个功能，当水中的杂质为正常的时候，只是通知监测人员做记录；
  当为轻度污染的时候，除了通知监测人员做记录外，还要通知预警人员，判断是否需要预警；当为中度或者高度污染的时候，
  除了通知监测人员做记录外，还要通知预警人员，判断是否需要预警，同时还要通知监测部门领导做相应的处理。
  */
  #include <iostream>
  #include <list>
  using namespace std;
  
  class WaterQualitySubject;
  
  // 观察者的接口
  /**
   * 水质观察者接口定义
   */
  class WatcherObserver
  {
  public:
      WatcherObserver() {}
      virtual ~WatcherObserver() {}
      /**
       * 被通知的方法
       * @param subject 传入被观察的目标对象
       */
      virtual void update(WaterQualitySubject *subject) = 0;
  
      // 和普通观察者模式， 增加了角色
      /**
       * 设置观察人员的职务
       * @param job 观察人员的职务
       */
      virtual void setJob(string job) = 0;
  
      /**
       * 获取观察人员的职务
       * @return 观察人员的职务
       */
      virtual string getJob() = 0;
  };
  
  /**
  * 定义水质监测的目标对象
  */
  class WaterQualitySubject
  {
  public:
      WaterQualitySubject() {}
      virtual ~WaterQualitySubject() {}
      /**
      * 注册观察者对象
       * @param observer 观察者对象
       */
      void attach(WatcherObserver *observer)
      {
          observers.push_back(observer);
      }
  
      /**
       * 删除观察者对象
       * @param observer 观察者对象
       */
      void detach(WatcherObserver *observer)
      {
          observers.remove(observer);
      }
  
      /**
       * 通知相应的观察者对象
       */
      virtual void notifyWatchers() = 0;
  
      /**
       * 获取水质污染的级别
       * @return 水质污染的级别
       */
      virtual int getPolluteLevel() = 0;
  
  protected:
      /**
       * 用来保存注册的观察者对象
       */
      list<WatcherObserver *> observers;
  };
  
  /**
   * 具体的观察者实现
   */
  class Watcher : public WatcherObserver
  {
  public:
      Watcher() {}
      virtual ~Watcher() {}
      string getJob()
      {
          return m_job;
      }
  
      void setJob(string job)
      {
          m_job = job;
      }
  
      virtual void update(WaterQualitySubject *subject)
      {
          //这里采用的是拉的方式
          cout << m_job << " 获取到通知，当前污染级别为：" << subject->getPolluteLevel() << endl;
      }
  
  private:
      /**
       * 职务
       */
      string m_job;
  };
  
  /**
   * 具体的水质监测对象
   */
  class WaterQuality : public WaterQualitySubject
  {
  public:
      WaterQuality() {}
      virtual ~WaterQuality() {}
      /**
       * 获取水质污染的级别
       * @return 水质污染的级别
       */
      int getPolluteLevel()
      {
          return m_polluteLevel;
      }
  
      /**
       * 当监测水质情况后，设置水质污染的级别
       * @param polluteLevel 水质污染的级别
       */
      virtual void setPolluteLevel(int polluteLevel)
      {
          m_polluteLevel = polluteLevel;
          //通知相应的观察者
          notifyWatchers();
      }
  
      /**
       * 通知相应的观察者对象
       */
  
      virtual void notifyWatchers()
      {
          //循环所有注册的观察者
          for (WatcherObserver *watcher : observers)
          {
              //开始根据污染级别判断是否需要通知，由这里总控
              if (m_polluteLevel >= 0)
              {
                  //通知监测员做记录
                  if (watcher->getJob().compare("监测人员") == 0)
                  {
                      watcher->update(this);
                  }
              }
  
              if (m_polluteLevel >= 1)
              {
                  //通知预警人员
                  if (watcher->getJob().compare("预警人员") == 0)
                  {
                      watcher->update(this);
                  }
              }
  
              if (m_polluteLevel >= 2)
              {
                  //通知监测部门领导
                  if (watcher->getJob().compare("监测部门领导") == 0)
                  {
                      watcher->update(this);
                  }
              }
          }
      }
  
  private:
      /**
       * 污染的级别，0表示正常，1表示轻度污染，2表示中度污染，3表示高度污染
       */
      int m_polluteLevel = 0;
  };
  
  int main()
  {
      //创建水质主题对象
      WaterQuality *subject = new WaterQuality();
      //创建几个观察者, 观察者分了不同角色
      WatcherObserver *watcher1 = new Watcher();
      watcher1->setJob("监测人员");
      WatcherObserver *watcher2 = new Watcher();
      watcher2->setJob("预警人员");
      WatcherObserver *watcher3 = new Watcher();
      watcher3->setJob("监测部门领导");
  
      //注册观察者
      subject->attach(watcher1);
      subject->attach(watcher2);
      subject->attach(watcher3);
  
      //填写水质报告
      cout << "当水质为正常的时候------------------〉" << endl;
      subject->setPolluteLevel(0);
  
      cout << "\n当水质为轻度污染的时候---------------〉" << endl;
      subject->setPolluteLevel(1);
  
      cout << "\n当水质为中度污染的时候---------------〉" << endl;
      subject->setPolluteLevel(2);
  
      // 释放观察者
      subject->detach(watcher1);
      subject->detach(watcher2);
      subject->detach(watcher3);
  
      delete watcher1;
      delete watcher2;
      delete watcher3;
  
      delete subject;
  
      return 0;
  }
  ```
  </details>

#### 5.2 工厂模式

![工厂模式](./images/工厂模式.png?raw=true)

- 定义一个用于创建对象的接口，让子类决定实例化哪一个类。工厂方法是一个类的实例化延迟到其子类。

- [代码](./code/designpattern/factory.cpp)

  <details>
  <summary>工厂模式</summary>
  
  ```C++
  #include <iostream>
  
  using namespace std;
  
  class ExportFileProduct
  {
  public:
      ExportFileProduct() {}
      virtual ~ExportFileProduct() {}
  
      virtual bool Export(string data) = 0;
  };
  
  // 保存成文件
  class ExportTextProduct : public ExportFileProduct
  {
  public:
      ExportTextProduct() {}
      virtual ~ExportTextProduct() {}
      virtual bool Export(string data)
      {
          cout << "导出数据:[" << data << "]保存成文本的方式" << endl;
          return true;
      }
  };
  
  class ExportDBProduct : public ExportFileProduct
  {
  public:
      ExportDBProduct() {}
      virtual ~ExportDBProduct() {}
      virtual bool Export(string data)
      {
          cout << "导出数据:[" << data << "]保存数据库的方式" << endl;
          return true;
      }
  };
  
  class ExportFactory
  {
  public:
      ExportFactory() {}
      virtual ~ExportFactory() {}
      /**
       * @brief Export
       * @param type 导出的类型
       * @param data 具体的数据
       * @return
       */
      virtual bool Export(int type, string data)
      {
          ExportFileProduct *product = nullptr;
          product = factoryMethod(type);
  
          bool ret = false;
          if (product)
          {
              ret = product->Export(data);
              delete product;
          }
          else
          {
              cout << "没有对应的导出方式";
          }
          return ret;
      }
  
  protected:
      virtual ExportFileProduct *factoryMethod(int type)
      {
          ExportFileProduct *product = nullptr;
          if (1 == type)
          {
              product = new ExportTextProduct();
          }
          else if (2 == type)
          {
              product = new ExportDBProduct();
          }
          return product;
      }
  };
  
  // 加一种新的导出方式:
  // (1)修改原来的工厂方法
  // (2)继承工厂方法去拓展
  class ExportXMLProduct : public ExportFileProduct
  {
  public:
      ExportXMLProduct() {}
      virtual ~ExportXMLProduct() {}
      virtual bool Export(string data)
      {
          cout << "导出数据:[" << data << "]保存XML的方式" << endl;
          return true;
      }
  };
  
  class ExportPortobufferProduct : public ExportFileProduct
  {
  public:
      ExportPortobufferProduct() {}
      virtual ~ExportPortobufferProduct() {}
      virtual bool Export(string data)
      {
          cout << "导出数据:[" << data << "]保存Portobuffer的方式" << endl;
          return true;
      }
  };
  
  class ExportFactory2 : public ExportFactory
  {
  public:
      ExportFactory2() {}
      virtual ~ExportFactory2() {}
  
  protected:
      virtual ExportFileProduct *factoryMethod(int type)
      {
          ExportFileProduct *product = nullptr;
          if (3 == type)
          {
              product = new ExportXMLProduct();
          }
          else if (4 == type)
          {
              product = new ExportPortobufferProduct();
          }
          else
          {
              product = ExportFactory::factoryMethod(type);
          }
          return product;
      }
  };
  
  int main()
  {
      cout << "ExportFactory" << endl;
      ExportFactory *factory = new ExportFactory();
  
      factory->Export(1, "上课人数");
      factory->Export(2, "上课人数");
      factory->Export(3, "上课人数");
  
      cout << "\nExportFactory2" << endl;
      ExportFactory *factory2 = new ExportFactory2();
  
      factory2->Export(1, "上课人数");
      factory2->Export(2, "上课人数");
      factory2->Export(3, "上课人数");
      factory2->Export(4, "上课人数");
  
      delete factory;
      delete factory2;
      return 0;
  }
  ```
  </details>

#### 5.3 单例模式

- 确保某一个类只有一个实例，而且自行实例化并向整个系统提供这个实例。

- [代码](./code/designpattern/singleton.cpp)

  <details>
  <summary>单例模式</summary>
  
  ```C++
  #define SINGLETON_INDEX 6 // 开关，不同模式的开关
  
  /*
   * 补充：
   * =default: 用于显式要求编译器提供合成版本的四大函数(构造、拷贝、析构、赋值)
   * =delete: 用于定义删除函数，在旧标准下，我们如果希望阻止拷贝可以通过显式声明拷贝构造函数和拷贝赋值函数为private，但新标准下允许我们定义删除函数
   */
  
  // 反汇编举例 objdump -S -d 4-singleton-c++11 > 4-singleton-c++11.txt
  // 直接汇编：g++ -S -o main2-2.s main2.cpp -std=c++11
  
  //1、原始懒汉式单例模式 懒汉式单例就是需要使用这个单例对象的时候才去创建这个单例对象。
  #if SINGLETON_INDEX == 1
  class Singleton
  {
  private:
      static Singleton *m_singleton;
      Singleton() = default;                             // 自动生成默认构造函数
      Singleton(const Singleton &s) = delete;            // 禁用拷贝构造函数
      Singleton &operator=(const Singleton &s) = delete; // 禁用拷贝赋值操作符
  
      class GarbageCollector
      {
      public:
          ~GarbageCollector()
          {
              cout << "~GarbageCollector\n";
              if (Singleton::m_singleton)
              {
                  cout << "free m_singleton\n";
                  delete Singleton::m_singleton;
                  Singleton::m_singleton = nullptr;
              }
          }
      };
      static GarbageCollector m_gc;
  
  public:
      static Singleton *getInstance()
      {
          if (Singleton::m_singleton == nullptr)
          {
              std::this_thread::sleep_for(std::chrono::milliseconds(10)); //休眠，模拟创建实例的时间
              m_singleton = new Singleton();
          }
          return m_singleton;
      }
  };
  // 必须在类外初始化
  Singleton *Singleton::m_singleton = nullptr;
  Singleton::GarbageCollector Singleton::m_gc;
  #elif SINGLETON_INDEX == 2
  // 2 线程安全的懒汉式单例模式
  //线程安全的懒汉式单例
  
  class Singleton
  {
  private:
      static Singleton *m_singleton;
      static mutex m_mutex;
      Singleton() = default;
      Singleton(const Singleton &s) = delete;            // 禁用拷贝构造函数
      Singleton &operator=(const Singleton &s) = delete; // 禁用拷贝赋值操作符
      class GarbageCollector
      {
      public:
          ~GarbageCollector()
          {
              cout << "~GarbageCollector\n";
              if (Singleton::m_singleton)
              {
                  cout << "free m_singleton\n";
                  delete Singleton::m_singleton;
                  Singleton::m_singleton = nullptr;
              }
          }
      };
      static GarbageCollector m_gc;
  
  public:
      static Singleton *getInstance()
      {                   // 加锁的粒度大，效率较低， 对高并发的访问
          m_mutex.lock(); // 加锁，保证只有一个线程在访问下面的语句
          if (Singleton::m_singleton == nullptr)
          {
              //std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //休眠，模拟创建实例的时间
              m_singleton = new Singleton();
          }
          m_mutex.unlock(); //解锁
          return m_singleton;
      }
  };
  Singleton *Singleton::m_singleton = nullptr;
  mutex Singleton::m_mutex;
  Singleton::GarbageCollector Singleton::m_gc;
  #elif SINGLETON_INDEX == 3
  // 3、锁住初始化实例语句之后再次检查实例是否被创建
  /* 双检查锁，但由于内存读写reorder不安全 因为C++创建对象时，会执行1、分配内存，2 调用构造，3 赋值操作三步操作，
  然而现代CPU和编译器高并发下可能会进行乱序重排操作，因而创建对象new CSingleton的第2步可能会晚于第3步进行指令调用，
  因而导致出现未定义的的行为。*/
  class Singleton
  {
  private:
      static Singleton *m_singleton;
      static mutex m_mutex;
      Singleton() = default;
      Singleton(const Singleton &s) = default;
      Singleton &operator=(const Singleton &s) = default;
      class GarbageCollector
      {
      public:
          ~GarbageCollector()
          {
              cout << "~GarbageCollector\n";
              if (Singleton::m_singleton)
              {
                  cout << "free m_singleton\n";
                  delete Singleton::m_singleton;
                  Singleton::m_singleton = nullptr;
              }
          }
      };
      static GarbageCollector m_gc;
  
  public:
      void *getSingletonAddress()
      {
          return m_singleton;
      }
      static Singleton *getInstance()
      {
          if (Singleton::m_singleton == nullptr)
          {
              m_mutex.lock(); // 加锁，保证只有一个线程在访问线程内的代码
              if (Singleton::m_singleton == nullptr)
              {                                  //再次检查
                  m_singleton = new Singleton(); // 对象的new不是原子操作 1、分配内存，2 调用构造，3 赋值操作，到第3步的时候才是m_singleton非空
                                                 //  1、分配内存，2 赋值操作 3 调用构造，到第2步的时候才是m_singleton非空
              }
              m_mutex.unlock(); //解锁
          }
          return m_singleton;
      }
  };
  Singleton *Singleton::m_singleton = nullptr;
  mutex Singleton::m_mutex;
  Singleton::GarbageCollector Singleton::m_gc;
  #elif SINGLETON_INDEX == 4
  // 4、C++ 11版本之后的跨平台实现
  class Singleton
  {
  private:
      static std::atomic<Singleton *> m_instance;
      static std::mutex m_mutex;
      Singleton() = default;
      Singleton(const Singleton &s) = default;
      Singleton &operator=(const Singleton &s) = default;
      class GarbageCollector
      {
      public:
          ~GarbageCollector()
          {
              cout << "~GarbageCollector\n";
              Singleton *tmp = m_instance.load(std::memory_order_relaxed);
              if (tmp)
              {
                  cout << "free m_singleton: " << tmp << endl;
                  delete tmp;
              }
          }
      };
      static GarbageCollector m_gc;
  
  public:
      void *getSingletonAddress()
      {
          return m_instance;
      }
      static Singleton *getInstance()
      {
          Singleton *tmp = m_instance.load(std::memory_order_relaxed);
          std::atomic_thread_fence(std::memory_order_acquire); //获取内存fence
          if (tmp == nullptr)
          {
              std::lock_guard<std::mutex> lock(m_mutex);
              tmp = m_instance.load(std::memory_order_relaxed);
              if (tmp == nullptr)
              {
                  tmp = new Singleton();                               // 1、分配内存，2 调用构造，3 赋值操作
                  std::atomic_thread_fence(std::memory_order_release); //释放内存fence
                  m_instance.store(tmp, std::memory_order_relaxed);
              }
          }
          return tmp;
      }
  };
  std::atomic<Singleton *> Singleton::m_instance;
  std::mutex Singleton::m_mutex;
  Singleton::GarbageCollector Singleton::m_gc;
  #elif SINGLETON_INDEX == 5
  // 懒汉式
  class Singleton
  {
  private:
      //Singleton() = default;  // 自动生成默认构造函数
      Singleton()
      { // 构造函数会影响局部静态变量，不能用隐式的构造函数
          cout << "Singleton construct\n";
      }
      Singleton(const Singleton &s) = delete;            // 禁用拷贝构造函数
      Singleton &operator=(const Singleton &s) = delete; // 禁用拷贝赋值操作符
  public:
      static Singleton *getInstance()
      {
          static Singleton s_singleton; // C++11线程安全，C++11之前不是线程安全  __cxa_guard_acquire 和 __cxa_guard_release
          return &s_singleton;
      }
  };
  #elif SINGLETON_INDEX == 6
  // 饿汉式，在main函数运行前初始化，绝对安全
  class Singleton
  {
  private:
      //Singleton() = default;   //自动生成默认构造函数
      Singleton()
      {
          cout << "Singleton construct\n";
      }
      Singleton(const Singleton &s) = delete;            // 禁用拷贝构造函数
      Singleton &operator=(const Singleton &s) = delete; // 禁用拷贝赋值操作符
      static Singleton m_singleton;
  
  public:
      static Singleton *getInstance()
      {
          return &m_singleton;
      }
  };
  Singleton Singleton::m_singleton;
  #endif
  ```
  </details>

## 二、后台组件编程专栏

### 1.MySQL基本操作与编程

#### 1.1 MySQL基础逻辑框架

![MySQL基础逻辑框架](./images/MySQL基础逻辑框架.png?raw=true)

#### 1.2 基本操作

- 安装

  ```shell
  sudo apt-get install mysql-server mysql-client -y
  ```

- 启动、重启、关闭

  ```shell
  sudo /etc/init.d/mysql start|stop|restart
  ```

- 连接

  ```shell
  mysql -h127.0.0.1 -uroot -p #root用户默认密码为空
  ```

- ERROR 1698 (28000): Access denied for user 'root'@'localhost'问题解决

  ```shell
  # 查看/etc/mysql/debian.cnf
  sudo vim /etc/mysql/debian.cnf
  ###########################################################
  # Automatically generated for Debian scripts. DO NOT TOUCH!
  [client]
  host     = localhost
  user     = debian-sys-maint # 记住这两项
  password = 5VRSokq1P0uQ2S39 # 记住这两项
  socket   = /var/run/mysqld/mysqld.sock
  [mysql_upgrade]
  host     = localhost
  user     = debian-sys-maint
  password = 5VRSokq1P0uQ2S39
  socket   = /var/run/mysqld/mysqld.sock
  ###########################################################
  mysql -udebian-sys-maint -p5VRSokq1P0uQ2S39
  # 登录MySQL成功后
  select user, plugin from mysql.user;
  update mysql.user set authentication_string=PASSWORD('123'), plugin='mysql_native_password' where user='root';
  flush privileges;
  ```

- 数据库操作

  ```sql
  # 创建数据库
  create database `test` default character set utf8;
  # 使用数据库
  use test;
  # 创建表
  create table user_info (
  	id int not null auto_increment,
  	`name` varchar(20),
  	`title` varchar(20),
  	primary key(id)
  )engine = InnoDB charset = utf8;
  # 增删查改
  insert into user_info(`name`, `title`)values('gongluck', 'test');
  select * from user_info;
  update user_info set `name`='test' where id = 1;
  delete from user_info where id = 1;
  ```

- 用户操作

  ```sql
  # 创建用户
  #CREATE USER username@host IDENTIFIED BY password;
  CREATE USER 'gongluck'@'%' IDENTIFIED BY '123';
  
  # 授权
  #GRANT privileges ON databasename.tablename TO 'username'@'host' WITH GRANT OPTION;
  #privileges：用户的操作权限，如SELECT，INSERT，UPDATE等，如果要授予所的权限则使用ALL
  GRANT SELECT, REPLICATION SLAVE, REPLICATION CLIENT ON *.* TO 'gongluck'@'%';
  GRANT ALL PRIVILEGES ON *.* TO 'gongluck'@'%';
  FLUSH PRIVILEGES;
  ```

#### 1.3 libmysql编程环境

- 安装

  ```shell
  sudo apt-get install libmysqlclient-dev -y
  ```

- [头文件（/usr/include/mysql/mysql.h）](./code/mysql/mysql.h)

  <details>
  <summary>mysql.h</summary>
  
  ```C
  /* Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved.
  
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License, version 2.0,
     as published by the Free Software Foundation.
  
     This program is also distributed with certain software (including
     but not limited to OpenSSL) that is licensed under separate terms,
     as designated in a particular file or component or in included license
     documentation.  The authors of MySQL hereby grant you an additional
     permission to link the program and your derivative works with the
     separately licensed software that they have included with MySQL.
  
     Without limiting anything contained in the foregoing, this file,
     which is part of C Driver for MySQL (Connector/C), is also subject to the
     Universal FOSS Exception, version 1.0, a copy of which can be found at
     http://oss.oracle.com/licenses/universal-foss-exception.
  
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License, version 2.0, for more details.
  
     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */
  
  /*
    This file defines the client API to MySQL and also the ABI of the
    dynamically linked libmysqlclient.
  
    The ABI should never be changed in a released product of MySQL,
    thus you need to take great care when changing the file. In case
    the file is changed so the ABI is broken, you must also update
    the SHARED_LIB_MAJOR_VERSION in cmake/mysql_version.cmake
  */
  
  #ifndef _mysql_h
  #define _mysql_h
  
  #ifdef	__cplusplus
  extern "C" {
  #endif
  
  #ifndef MY_GLOBAL_INCLUDED                /* If not standard header */
  #ifndef MYSQL_ABI_CHECK
  #include <sys/types.h>
  #endif
  typedef char my_bool;
  #if !defined(_WIN32)
  #define STDCALL
  #else
  #define STDCALL __stdcall
  #endif
  
  #ifndef my_socket_defined
  #ifdef _WIN32
  #include <windows.h>
  #ifdef WIN32_LEAN_AND_MEAN
  #include <winsock2.h>
  #endif
  #define my_socket SOCKET
  #else
  typedef int my_socket;
  #endif /* _WIN32 */
  #endif /* my_socket_defined */
  #endif /* MY_GLOBAL_INCLUDED */
  
  #include "mysql_version.h"
  #include "mysql_com.h"
  #include "mysql_time.h"
  
  #include "my_list.h" /* for LISTs used in 'MYSQL' and 'MYSQL_STMT' */
  
  /* Include declarations of plug-in API */
  #include "mysql/client_plugin.h"
  
  extern unsigned int mysql_port;
  extern char *mysql_unix_port;
  
  #define CLIENT_NET_READ_TIMEOUT		365*24*3600	/* Timeout on read */
  #define CLIENT_NET_WRITE_TIMEOUT	365*24*3600	/* Timeout on write */
  
  #define IS_PRI_KEY(n)	((n) & PRI_KEY_FLAG)
  #define IS_NOT_NULL(n)	((n) & NOT_NULL_FLAG)
  #define IS_BLOB(n)	((n) & BLOB_FLAG)
  /**
     Returns true if the value is a number which does not need quotes for
     the sql_lex.cc parser to parse correctly.
  */
  #define IS_NUM(t)	(((t) <= MYSQL_TYPE_INT24 && (t) != MYSQL_TYPE_TIMESTAMP) || (t) == MYSQL_TYPE_YEAR || (t) == MYSQL_TYPE_NEWDECIMAL)
  #define IS_LONGDATA(t) ((t) >= MYSQL_TYPE_TINY_BLOB && (t) <= MYSQL_TYPE_STRING)
  
  
  typedef struct st_mysql_field {
    char *name;                 /* Name of column */
    char *org_name;             /* Original column name, if an alias */
    char *table;                /* Table of column if column was a field */
    char *org_table;            /* Org table name, if table was an alias */
    char *db;                   /* Database for table */
    char *catalog;	      /* Catalog for table */
    char *def;                  /* Default value (set by mysql_list_fields) */
    unsigned long length;       /* Width of column (create length) */
    unsigned long max_length;   /* Max width for selected set */
    unsigned int name_length;
    unsigned int org_name_length;
    unsigned int table_length;
    unsigned int org_table_length;
    unsigned int db_length;
    unsigned int catalog_length;
    unsigned int def_length;
    unsigned int flags;         /* Div flags */
    unsigned int decimals;      /* Number of decimals in field */
    unsigned int charsetnr;     /* Character set */
    enum enum_field_types type; /* Type of field. See mysql_com.h for types */
    void *extension;
  } MYSQL_FIELD;
  
  typedef char **MYSQL_ROW;		/* return data as array of strings */
  typedef unsigned int MYSQL_FIELD_OFFSET; /* offset to current field */
  
  #ifndef MY_GLOBAL_INCLUDED
  #if defined (_WIN32)
  typedef unsigned __int64 my_ulonglong;
  #else
  typedef unsigned long long my_ulonglong;
  #endif
  #endif
  
  #include "typelib.h"
  
  #define MYSQL_COUNT_ERROR (~(my_ulonglong) 0)
  
  /* backward compatibility define - to be removed eventually */
  #define ER_WARN_DATA_TRUNCATED WARN_DATA_TRUNCATED
  
  typedef struct st_mysql_rows {
    struct st_mysql_rows *next;		/* list of rows */
    MYSQL_ROW data;
    unsigned long length;
  } MYSQL_ROWS;
  
  typedef MYSQL_ROWS *MYSQL_ROW_OFFSET;	/* offset to current row */
  
  #include "my_alloc.h"
  
  typedef struct embedded_query_result EMBEDDED_QUERY_RESULT;
  typedef struct st_mysql_data {
    MYSQL_ROWS *data;
    struct embedded_query_result *embedded_info;
    MEM_ROOT alloc;
    my_ulonglong rows;
    unsigned int fields;
    /* extra info for embedded library */
    void *extension;
  } MYSQL_DATA;
  
  enum mysql_option 
  {
    MYSQL_OPT_CONNECT_TIMEOUT, MYSQL_OPT_COMPRESS, MYSQL_OPT_NAMED_PIPE,
    MYSQL_INIT_COMMAND, MYSQL_READ_DEFAULT_FILE, MYSQL_READ_DEFAULT_GROUP,
    MYSQL_SET_CHARSET_DIR, MYSQL_SET_CHARSET_NAME, MYSQL_OPT_LOCAL_INFILE,
    MYSQL_OPT_PROTOCOL, MYSQL_SHARED_MEMORY_BASE_NAME, MYSQL_OPT_READ_TIMEOUT,
    MYSQL_OPT_WRITE_TIMEOUT, MYSQL_OPT_USE_RESULT,
    MYSQL_OPT_USE_REMOTE_CONNECTION, MYSQL_OPT_USE_EMBEDDED_CONNECTION,
    MYSQL_OPT_GUESS_CONNECTION, MYSQL_SET_CLIENT_IP, MYSQL_SECURE_AUTH,
    MYSQL_REPORT_DATA_TRUNCATION, MYSQL_OPT_RECONNECT,
    MYSQL_OPT_SSL_VERIFY_SERVER_CERT, MYSQL_PLUGIN_DIR, MYSQL_DEFAULT_AUTH,
    MYSQL_OPT_BIND,
    MYSQL_OPT_SSL_KEY, MYSQL_OPT_SSL_CERT, 
    MYSQL_OPT_SSL_CA, MYSQL_OPT_SSL_CAPATH, MYSQL_OPT_SSL_CIPHER,
    MYSQL_OPT_SSL_CRL, MYSQL_OPT_SSL_CRLPATH,
    MYSQL_OPT_CONNECT_ATTR_RESET, MYSQL_OPT_CONNECT_ATTR_ADD,
    MYSQL_OPT_CONNECT_ATTR_DELETE,
    MYSQL_SERVER_PUBLIC_KEY,
    MYSQL_ENABLE_CLEARTEXT_PLUGIN,
    MYSQL_OPT_CAN_HANDLE_EXPIRED_PASSWORDS,
    MYSQL_OPT_SSL_ENFORCE,
    MYSQL_OPT_MAX_ALLOWED_PACKET, MYSQL_OPT_NET_BUFFER_LENGTH,
    MYSQL_OPT_TLS_VERSION,
    MYSQL_OPT_SSL_MODE,
    MYSQL_OPT_GET_SERVER_PUBLIC_KEY
  };
  
  /**
    @todo remove the "extension", move st_mysql_options completely
    out of mysql.h
  */
  struct st_mysql_options_extention; 
  
  struct st_mysql_options {
    unsigned int connect_timeout, read_timeout, write_timeout;
    unsigned int port, protocol;
    unsigned long client_flag;
    char *host,*user,*password,*unix_socket,*db;
    struct st_dynamic_array *init_commands;
    char *my_cnf_file,*my_cnf_group, *charset_dir, *charset_name;
    char *ssl_key;				/* PEM key file */
    char *ssl_cert;				/* PEM cert file */
    char *ssl_ca;					/* PEM CA file */
    char *ssl_capath;				/* PEM directory of CA-s? */
    char *ssl_cipher;				/* cipher to use */
    char *shared_memory_base_name;
    unsigned long max_allowed_packet;
    my_bool use_ssl;                              /* Deprecated ! Former use_ssl */
    my_bool compress,named_pipe;
    my_bool unused1;
    my_bool unused2;
    my_bool unused3;
    my_bool unused4;
    enum mysql_option methods_to_use;
    union {
      /*
        The ip/hostname to use when authenticating
        client against embedded server built with
        grant tables - only used in embedded server
      */
      char *client_ip;
  
      /*
        The local address to bind when connecting to
        remote server - not used in embedded server
      */
      char *bind_address;
    } ci;
    my_bool unused5;
    /* 0 - never report, 1 - always report (default) */
    my_bool report_data_truncation;
  
    /* function pointers for local infile support */
    int (*local_infile_init)(void **, const char *, void *);
    int (*local_infile_read)(void *, char *, unsigned int);
    void (*local_infile_end)(void *);
    int (*local_infile_error)(void *, char *, unsigned int);
    void *local_infile_userdata;
    struct st_mysql_options_extention *extension;
  };
  
  enum mysql_status 
  {
    MYSQL_STATUS_READY, MYSQL_STATUS_GET_RESULT, MYSQL_STATUS_USE_RESULT,
    MYSQL_STATUS_STATEMENT_GET_RESULT
  };
  
  enum mysql_protocol_type 
  {
    MYSQL_PROTOCOL_DEFAULT, MYSQL_PROTOCOL_TCP, MYSQL_PROTOCOL_SOCKET,
    MYSQL_PROTOCOL_PIPE, MYSQL_PROTOCOL_MEMORY
  };
  
  enum mysql_ssl_mode
  {
    SSL_MODE_DISABLED= 1, SSL_MODE_PREFERRED, SSL_MODE_REQUIRED,
    SSL_MODE_VERIFY_CA, SSL_MODE_VERIFY_IDENTITY
  };
  
  typedef struct character_set
  {
    unsigned int      number;     /* character set number              */
    unsigned int      state;      /* character set state               */
    const char        *csname;    /* collation name                    */
    const char        *name;      /* character set name                */
    const char        *comment;   /* comment                           */
    const char        *dir;       /* character set directory           */
    unsigned int      mbminlen;   /* min. length for multibyte strings */
    unsigned int      mbmaxlen;   /* max. length for multibyte strings */
  } MY_CHARSET_INFO;
  
  struct st_mysql_methods;
  struct st_mysql_stmt;
  
  typedef struct st_mysql
  {
    NET		net;			/* Communication parameters */
    unsigned char	*connector_fd;		/* ConnectorFd for SSL */
    char		*host,*user,*passwd,*unix_socket,*server_version,*host_info;
    char          *info, *db;
    struct charset_info_st *charset;
    MYSQL_FIELD	*fields;
    MEM_ROOT	field_alloc;
    my_ulonglong affected_rows;
    my_ulonglong insert_id;		/* id if insert on table with NEXTNR */
    my_ulonglong extra_info;		/* Not used */
    unsigned long thread_id;		/* Id for connection in server */
    unsigned long packet_length;
    unsigned int	port;
    unsigned long client_flag,server_capabilities;
    unsigned int	protocol_version;
    unsigned int	field_count;
    unsigned int 	server_status;
    unsigned int  server_language;
    unsigned int	warning_count;
    struct st_mysql_options options;
    enum mysql_status status;
    my_bool	free_me;		/* If free in mysql_close */
    my_bool	reconnect;		/* set to 1 if automatic reconnect */
  
    /* session-wide random string */
    char	        scramble[SCRAMBLE_LENGTH+1];
    my_bool unused1;
    void *unused2, *unused3, *unused4, *unused5;
  
    LIST  *stmts;                     /* list of all statements */
    const struct st_mysql_methods *methods;
    void *thd;
    /*
      Points to boolean flag in MYSQL_RES  or MYSQL_STMT. We set this flag 
      from mysql_stmt_close if close had to cancel result set of this object.
    */
    my_bool *unbuffered_fetch_owner;
    /* needed for embedded server - no net buffer to store the 'info' */
    char *info_buffer;
    void *extension;
  } MYSQL;
  
  
  typedef struct st_mysql_res {
    my_ulonglong  row_count;
    MYSQL_FIELD	*fields;
    MYSQL_DATA	*data;
    MYSQL_ROWS	*data_cursor;
    unsigned long *lengths;		/* column lengths of current row */
    MYSQL		*handle;		/* for unbuffered reads */
    const struct st_mysql_methods *methods;
    MYSQL_ROW	row;			/* If unbuffered read */
    MYSQL_ROW	current_row;		/* buffer to current row */
    MEM_ROOT	field_alloc;
    unsigned int	field_count, current_field;
    my_bool	eof;			/* Used by mysql_fetch_row */
    /* mysql_stmt_close() had to cancel this result */
    my_bool       unbuffered_fetch_cancelled;  
    void *extension;
  } MYSQL_RES;
  
  
  #if !defined(MYSQL_SERVER) && !defined(MYSQL_CLIENT)
  #define MYSQL_CLIENT
  #endif
  
  /*
    Set up and bring down the server; to ensure that applications will
    work when linked against either the standard client library or the
    embedded server library, these functions should be called.
  */
  int STDCALL mysql_server_init(int argc, char **argv, char **groups);
  void STDCALL mysql_server_end(void);
  
  /*
    mysql_server_init/end need to be called when using libmysqld or
    libmysqlclient (exactly, mysql_server_init() is called by mysql_init() so
    you don't need to call it explicitely; but you need to call
    mysql_server_end() to free memory). The names are a bit misleading
    (mysql_SERVER* to be used when using libmysqlCLIENT). So we add more general
    names which suit well whether you're using libmysqld or libmysqlclient. We
    intend to promote these aliases over the mysql_server* ones.
  */
  #define mysql_library_init mysql_server_init
  #define mysql_library_end mysql_server_end
  
  
  /*
    Set up and bring down a thread; these function should be called
    for each thread in an application which opens at least one MySQL
    connection.  All uses of the connection(s) should be between these
    function calls.
  */
  my_bool STDCALL mysql_thread_init(void);
  void STDCALL mysql_thread_end(void);
  
  /*
    Functions to get information from the MYSQL and MYSQL_RES structures
    Should definitely be used if one uses shared libraries.
  */
  
  my_ulonglong STDCALL mysql_num_rows(MYSQL_RES *res);
  unsigned int STDCALL mysql_num_fields(MYSQL_RES *res);
  my_bool STDCALL mysql_eof(MYSQL_RES *res);
  MYSQL_FIELD *STDCALL mysql_fetch_field_direct(MYSQL_RES *res,
  					      unsigned int fieldnr);
  MYSQL_FIELD * STDCALL mysql_fetch_fields(MYSQL_RES *res);
  MYSQL_ROW_OFFSET STDCALL mysql_row_tell(MYSQL_RES *res);
  MYSQL_FIELD_OFFSET STDCALL mysql_field_tell(MYSQL_RES *res);
  
  unsigned int STDCALL mysql_field_count(MYSQL *mysql);
  my_ulonglong STDCALL mysql_affected_rows(MYSQL *mysql);
  my_ulonglong STDCALL mysql_insert_id(MYSQL *mysql);
  unsigned int STDCALL mysql_errno(MYSQL *mysql);
  const char * STDCALL mysql_error(MYSQL *mysql);
  const char *STDCALL mysql_sqlstate(MYSQL *mysql);
  unsigned int STDCALL mysql_warning_count(MYSQL *mysql);
  const char * STDCALL mysql_info(MYSQL *mysql);
  unsigned long STDCALL mysql_thread_id(MYSQL *mysql);
  const char * STDCALL mysql_character_set_name(MYSQL *mysql);
  int          STDCALL mysql_set_character_set(MYSQL *mysql, const char *csname);
  
  MYSQL *		STDCALL mysql_init(MYSQL *mysql);
  my_bool		STDCALL mysql_ssl_set(MYSQL *mysql, const char *key,
  				      const char *cert, const char *ca,
  				      const char *capath, const char *cipher);
  const char *    STDCALL mysql_get_ssl_cipher(MYSQL *mysql);
  my_bool		STDCALL mysql_change_user(MYSQL *mysql, const char *user, 
  					  const char *passwd, const char *db);
  MYSQL *		STDCALL mysql_real_connect(MYSQL *mysql, const char *host,
  					   const char *user,
  					   const char *passwd,
  					   const char *db,
  					   unsigned int port,
  					   const char *unix_socket,
  					   unsigned long clientflag);
  int		STDCALL mysql_select_db(MYSQL *mysql, const char *db);
  int		STDCALL mysql_query(MYSQL *mysql, const char *q);
  int		STDCALL mysql_send_query(MYSQL *mysql, const char *q,
  					 unsigned long length);
  int		STDCALL mysql_real_query(MYSQL *mysql, const char *q,
  					unsigned long length);
  MYSQL_RES *     STDCALL mysql_store_result(MYSQL *mysql);
  MYSQL_RES *     STDCALL mysql_use_result(MYSQL *mysql);
  
  void        STDCALL mysql_get_character_set_info(MYSQL *mysql,
                             MY_CHARSET_INFO *charset);
  
  int STDCALL mysql_session_track_get_first(MYSQL *mysql,
                                            enum enum_session_state_type type,
                                            const char **data,
                                            size_t *length);
  int STDCALL mysql_session_track_get_next(MYSQL *mysql,
                                           enum enum_session_state_type type,
                                           const char **data,
                                           size_t *length);
  /* local infile support */
  
  #define LOCAL_INFILE_ERROR_LEN 512
  
  void
  mysql_set_local_infile_handler(MYSQL *mysql,
                                 int (*local_infile_init)(void **, const char *,
                              void *),
                                 int (*local_infile_read)(void *, char *,
  							unsigned int),
                                 void (*local_infile_end)(void *),
                                 int (*local_infile_error)(void *, char*,
  							 unsigned int),
                                 void *);
  
  void
  mysql_set_local_infile_default(MYSQL *mysql);
  
  int		STDCALL mysql_shutdown(MYSQL *mysql,
                                         enum mysql_enum_shutdown_level
                                         shutdown_level);
  int		STDCALL mysql_dump_debug_info(MYSQL *mysql);
  int		STDCALL mysql_refresh(MYSQL *mysql,
  				     unsigned int refresh_options);
  int		STDCALL mysql_kill(MYSQL *mysql,unsigned long pid);
  int		STDCALL mysql_set_server_option(MYSQL *mysql,
  						enum enum_mysql_set_option
  						option);
  int		STDCALL mysql_ping(MYSQL *mysql);
  const char *	STDCALL mysql_stat(MYSQL *mysql);
  const char *	STDCALL mysql_get_server_info(MYSQL *mysql);
  const char *	STDCALL mysql_get_client_info(void);
  unsigned long	STDCALL mysql_get_client_version(void);
  const char *	STDCALL mysql_get_host_info(MYSQL *mysql);
  unsigned long	STDCALL mysql_get_server_version(MYSQL *mysql);
  unsigned int	STDCALL mysql_get_proto_info(MYSQL *mysql);
  MYSQL_RES *	STDCALL mysql_list_dbs(MYSQL *mysql,const char *wild);
  MYSQL_RES *	STDCALL mysql_list_tables(MYSQL *mysql,const char *wild);
  MYSQL_RES *	STDCALL mysql_list_processes(MYSQL *mysql);
  int		STDCALL mysql_options(MYSQL *mysql,enum mysql_option option,
  				      const void *arg);
  int		STDCALL mysql_options4(MYSQL *mysql,enum mysql_option option,
                                         const void *arg1, const void *arg2);
  int             STDCALL mysql_get_option(MYSQL *mysql, enum mysql_option option,
                                           const void *arg);
  void		STDCALL mysql_free_result(MYSQL_RES *result);
  void		STDCALL mysql_data_seek(MYSQL_RES *result,
  					my_ulonglong offset);
  MYSQL_ROW_OFFSET STDCALL mysql_row_seek(MYSQL_RES *result,
  						MYSQL_ROW_OFFSET offset);
  MYSQL_FIELD_OFFSET STDCALL mysql_field_seek(MYSQL_RES *result,
  					   MYSQL_FIELD_OFFSET offset);
  MYSQL_ROW	STDCALL mysql_fetch_row(MYSQL_RES *result);
  unsigned long * STDCALL mysql_fetch_lengths(MYSQL_RES *result);
  MYSQL_FIELD *	STDCALL mysql_fetch_field(MYSQL_RES *result);
  MYSQL_RES *     STDCALL mysql_list_fields(MYSQL *mysql, const char *table,
  					  const char *wild);
  unsigned long	STDCALL mysql_escape_string(char *to,const char *from,
  					    unsigned long from_length);
  unsigned long	STDCALL mysql_hex_string(char *to,const char *from,
                                           unsigned long from_length);
  unsigned long STDCALL mysql_real_escape_string(MYSQL *mysql,
  					       char *to,const char *from,
  					       unsigned long length);
  unsigned long STDCALL mysql_real_escape_string_quote(MYSQL *mysql,
                   char *to, const char *from,
                   unsigned long length, char quote);
  void          STDCALL mysql_debug(const char *debug);
  void          STDCALL myodbc_remove_escape(MYSQL *mysql,char *name);
  unsigned int  STDCALL mysql_thread_safe(void);
  my_bool       STDCALL mysql_embedded(void);
  my_bool       STDCALL mysql_read_query_result(MYSQL *mysql);
  int           STDCALL mysql_reset_connection(MYSQL *mysql);
  
  /*
    The following definitions are added for the enhanced 
    client-server protocol
  */
  
  /* statement state */
  enum enum_mysql_stmt_state
  {
    MYSQL_STMT_INIT_DONE= 1, MYSQL_STMT_PREPARE_DONE, MYSQL_STMT_EXECUTE_DONE,
    MYSQL_STMT_FETCH_DONE
  };
  
  
  /*
    This structure is used to define bind information, and
    internally by the client library.
    Public members with their descriptions are listed below
    (conventionally `On input' refers to the binds given to
    mysql_stmt_bind_param, `On output' refers to the binds given
    to mysql_stmt_bind_result):
  
    buffer_type    - One of the MYSQL_* types, used to describe
                     the host language type of buffer.
                     On output: if column type is different from
                     buffer_type, column value is automatically converted
                     to buffer_type before it is stored in the buffer.
    buffer         - On input: points to the buffer with input data.
                     On output: points to the buffer capable to store
                     output data.
                     The type of memory pointed by buffer must correspond
                     to buffer_type. See the correspondence table in
                     the comment to mysql_stmt_bind_param.
  
    The two above members are mandatory for any kind of bind.
  
    buffer_length  - the length of the buffer. You don't have to set
                     it for any fixed length buffer: float, double,
                     int, etc. It must be set however for variable-length
                     types, such as BLOBs or STRINGs.
  
    length         - On input: in case when lengths of input values
                     are different for each execute, you can set this to
                     point at a variable containining value length. This
                     way the value length can be different in each execute.
                     If length is not NULL, buffer_length is not used.
                     Note, length can even point at buffer_length if
                     you keep bind structures around while fetching:
                     this way you can change buffer_length before
                     each execution, everything will work ok.
                     On output: if length is set, mysql_stmt_fetch will
                     write column length into it.
  
    is_null        - On input: points to a boolean variable that should
                     be set to TRUE for NULL values.
                     This member is useful only if your data may be
                     NULL in some but not all cases.
                     If your data is never NULL, is_null should be set to 0.
                     If your data is always NULL, set buffer_type
                     to MYSQL_TYPE_NULL, and is_null will not be used.
  
    is_unsigned    - On input: used to signify that values provided for one
                     of numeric types are unsigned.
                     On output describes signedness of the output buffer.
                     If, taking into account is_unsigned flag, column data
                     is out of range of the output buffer, data for this column
                     is regarded truncated. Note that this has no correspondence
                     to the sign of result set column, if you need to find it out
                     use mysql_stmt_result_metadata.
    error          - where to write a truncation error if it is present.
                     possible error value is:
                     0  no truncation
                     1  value is out of range or buffer is too small
  
    Please note that MYSQL_BIND also has internals members.
  */
  
  typedef struct st_mysql_bind
  {
    unsigned long	*length;          /* output length pointer */
    my_bool       *is_null;	  /* Pointer to null indicator */
    void		*buffer;	  /* buffer to get/put data */
    /* set this if you want to track data truncations happened during fetch */
    my_bool       *error;
    unsigned char *row_ptr;         /* for the current data position */
    void (*store_param_func)(NET *net, struct st_mysql_bind *param);
    void (*fetch_result)(struct st_mysql_bind *, MYSQL_FIELD *,
                         unsigned char **row);
    void (*skip_result)(struct st_mysql_bind *, MYSQL_FIELD *,
  		      unsigned char **row);
    /* output buffer length, must be set when fetching str/binary */
    unsigned long buffer_length;
    unsigned long offset;           /* offset position for char/binary fetch */
    unsigned long length_value;     /* Used if length is 0 */
    unsigned int	param_number;	  /* For null count and error messages */
    unsigned int  pack_length;	  /* Internal length for packed data */
    enum enum_field_types buffer_type;	/* buffer type */
    my_bool       error_value;      /* used if error is 0 */
    my_bool       is_unsigned;      /* set if integer type is unsigned */
    my_bool	long_data_used;	  /* If used with mysql_send_long_data */
    my_bool	is_null_value;    /* Used if is_null is 0 */
    void *extension;
  } MYSQL_BIND;
  
  
  struct st_mysql_stmt_extension;
  
  /* statement handler */
  typedef struct st_mysql_stmt
  {
    MEM_ROOT       mem_root;             /* root allocations */
    LIST           list;                 /* list to keep track of all stmts */
    MYSQL          *mysql;               /* connection handle */
    MYSQL_BIND     *params;              /* input parameters */
    MYSQL_BIND     *bind;                /* output parameters */
    MYSQL_FIELD    *fields;              /* result set metadata */
    MYSQL_DATA     result;               /* cached result set */
    MYSQL_ROWS     *data_cursor;         /* current row in cached result */
    /*
      mysql_stmt_fetch() calls this function to fetch one row (it's different
      for buffered, unbuffered and cursor fetch).
    */
    int            (*read_row_func)(struct st_mysql_stmt *stmt, 
                                    unsigned char **row);
    /* copy of mysql->affected_rows after statement execution */
    my_ulonglong   affected_rows;
    my_ulonglong   insert_id;            /* copy of mysql->insert_id */
    unsigned long	 stmt_id;	       /* Id for prepared statement */
    unsigned long  flags;                /* i.e. type of cursor to open */
    unsigned long  prefetch_rows;        /* number of rows per one COM_FETCH */
    /*
      Copied from mysql->server_status after execute/fetch to know
      server-side cursor status for this statement.
    */
    unsigned int   server_status;
    unsigned int	 last_errno;	       /* error code */
    unsigned int   param_count;          /* input parameter count */
    unsigned int   field_count;          /* number of columns in result set */
    enum enum_mysql_stmt_state state;    /* statement state */
    char		 last_error[MYSQL_ERRMSG_SIZE]; /* error message */
    char		 sqlstate[SQLSTATE_LENGTH+1];
    /* Types of input parameters should be sent to server */
    my_bool        send_types_to_server;
    my_bool        bind_param_done;      /* input buffers were supplied */
    unsigned char  bind_result_done;     /* output buffers were supplied */
    /* mysql_stmt_close() had to cancel this result */
    my_bool       unbuffered_fetch_cancelled;  
    /*
      Is set to true if we need to calculate field->max_length for 
      metadata fields when doing mysql_stmt_store_result.
    */
    my_bool       update_max_length;     
    struct st_mysql_stmt_extension *extension;
  } MYSQL_STMT;
  
  enum enum_stmt_attr_type
  {
    /*
      When doing mysql_stmt_store_result calculate max_length attribute
      of statement metadata. This is to be consistent with the old API, 
      where this was done automatically.
      In the new API we do that only by request because it slows down
      mysql_stmt_store_result sufficiently.
    */
    STMT_ATTR_UPDATE_MAX_LENGTH,
    /*
      unsigned long with combination of cursor flags (read only, for update,
      etc)
    */
    STMT_ATTR_CURSOR_TYPE,
    /*
      Amount of rows to retrieve from server per one fetch if using cursors.
      Accepts unsigned long attribute in the range 1 - ulong_max
    */
    STMT_ATTR_PREFETCH_ROWS
  };
  
  
  MYSQL_STMT * STDCALL mysql_stmt_init(MYSQL *mysql);
  int STDCALL mysql_stmt_prepare(MYSQL_STMT *stmt, const char *query,
                                 unsigned long length);
  int STDCALL mysql_stmt_execute(MYSQL_STMT *stmt);
  int STDCALL mysql_stmt_fetch(MYSQL_STMT *stmt);
  int STDCALL mysql_stmt_fetch_column(MYSQL_STMT *stmt, MYSQL_BIND *bind_arg, 
                                      unsigned int column,
                                      unsigned long offset);
  int STDCALL mysql_stmt_store_result(MYSQL_STMT *stmt);
  unsigned long STDCALL mysql_stmt_param_count(MYSQL_STMT * stmt);
  my_bool STDCALL mysql_stmt_attr_set(MYSQL_STMT *stmt,
                                      enum enum_stmt_attr_type attr_type,
                                      const void *attr);
  my_bool STDCALL mysql_stmt_attr_get(MYSQL_STMT *stmt,
                                      enum enum_stmt_attr_type attr_type,
                                      void *attr);
  my_bool STDCALL mysql_stmt_bind_param(MYSQL_STMT * stmt, MYSQL_BIND * bnd);
  my_bool STDCALL mysql_stmt_bind_result(MYSQL_STMT * stmt, MYSQL_BIND * bnd);
  my_bool STDCALL mysql_stmt_close(MYSQL_STMT * stmt);
  my_bool STDCALL mysql_stmt_reset(MYSQL_STMT * stmt);
  my_bool STDCALL mysql_stmt_free_result(MYSQL_STMT *stmt);
  my_bool STDCALL mysql_stmt_send_long_data(MYSQL_STMT *stmt, 
                                            unsigned int param_number,
                                            const char *data, 
                                            unsigned long length);
  MYSQL_RES *STDCALL mysql_stmt_result_metadata(MYSQL_STMT *stmt);
  MYSQL_RES *STDCALL mysql_stmt_param_metadata(MYSQL_STMT *stmt);
  unsigned int STDCALL mysql_stmt_errno(MYSQL_STMT * stmt);
  const char *STDCALL mysql_stmt_error(MYSQL_STMT * stmt);
  const char *STDCALL mysql_stmt_sqlstate(MYSQL_STMT * stmt);
  MYSQL_ROW_OFFSET STDCALL mysql_stmt_row_seek(MYSQL_STMT *stmt, 
                                               MYSQL_ROW_OFFSET offset);
  MYSQL_ROW_OFFSET STDCALL mysql_stmt_row_tell(MYSQL_STMT *stmt);
  void STDCALL mysql_stmt_data_seek(MYSQL_STMT *stmt, my_ulonglong offset);
  my_ulonglong STDCALL mysql_stmt_num_rows(MYSQL_STMT *stmt);
  my_ulonglong STDCALL mysql_stmt_affected_rows(MYSQL_STMT *stmt);
  my_ulonglong STDCALL mysql_stmt_insert_id(MYSQL_STMT *stmt);
  unsigned int STDCALL mysql_stmt_field_count(MYSQL_STMT *stmt);
  
  my_bool STDCALL mysql_commit(MYSQL * mysql);
  my_bool STDCALL mysql_rollback(MYSQL * mysql);
  my_bool STDCALL mysql_autocommit(MYSQL * mysql, my_bool auto_mode);
  my_bool STDCALL mysql_more_results(MYSQL *mysql);
  int STDCALL mysql_next_result(MYSQL *mysql);
  int STDCALL mysql_stmt_next_result(MYSQL_STMT *stmt);
  void STDCALL mysql_close(MYSQL *sock);
  
  
  /* status return codes */
  #define MYSQL_NO_DATA        100
  #define MYSQL_DATA_TRUNCATED 101
  
  #define mysql_reload(mysql) mysql_refresh((mysql),REFRESH_GRANT)
  
  #define HAVE_MYSQL_REAL_CONNECT
  
  #ifdef	__cplusplus
  }
  #endif
  
  #endif /* _mysql_h */
  ```
  </details>

- [例子代码](./code/mysql/mysql.c)

  <details>
  <summary>例子代码</summary>
  
  ```C
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;
  
  char server[] = "127.0.0.1";
  char user[] = "gongluck";
  char password[] = "123";
  char database[] = "test";
  
  conn = mysql_init(NULL);
  if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
  {
      printf("%s\n", mysql_error(conn));
  }
  
  char sql[128] = {0};
  sprintf(sql, "insert into user_info(`name`,title)values('gongluck', 'test');");
  if (mysql_query(conn, sql))
  {
      printf("%s\n", mysql_error(conn));
  }
  
  if (mysql_query(conn, "select * from user_info"))
  {
      printf("%s\n", mysql_error(conn));
  }
  
  res = mysql_use_result(conn);
  while ((row = mysql_fetch_row(res)) != NULL)
  {
       printf("%s\t", row[0]);
       printf("%s\t", row[1]);
       printf("%s\n", row[2]);
  }
  
  mysql_free_result(res);
  mysql_close(conn);
  ```
  </details>

### 2.MySQL事务、索引、存储引擎

#### 2.1 MySQL事务

- 在MySQL中只有使用了Innodb数据库引擎的数据库或表才支持事务。
- 事务处理可以用来维护数据库的完整性，保证成批的SQL语句要么全部执行，要么全部不执行。
- 事务用来管理insert,update,delete语句。 
- 事务四大特征： 
  - 原子性（Atomicity，或称不可分割性）
  - 一致性（Consistency）
  - 隔离性（Isolation，又称独立性）
  - 持久性（Durability）

- 按照锁的粒度把数据库锁分为行级锁(INNODB引擎)、表级锁(MYISAM引擎)和页级锁(BDB引擎 )。
  - 行级锁
    - 行级锁是Mysql中锁定粒度最细的一种锁，表示只针对当前操作的行进行加锁。
    - 行级锁能大大减少数据库操作的冲突。
    - 其加锁粒度最小，但加锁的开销也最大。
    - 行级锁分为共享锁和排他锁。 
    - 特点：开销大，加锁慢；会出现死锁；锁定粒度最小，发生锁冲突的概率最低，并发度也最高。
  - 表级锁
    - 表级锁是MySQL中锁定粒度最大的一种锁，表示对当前操作的整张表加锁，它实现简单，资源消耗较少，被大部分 MySQL引擎支持。
    - 最常使用的MYISAM与INNODB都支持表级锁定。
    - 表级锁定分为表共享读锁（共享锁）与表独占写锁 （排他锁）。 
    - 特点：开销小，加锁快；不会出现死锁；锁定粒度大，发出锁冲突的概率最高，并发度最低。 
  - 页级锁
    - 页级锁是MySQL中锁定粒度介于行级锁和表级锁中间的一种锁。
    - 表级锁速度快，但冲突多，行级冲突少，但速度慢。 所以取了折衷的页级，一次锁定相邻的一组记录。
    - BDB支持页级锁
    - 特点：开销和加锁时间界于表锁和行锁之间；会出现死锁；锁定粒度界于表锁和行锁之间，并发度一般。

#### 2.2 MySQL索引

- 根据存储分类
  - B-树索引
  - 哈希索引

- 根据用途分类
  - 普通索引
  - 唯一性索引
  - 主键索引
  - 空间索引
  - 全文索引

- 索引的实现原理
  - MyISAM引擎使用B+Tree作为索引结构，叶节点的data域存放的是数据记录的地址。
  - InnoDB也使用B+Tree作为索引结构，InnoDB的数据文件本身就是索引文件。

#### 2.3 MySQL存储引擎

![MySQL存储引擎](./images/MySQL存储引擎.png?raw=true)

- 创建表指定存储引擎（默认InnoDB）

  ```sql
  CREATE TABLE `test`(`id` int(11) NOT NULL AUTO_INCREMENT) ENGINE = InnoDB; 
  ```

- 修改数据表命令

  ```sql
  alter table test engine = MyISAM;
  ```

- InnoDB

  - 所有的数据按照主键来组织。数据和索引放在一块，都位于B+数的叶子节点上

  - InnoDB存储引擎在磁盘中存放的对应的表的磁盘文件有*.frm*、*.ibd*这两个文件
  - frm文件是存放表结构，表的定义信息
  - ibd文件是存放表中的数据、索引信息

- MyISAM

  - 主键索引的叶子节点只存放数据在物理磁盘上的指针

  - MyISAM索引文件在数据库中存放的对应表的磁盘文件有*.frm*、*.MYD*、*.MYI*结尾的三个文件
  - frm文件是存放的表结构，表的定义信息
  - MYD文件是存放着表中的数据
  - MYI文件存放着表的索引信息

### 3.Nginx反向代理负载均衡配置

#### 3.1 安装编译

```shell
# 安装依赖
sudo apt-get update
sudo apt-get install build-essential libtool -y
sudo apt-get install libpcre3 libpcre3-dev -y
sudo apt-get install zlib1g-dev -y
sudo apt-get install openssl -y
#下载nginx
wget http://nginx.org/download/nginx-1.19.0.tar.gz
tar zxvf nginx-1.19.0.tar.gz
cd nginx-1.19.0/
# 配置
./configure --prefix=/usr/local/nginx --with-http_ssl_module --with-http_realip_module --with-http_addition_module --with-http_gzip_static_module --with-http_secure_link_module --with-http_stub_status_module --with-stream --with-pcre #--with-zlib --with-openssl
# 编译
make -j 8
# 安装
sudo make install
# 启动
sudo /usr/local/nginx/sbin/nginx -c test.conf
# 停止
sudo /usr/local/nginx/sbin/nginx -s stop
# 重新加载配置文件
sudo /usr/local/nginx/sbin/nginx -s reload
```

#### 3.2 配置文件

```nginx
worker_processes 4;#工作进程数

events {
	worker_connections 1024;#单个工作进程可以允许同时建立外部连接的数量
}

#设定http服务器，利用它的反向代理功能提供负载均衡支持
http {
    #负载均衡配置
	upstream test {
        #upstream的负载均衡，weight是权重，可以根据机器配置定义权重。
        #weigth参数表示权值，权值越高被分配到的几率越大。
		server www.baidu.com weight=2;
		server www.163.com weight=1;
		server www.example7.com weight=1;
	}

	server {
		listen 8888;
		server_name localhost;
		
		client_max_body_size 100m;
		
        #反向代理
		location / {
#			root /usr/local/nginx/html/;
#			proxy_pass http://172.20.106.204;
			proxy_pass http://test;
			
#			proxy_redirect   off;
			proxy_set_header Host             www.example7.com;
#			proxy_set_header X-Real-IP        $remote_addr;
#			proxy_set_header X-Forwarded-For  $proxy_add_x_forwarded_for;
		}

		location /images/ {
			root /usr/local/nginx/;#访问/usr/local/nginx/images/
		}
		
		location ~ \.(mp3|mp4) {
			root /usr/local/nginx/media/;#访问/usr/local/nginx/media/
		}	
	}
}
```

### 4.HTTP和Restful

#### 4.1 Http

![http和https](./images/http和https.png?raw=true)

- HTTP协议，即超文本传输协议（Hypertext transfer protocol）。是一种详细规定了浏览器和万维网（WWW = World Wide Web）服务器之间互相通信的规则，通过因特网传送万维网文档的数据传送协议，可以传输文本，图片，视频等。

- HTTP协议作为TCP/IP模型中应用层的协议也不例外。HTTP 协议通常承载于TCP协议之 上，有时也承载于TLS或SSL协议层之上，这个时候，就成了我们常说的HTTPS。

- HTTP默认的端口号为 80，HTTPS的端口号为443。

- HTTP0.9和1.0使用非持续连接：限制每次连接只处理一个请求，服务器处理完客户的请求，并收到客户的应答后，即断开连接。HTTP1.1使用持续连接：不必为每个web对象创建一个新的连接，一个连接可以传送多个对象，采用这种方式可以节省传输时间。

- HTTP是媒体独立的：这意味着，只要客户端和服务器知道如何处理的数据内容，任何类型的数据都可以通过HTTP发 送。客户端以及服务器指定使用适合的MIME-type内容类型。 

- HTTP是无状态：HTTP协议是无状态协议。无状态是指协议对于事务处理没有记忆能力。缺少状态意味着如果后续处 理需要前面的信息，则它必须重传，这样可能导致每次连接传送的数据量增大。另一方面，在服务器不需要先前信息时它的应答就较快。

- HTTP1.0定义了三种请求方法: GET、POST和HEAD方法。

- HTTP1.1新增了六种请求方法: OPTIONS、PUT、PATCH、DELETE、TRACE和CONNECT方法。

  |  方法   |                             描述                             |
  | :-----: | :----------------------------------------------------------: |
  |   GET   |             请求指定的页面信息，并返回实体主体。             |
  |  HEAD   | 类似于GET请求，只不过返回的响应中没有具体的内容，用于获取报头。 |
  |  POST   | 向指定资源提交数据进行处理请求（例如提交表单或者上传文件）。 数据被包含在请求体中。POST请求可能会导致新的资源的建立和/或已有资源的修改。 |
  |   PUT   |       从客户端向服务器传送的数据取代指定的文档的内容。       |
  | DELETE  |                  请求服务器删除指定的页面。                  |
  | CONNECT |   HTTP/1.1协议中预留给能够将连接改为管道方式的代理服务器。   |
  | OPTIONS |                 允许客户端查看服务器的性能。                 |
  |  TRACE  |          回显服务器收到的请求，主要用于测试或诊断。          |
  |  PATCH  |      是对 PUT 方法的补充，用来对已知资源进行局部更新。       |


- HTTP响应头信息

  |      应答头      |                             说明                             |
  | :--------------: | :----------------------------------------------------------: |
  |      Allow       |          服务器支持哪些请求方法（如GET、POST等）。           |
  | Content-Encoding | 文档的编码（Encode）方法。只有在解码之后才可以得到Content-Type头指定的内容类型。利用gzip压缩文档能够显著地减少HTML文档的下载时间。 |
  |  Content-Length  |                        表示内容长度。                        |
  |   Content-Type   | 表示后面的文档属于什么MIME类型。Servlet默认为text/plain，但通常需要显式 地指定为text/html。 |
  |     Location     |                表示客户应当到哪里去提取文档。                |
  |    Set-Cookie    |                   设置和页面关联的Cookie。                   |

#### 4.2 Restful

- REST全称是Representational State Transfer，中文意思是表述（编者注：通常译为表征）性状态转移。 它首次出现在2000年Roy Fielding的博士论文中，Roy Fielding是HTTP规范的主要编写者之一。 他在论文中提到："我这篇文章的写作目的，就是想在符合架构原理的前提下，理解和评估以网络为基础的应用软件的架构设计，得到一个功能强、性能好、适宜通信的架构。REST指的是一组架构约束条件和原则。" 如果一个架构符合REST的约束条件和原则，我们就称它为RESTful架构。
- REST本身并没有创造新的技术、组件或服务，而隐藏在RESTful背后的理念就是使用Web的现有特征和能力，更好地使用现有Web标准中的一些准则和约束。虽然REST本身受Web技术的影响很深，但是理论上REST架构风格并不是绑定在HTTP上，只不过目前HTTP是唯一与REST相关的实例。所以我们这里描述的REST也是通过HTTP实现的REST。
- RESTful架构应该遵循统一接口原则，统一接口包含了一组受限的预定义的操作，不论什么样的资源，都是通过使用相同的接口进行资源的访问。接口应该使用标准的HTTP方法如GET，PUT和POST，并遵循这些方法的语义。
- 如果按照HTTP方法的语义来暴露资源，那么接口将会拥有安全性和幂等性的特性，例如GET和HEAD请求都是安全的，无论请求多少次，都不会改变服务器状态。而GET、HEAD、PUT和DELETE请求都是幂等的，无论对资源操作多少次，结果总是一样的，后面的请求并不会产生比第一次更多的影响。

### 5.Redis操作和编程

#### 5.1 Redis操作

- REmote DIctionary Server（Redis）是一个由Salvatore Sanfilippo写的key-value存储系统。Redis是一个开源的使用ANSI C语言编写、遵守 BSD 协议、支持网络、可基于内存亦可持久化的日志型、Key-Value数据库，并提供多种语言的API。它通常被称为数据结构服务器，因为值（value）可以是字符串(String)、哈希(Hash)、列表(list)、集合(sets)和有序集合(sorted sets)等类型。

- 编译安装

  ```shell
  # 下载源码
  wget http://download.redis.io/releases/redis-6.0.0.tar.gz
  tar zxvf redis-6.0.0.tar.gz
  # 编译
  cd redis-6.0.0/
  make -j 8
  # 安装（默认安装到/usr/local/bin/目录）
  sudo make install
  ```

- 常用命令

  - 查看版本

    ```shell
    redis-server -v
    ```

  - 启动

    ```shell
    redis-server redis.conf
    ```

- redis-cli

  - 连接

    ```shell
    redis-cli -h 127.0.0.1 -p 6379 -a 123456
    ```

  - 停止服务

    ```shell
    redis-cli -h 127.0.0.1 -p 6379 shutdown
    ```

  - 内部命令

    |                 命令                 |                             说明                             |
    | :----------------------------------: | :----------------------------------------------------------: |
    |               DEL key                |               该命令用于在key存在时删除 key。                |
    |               DUMP key               |             序列化给定key，并返回被序列化的值。              |
    |              EXISTS key              |                    检查给定key是否存在。                     |
    |          EXPIRE key seconds          |               为给定key设置过期时间，以秒计。                |
    |        EXPIREAT key timestamp        | EXPIREAT的作用和EXPIRE类似，都用于为key设置过期时间。不同在于EXPIREAT命令接受的时间参数是UNIX时间戳(unix timestamp)。 |
    |       PEXPIRE key milliseconds       |                 设置key的过期时间以毫秒计。                  |
    | PEXPIREAT key milliseconds-timestamp |      设置key过期时间的时间戳(unix timestamp) 以毫秒计。      |
    |             KEYS pattern             |            查找所有符合给定模式( pattern)的key。             |
    |             MOVE key db              |         将当前数据库的key移动到给定的数据库db当中。          |
    |             PERSIST key              |              移除key的过期时间，key将持久保持。              |
    |               PTTL key               |            以毫秒为单位返回key的剩余的过期时间。             |
    |               TTL key                |  以秒为单位，返回给定key的剩余生存时间(TTL, time to live)。  |
    |              RANDOMKEY               |               从当前数据库中随机返回一个key。                |
    |          RENAME key newkey           |                       修改key的名称。                        |
    |         RENAMENX key newkey          |           仅当newkey不存在时，将key改名为newkey。            |
    |               TYPE key               |                  返回key所储存的值的类型。                   |

- redis基本数据结构

  ![redis基本数据结构](./images/redis基本数据结构.png?raw=true)

#### 5.2 hiredis编程

- 编译安装hiredis

  ```shell
  cd deps/hiredis
  make -j 8
  sudo make install
  sudo ldconfig
  ```

- [例子代码](./code/redis/hiredis.c)

  <details>
  <summary>例子代码</summary>

  ```C
  // 连接Redis服务
  redisContext *context = redisConnect("127.0.0.1", 6379);
  if (context == NULL || context->err)
  {
      if (context)
      {
          printf("%s\n", context->errstr);
      }
      else
      {
          printf("redisConnect error\n");
      }
      exit(EXIT_FAILURE);
  }
  printf("-----------------connect success--------------------\n");
  
  // REDIS_REPLY_STRING == 1 :返回值是字符串,字符串储存在redis->str当中,字符串长度为redis->len。
  // REDIS_REPLY_ARRAY == 2 :返回值是数组,数组大小存在redis->elements里面,数组值存储在redis->element[i]里面。数组里面存储的是指向redisReply的指针,数组里面的返回值可以通过redis->element[i]->str来访问,数组的结果里全是type==REDIS_REPLY_STRING的redisReply对象指针。
  // REDIS_REPLY_INTEGER == 3 :返回值为整数long long。
  // REDIS_REPLY_NIL == 4 :返回值为空表示执行结果为空。
  // REDIS_REPLY_STATUS == 5 :返回命令执行的状态,比如set foo bar返回的状态为OK,存储在str当中reply->str == "OK"。
  // REDIS_REPLY_ERROR == 6 :命令执行错误,错误信息存放在reply->str当中。
  
  // 授权
  redisReply *reply = redisCommand(context, "auth gongluck");
  printf("type : %d\n", reply->type);
  if (reply->type == REDIS_REPLY_STATUS)
  {
      printf("auth ok\n");
  }
  else if (reply->type == REDIS_REPLY_ERROR)
  {
      printf("auth err : %s\n", reply->str);
  }
  freeReplyObject(reply);
  
  // Set Key Value
  char *key = "str";
  char *val = "Hello World";
  reply = redisCommand(context, "SET %s %s", key, val);
  printf("type : %d\n", reply->type);
  if (reply->type == REDIS_REPLY_STATUS)
  {
      printf("SET %s %s\n", key, val);
  }
  freeReplyObject(reply);
  
  // GET Key
  reply = redisCommand(context, "GET %s", key);
  if (reply->type == REDIS_REPLY_STRING)
  {
      printf("GET str %s\n", reply->str);
      printf("GET len %ld\n", reply->len);
  }
  freeReplyObject(reply);
  
  // APPEND key value
  char *append = " I am your GOD";
  reply = redisCommand(context, "APPEND %s %s", key, append);
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("APPEND %s %s \n", key, append);
  }
  freeReplyObject(reply);
  
  reply = redisCommand(context, "GET %s", key);
  if (reply->type == REDIS_REPLY_STRING)
  {
      printf("GET %s\n", reply->str);
  }
  freeReplyObject(reply);
  
  // INCR key
  reply = redisCommand(context, "INCR counter");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("INCR counter %lld\n", reply->integer);
  }
  freeReplyObject(reply);
  reply = redisCommand(context, "INCR counter");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("INCR counter %lld\n", reply->integer);
  }
  freeReplyObject(reply);
  
  // DECR key
  reply = redisCommand(context, "DECR counter");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("DECR counter %lld\n", reply->integer);
  }
  freeReplyObject(reply);
  reply = redisCommand(context, "DECR counter");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("DECR counter %lld\n", reply->integer);
  }
  freeReplyObject(reply);
  
  // DECRBY key decrement
  reply = redisCommand(context, "DECRBY counter 5");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("DECRBY counter %lld\n", reply->integer);
  }
  freeReplyObject(reply);
  reply = redisCommand(context, "DECRBY counter 5");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("DECRBY counter %lld\n", reply->integer);
  }
  freeReplyObject(reply);
  
  // INCRBY key increment
  reply = redisCommand(context, "INCRBY counter 5");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("INCRBY counter %lld\n", reply->integer);
  }
  freeReplyObject(reply);
  
  reply = redisCommand(context, "INCRBY counter 5");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("INCRBY counter %lld\n", reply->integer);
  }
  freeReplyObject(reply);
  
  // GETRANGE key start end
  reply = redisCommand(context, "GETRANGE str 0 5");
  if (reply->type == REDIS_REPLY_STRING)
  {
      printf("GETRANGE %s %s\n", key, reply->str);
  }
  freeReplyObject(reply);
  
  // GETSET key value
  reply = redisCommand(context, "GETSET %s %s", key, val);
  if (reply->type == REDIS_REPLY_STRING)
  {
      printf("GETSET %s %s\n", key, reply->str);
  }
  freeReplyObject(reply);
  
  /*INCRBYFLOAT key increment*/
  reply = redisCommand(context, "INCRBYFLOAT f 2.1");
  if (reply->type == REDIS_REPLY_STRING)
  {
      printf("INCRBYFLOAT counter %s\n", reply->str);
  }
  freeReplyObject(reply);
  
  /*MSET key value [key value ...]*/
  reply = redisCommand(context, "MSET k1 hello k2 world k3 good");
  if (reply->type == REDIS_REPLY_STATUS)
  {
      printf("MSET k1 hello k2 world k3 good\n");
  }
  freeReplyObject(reply);
  
  /*MGET key [key ...]*/
  reply = redisCommand(context, "MGET k1 k2 k3");
  if (reply->type == REDIS_REPLY_ARRAY)
  {
      printf("MGET k1  k2  k3 \n");
      redisReply **pReply = reply->element;
      int i = 0;
      size_t len = reply->elements;
      //hello world good
      for (; i < len; ++i)
      {
          printf("%s ", pReply[i]->str);
      }
      printf("\n");
  }
  freeReplyObject(reply);
  
  /*STRLEN key*/
  reply = redisCommand(context, "STRLEN str");
  if (reply->type == REDIS_REPLY_INTEGER)
  {
      printf("STRLEN str %lld \n", reply->integer);
  }
  freeReplyObject(reply);
  
  /*SETEX key seconds value*/
  reply = redisCommand(context, "SETEX s 10 10seconds");
  if (reply->type == REDIS_REPLY_STATUS)
  {
      printf("SETEX s 10 10seconds\n");
      freeReplyObject(reply);
      int i = 0;
      while (i++ < 12)
      {
          reply = redisCommand(context, "GET s");
          if (reply->type == REDIS_REPLY_STRING)
          {
              printf("%d s %s\n", i, reply->str);
          }
          else if (reply->type == REDIS_REPLY_NIL)
          {
              printf("%d s nil\n", i);
          }
          freeReplyObject(reply);
          sleep(1);
      }
  }
  
  redisFree(context);
  return EXIT_SUCCESS;
  ```
  </details>
  

### 6.MongoDB操作和编程

#### 6.1 MongoDB操作

- MongoDB概念

  | SQL术语/概念 | MongoDB术语/概念 |              解释/说明               |
  | :----------: | :--------------: | :----------------------------------: |
  |   database   |     database     |                数据库                |
  |    table     |    collection    |            数据库表/集合             |
  |     row      |     document     |           数据记录行/文档            |
  |    column    |      field       |             数据字段/域              |
  |    index     |      index       |                 索引                 |
  | table joins  |        -         |        表连接，MongoDB不支持         |
  | primary key  |   primary key    | 主键，MongoDB自动将_id字段设置为主键 |

- 安装MongoDB

  ```shell
  wget https://fastdl.mongodb.org/linux/mongodb-linux-x86_64-ubuntu1804-4.4.1.tgz
  tar -zxvf mongodb-linux-x86_64-ubuntu1804-4.4.1.tgz 
  sudo mv mongodb-linux-x86_64-ubuntu1804-4.4.1 /usr/local/mongodb
  ```

- 启动MongoDB

  - MongoDB的数据存储在data目录的db目录下，但是这个目录在安装过程不会自动创建，所以需要手动创建data目录，并在data目录中创建db目录。

  - 可以在命令行中执行mongo安装目录中的bin目录执行mongod命令来启动mongdb服务。如果数据库目录不是/data/db，可以通过--dbpath来指定。

    ```shell
    /usr/local/mongodb/bin/mongod --dbpath=/data/db --bind_ip=0.0.0.0
    ```

- MongoDB后台管理

  - MongoDB Shell是MongoDB自带的交互式Javascript shell，用来对MongoDB进行操作和管理的交互式环境。它默认会链接到test文档（数据库）

    ```shell
    /usr/local/mongodb/bin/mongo
    ```

  - 显示所有数据的列表

    ```sql
    show dbs
    ```

  - 显示当前数据库对象或集合

    ```sql
    db
    ```

  - （创建）连接到一个指定的数据库。

    ```sql
    use gongluck
    ```

  - 插入数据

    ```sql
    db.gongluck.insert({"name":"gongluck"})
    ```

  - 删除当前数据库

    ```sql
    db.dropDatabase()
    ```

  - 创建集合

    ```sql
    db.createCollection("test")
    ```

  - 查看已有集合

    ```sql
    show collections
    ```

  - 删除集合

    ```sql
    db.test.drop()
    ```

  - 插入文档

    ```sql
    db.test.insert({"name":"gongluck","socre":100})
    ```

  - 查看已插入文档

    ```sql
    db.test.find().pretty()
    ```

  - 更新文档

    ```sql
    db.test.update({"name":"gongluck"}, {$set:{"name":"updated"}}, {multi:true})
    db.test.save({"_id":ObjectId("5fafd5407f51c6334abca881"),"name":"gongluck","socre":100})
    ```

  - 删除文档

    ```sql
    db.test.remove({"name":"gongluck"})
    db.test.remove()
    ```

  - 创建索引

    ```sql
    # 1:升序,-1:降序
    db.test.createIndex({"name":1,"score":-1})
    ```

#### 6.2 MongoDB编程

- 编译mongo-c-driver

  ```shell
  wget https://github.com/mongodb/mongo-c-driver/releases/download/1.17.2/mongo-c-driver-1.17.2.tar.gz
  tar -zxvf mongo-c-driver-1.17.2.tar.gz
  mongo-c-driver-1.17.2/
  mkdir cmake-build
  cd cmake-build
  cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
  cmake --build .
  sudo cmake --build . --target install
  ```

- [例子代码](./code/mongodb/mongodb.c)

  <details>
  <summary>例子代码</summary>
  
  ```C
  //gcc mongodb.c -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0/ -lmongoc-1.0 -lbson-1.0
  
  #include <bson.h>
  #include <mongoc.h>
  
  int main()
  {
      mongoc_client_t *client;
      mongoc_collection_t *collection;
      bson_t *insert;
      bson_error_t error;
  
      //初始化libmongoc驱动
      mongoc_init();
      
      //创建连接对象
      client = mongoc_client_new("mongodb://localhost:27017");
      
      //获取指定数据库和集合
      collection = mongoc_client_get_collection(client, "gongluck", "test");
      
      //字段为hello，值为world字符串
      insert = BCON_NEW("hello", BCON_UTF8("world"));
      
      //插入文档
      if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, insert, NULL, &error))
      {
          fprintf(stderr, "%s\n", error.message);
      }
      
      bson_destroy(insert);
      
      mongoc_collection_destroy(collection); //释放表对象
      mongoc_client_destroy(client);         //释放连接对象
      mongoc_cleanup();                      //释放libmongoc驱动
      return 0;
  }
  ```
  </details>

## 三、基础组件开发专栏

### 1.线程池原理与实现

#### 1.1 线程池工作流程

![线程池工作流程](./images/线程池工作流程.png?raw=true)

#### 1.2 线程池实现

- [线程池实现代码](./code/threadpool/threadpool.c)

  <details>
  <summary>线程池实现代码</summary>
  
  ```C
  #include <pthread.h>
  
  // 添加队列节点
  #define LL_ADD(item, list) \
      do                     \
      {                      \
          item->prev = NULL; \
          item->next = list; \
          list = item;       \
      } while (0)
  
  // 移除队列节点
  #define LL_REMOVE(item, list)              \
      do                                     \
      {                                      \
          if (item->prev != NULL)            \
              item->prev->next = item->next; \
          if (item->next != NULL)            \
              item->next->prev = item->prev; \
          if (list == item)                  \
              list = item->next;             \
          item->prev = item->next = NULL;    \
      } while (0)
  
  // 工作线程
  typedef struct WORKER
  {
      pthread_t thread;
      int terminate;
      struct WORKQUEUE *workqueue;
      struct WORKER *prev;
      struct WORKER *next;
  } Worker;
  
  // 工作任务
  typedef struct JOB
  {
      void (*job_function)(struct JOB *job);
      void *user_data;
      struct JOB *prev;
      struct JOB *next;
  } Job;
  
  // 工作调度
  typedef struct WORKQUEUE
  {
      struct WORKER *workers;
      struct JOB *waiting_jobs;
      pthread_mutex_t jobs_mtx;
      pthread_cond_t jobs_cond;
  } WorkQueue;
  
  typedef WorkQueue ThreadPool;
  
  // 工作线程回调函数
  static void *WorkerThread(void *ptr)
  {
      Worker *worker = (Worker *)ptr;
  
      while (1)
      {
          pthread_mutex_lock(&worker->workqueue->jobs_mtx);
  
          while (worker->workqueue->waiting_jobs == NULL)
          {
              if (worker->terminate)
                  break;
              pthread_cond_wait(&worker->workqueue->jobs_cond, &worker->workqueue->jobs_mtx);
          }
  
          if (worker->terminate)
          {
              pthread_mutex_unlock(&worker->workqueue->jobs_mtx);
              break;
          }
  
          Job *job = worker->workqueue->waiting_jobs;
          if (job != NULL)
          {
              LL_REMOVE(job, worker->workqueue->waiting_jobs);
          }
  
          pthread_mutex_unlock(&worker->workqueue->jobs_mtx);
  
          if (job == NULL)
              continue;
  
          job->job_function(job);
      }
  
      free(worker);
      pthread_exit(NULL);
  }
  
  // 创建线程池
  int ThreadPoolCreate(ThreadPool *workqueue, int numWorkers)
  {
      if (numWorkers < 1)
          numWorkers = 1;
      memset(workqueue, 0, sizeof(ThreadPool));
  
      pthread_mutex_init(&workqueue->jobs_mtx, NULL);
      pthread_cond_init(&workqueue->jobs_cond, NULL);
  
      for (int i = 0; i < numWorkers; i++)
      {
          Worker *worker = (Worker *)malloc(sizeof(Worker));
          if (worker == NULL)
          {
              perror("malloc");
              return 1;
          }
  
          memset(worker, 0, sizeof(Worker));
          worker->workqueue = workqueue;
  
          int ret = pthread_create(&worker->thread, NULL, WorkerThread, (void *)worker);
          if (ret)
          {
              perror("pthread_create");
              free(worker);
              return 1;
          }
  
          LL_ADD(worker, worker->workqueue->workers);
      }
  
      return 0;
  }
  
  // 终止线程池
  void ThreadPoolShutdown(ThreadPool *workqueue)
  {
      for (Worker *worker = workqueue->workers; worker != NULL; worker = worker->next)
      {
          worker->terminate = 1;
      }
  
      pthread_mutex_lock(&workqueue->jobs_mtx);
  
      workqueue->workers = NULL;
      workqueue->waiting_jobs = NULL;
  
      pthread_cond_broadcast(&workqueue->jobs_cond);
  
      pthread_mutex_unlock(&workqueue->jobs_mtx);
  }
  
  // 添加任务
  void ThreadPoolQueue(ThreadPool *workqueue, Job *job)
  {
      pthread_mutex_lock(&workqueue->jobs_mtx);
  
      LL_ADD(job, workqueue->waiting_jobs);
  
      pthread_cond_signal(&workqueue->jobs_cond);
      pthread_mutex_unlock(&workqueue->jobs_mtx);
  }
  ```
  </details>

### 2.CAS和无锁队列

#### 2.1 CAS

- 比较并交换（compare and swap，CAS），是**原⼦操作**的⼀种，可⽤于在多线程编程中实现不被打断的数据交换操作，从而避免多线程同时改写某⼀数据时由于执行顺序不确定性以及中断的不可预知性产⽣的数据不一致问题有了CAS，我们就可以用它来实现各种无锁（lock free）的数据结构。

- 该操作通过将内存中的值与指定数据进行比较，当数值⼀样时将内存中的数据替换为新的值。

  ```C
  int compare_and_swap(int *reg, int oldval, int newval)
  {
      int old_ref_val = *reg;
      if(old_reg_val == oldval)//compare
          *reg = newval;//swap
      return old_reg_val;
  }
  ```

- gcc/g++中的CAS

  ```C
  bool __sync_bool_compare_and_swap(type *ptr, type oldval type newval, ...);
  type __sync_val_compare_and_swap(type *ptr, type oldval type newval, ...);
  ```

- Windows的CAS

  ```C
  InterlockedCompareExchange(__inout LONG volatile  *Target, __in LONG Exchange, __in LONG Comperand);
  ```

- C++11标准库的CAS

  ```C
  template< class T >
  bool atomic_compare_exchange_weak(std::atomic<T>* obj, T* expected, T desired);
  template< class T >
  bool atomic_compare_exchange_weak(volatile std::atomic<T>* obj, T* expected, T desired );
  ```

#### 2.2 无锁队列

- [无锁队列代码](./code/cas/cas.h)

  <details>
  <summary>无锁队列代码</summary>
  
  ```C
  /*
   * @Author: gongluck 
   * @Date: 2020-11-16 16:02:56 
   * @Last Modified by:   gongluck 
   * @Last Modified time: 2020-11-16 16:02:56 
   */
  
  template <typename ElemType>
  class Queue
  {
  public:
      Queue();
      ~Queue();
  
  public:
      void push(ElemType elem);
      bool pop();
      void show();
  
  private:
      struct _qNode
      {
          _qNode() : _next(nullptr) {}
          _qNode(ElemType elem) : _elem(elem), _next(nullptr) {}
          ElemType _elem;
          struct _qNode *_next;
      };
  
  private:
      struct _qNode *_head;
      struct _qNode *_tail;
  };
  
  template <typename ElemType>
  Queue<ElemType>::Queue()
  {
      _head = _tail = new _qNode();
  }
  
  template <typename ElemType>
  Queue<ElemType>::~Queue()
  {
      while (_head != nullptr)
      {
          struct _qNode *tempNode = _head;
          _head = _head->_next;
          delete tempNode;
      }
  }
  
  template <typename ElemType>
  void Queue<ElemType>::push(ElemType elem)
  {
      struct _qNode *newNode = new struct _qNode(elem);
      struct _qNode *oldp = _tail;
      while (!__sync_bool_compare_and_swap(&_tail->_next, nullptr, newNode))
          ;
      __sync_bool_compare_and_swap(&_tail, oldp, newNode);
  }
  
  template <typename ElemType>
  bool Queue<ElemType>::pop()
  {
      struct _qNode *p;
      do
      {
          p = _head;
          if (p->_next == nullptr)
              return false;
      } while (!__sync_bool_compare_and_swap(&_head, p, p->_next));
      delete p;
      return true;
  }
  ```
  </details>
  

### 3.内存池

#### 3.1 Nginx内存池结构

- Nginx内存池结构

  ![Nginx内存池结构](./images/Nginx内存池结构.jpg?raw=true)

- nginx对内存的管理分为大内存与小内存，当某一个申请的内存大于某一个值时，就需要从大内存中分配空间，否则从小内存中分配空间。

- nginx中的内存池是在创建的时候就设定好了大小，在以后分配小块内存的时候，如果内存不够，则是重新创建一块内存串到内存池中，而不是将原有的内存池进行扩张。当要分配大块内存是，则是在内存池外面再分配空间进行管理的，称为大块内存池。

- Nginx内存池中大内存块和小内存块的分配与释放是不一样的。使用内存池时，可以使用ngx_palloc进行分配，使用ngx_pfree释放。对于大内存，这样做是没有问题的，而对于小内存就不一样了，分配的小内存，不会进行释放。

#### 3.2 [Nginx内存池的实现](./code/mmpool/ngx_palloc.c)

<details>
<summary>Nginx内存池的实现</summary>


```C
ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    if (p == NULL) {
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);//初始状态：last指向ngx_pool_t结构体之后数据取起始位置
    p->d.end = (u_char *) p + size;//end指向分配的整个size大小的内存的末尾
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;

    return p;
}


void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    //首先调用所有的数据清理函数
    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#if (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    for (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        if (n == NULL) {
            break;
        }
    }

#endif

    //释放所有的大块内存
    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    //最后释放所有内存池中的内存块
    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        if (n == NULL) {
            break;
        }
    }
}


void
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    //释放所有大块内存
    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    //重置所有小块内存区
    for (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + sizeof(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = NULL;
    pool->large = NULL;
}


void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#if !(NGX_DEBUG_PALLOC)
    if (size <= pool->max) {
        return ngx_palloc_small(pool, size, 1);
    }
#endif

    return ngx_palloc_large(pool, size);
}


void *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#if !(NGX_DEBUG_PALLOC)
    if (size <= pool->max) {
        return ngx_palloc_small(pool, size, 0);
    }
#endif

    return ngx_palloc_large(pool, size);
}


static ngx_inline void *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    do {
        m = p->d.last;

        if (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        if ((size_t) (p->d.end - m) >= size) {//如果在当前内存块有效范围内，进行内存指针的移动
            p->d.last = m + size;

            return m;
        }

        p = p->d.next;//如果当前内存块有效容量不够分配，则移动到下一个内存块进行分配

    } while (p);

    return ngx_palloc_block(pool, size);
}


static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);//计算内存池第一个内存块的大小

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);//分配和第一个内存块同样大小的内存块
    if (m == NULL) {
        return NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;//设置新内存块的end
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(ngx_pool_data_t);//将指针m移动到d后面的一个位置，作为起始位置
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;//设置新内存块的last，即申请使用size大小的内存

    //这里的循环用来找最后一个链表节点，这里failed用来控制循环的长度，如果分配失败次数达到5次，就忽略，不需要每次都从头找起
    for (p = pool->current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    return m;
}


static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    //直接在系统堆中分配一块空间
    p = ngx_alloc(size, pool->log);
    if (p == NULL) {
        return NULL;
    }

    n = 0;
    //查找到一个空的large区，如果有，则将刚才分配的空间交由它管理
    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

    //为了提高效率， 如果在三次内没有找到空的large结构体，则创建一个
    large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}


void *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    if (p == NULL) {
        return NULL;
    }

    large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

    //只检查是否是大内存块，如果是大内存块则释放
    for (l = pool->large; l; l = l->next) {
        if (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = NULL;

            return NGX_OK;
        }
    }

    return NGX_DECLINED;
}
```
</details>

## 四、开源框架专题

### 1.字符编码和压缩

#### 1.1 字符编码

- ASCII（American Standard Code for Information Interchange），128个字符，用7位二进制表示（00000000-01111111即0x00-0x7F）;EASCII（Extended ASCII），256个字符，用8位二进制表示（00000000-11111111即0x00-0xFF）。当计算机传到了欧洲，国际标准化组织在ASCII的基础上进行了扩展，形成了ISO-8859标准，跟EASCII类似，兼容ASCII，在高128个码位上有所区别。但是由于欧洲的语言环境十分复杂，所以根据各地区的语言又形成了很多子标准，ISO-8859-1、ISO-8859- 2、ISO-8859-3、……、ISO-8859-16。

- 双字节编码可以是变长的，也就是说同一个编码里面有些字符是单字节表示，有些字符是双字节表示。这样做的好处是，一方面可以兼容ASCII，另一方面可以节省存储容量，代价就是会损失一部分码位。

  - GBK（Chinese Internal Code Specification汉字内码扩展规范）是GB2312的扩展（gbk编码能够用来同时表示繁体字和简体字），按理说都属于双字节编码，码位是一样的，根本谈不上扩展，但实际上是预留空间在起作用。
  - UNICODE字符集（国际标准字符集），它将世界各种语言的每个字符定义一个唯一的编码，以满足跨语言、跨平台的文本信息转换。有多个编码方式，分别是UTF-8，UTF-16，UTF-32编码。

- UTF是Unicode Transformation Format的缩写，意思是“Unicode转换格式”，后面的数字表明**至少**使用多少个比特位（Bit）来存储字符。

  - UFT-8：一种变长的编码方案，使用1~6个字节来存储；

    ![Unicode码表对应utf8编码](./images/Unicode码表对应utf8编码.png?raw=true)

  - UFT-32：一种固定长度的编码方案，不管字符编号大小，直接存储Unicode编号，始终使用4个字节来存储；

  - UTF-16：介于UTF-8和UTF-32之间，使用2个或者4个字节来存储，长度既固定又可变。

    ![Unicode码表对应utf16编码](./images/Unicode码表对应utf16编码.png?raw=true)

  - UTF格式在文件中的**固定文件头**

    ![utf固定文件头](./images/utf固定文件头.png?raw=true)

#### 1.2 libiconv字符编码转换库

- [libiconv例子](./code/convert/iconv.c)

  <details>
  <summary>libiconv例子</summary>
  
  ```C
  /*
   * @Author: gongluck 
   * @Date: 2020-11-18 07:52:34 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-18 09:14:11
   */
  
  #include <stdio.h>
  #include <stdlib.h>
  #include <iconv.h>
  
  int main()
  {
      iconv_t cd = iconv_open("GBK", "UTF-8"); //UTF8 -> GBK
      if (cd == (iconv_t)-1)
      {
          printf("iconv_open failed.\n");
          exit(-1);
      }
  
      char str[] = "iconv例子";
      char *pstr = str;
      size_t len = sizeof(str);
      char out[100] = {0};
      char *pout = out;
      size_t leftlen = sizeof(out);
      while (pstr-str < sizeof(str))
      {
          pout = out;
          leftlen = sizeof(out);
          int n = iconv(cd, &pstr, &len, &pout, &leftlen);
          //printf("iconv return %d\n", n);
          if (pout != out)//有转换成功字符
          {
              printf("%.*s", (int)(sizeof(out)-leftlen), out);
          }
          else
          {
              break;
          }
          
          if(n != -1)
          {
              pout = out;
  			leftlen = sizeof(out);
  			n = iconv(cd, NULL, NULL, &pout, &leftlen);
  			if (pout != out) // 检测iconv内部是否还有缓存
  			{
  				printf("%.*s", (int)(sizeof(out)-leftlen), out);
  			}
  			break;
          }
      }
      printf("\n");
      iconv_close(cd);
  
      return 0;
  }
  ```
  </details>

#### 1.3 zlib

- 编译安装zlib

  ```shell
  #下载
  wget http://www.zlib.net/zlib-1.2.11.tar.gz
  #解压
  tar -zxvf zlib-1.2.11.tar.gz
  #进入目录
  cd zlib-1.2.11
  #配置
  ./configure
  #编译
  make -j 8
  #检查
  make check
  #安装
  sudo make install
  ```

- [zlib例子](./code/zlib/zlib.c)

  <details>
  <summary>zlib例子</summary>
  
  ```C
  /*
   * @Author: gongluck 
   * @Date: 2020-11-18 17:02:24 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-18 17:05:37
   */
  
  // gcc zlib.c -lz
  // ./a.out < zlib.c > out
  // ./a.out -d < out > zlib.c
  
  #include <stdio.h>
  #include <string.h>
  #include <assert.h>
  #include "zlib.h"
  
  #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
  #include <fcntl.h>
  #include <io.h>
  #define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
  #else
  #define SET_BINARY_MODE(file)
  #endif
  
  #define CHUNK 1024
  
  /* Compress from file source to file dest until EOF on source.
     def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
     allocated for processing, Z_STREAM_ERROR if an invalid compression
     level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
     version of the library linked do not match, or Z_ERRNO if there is
     an error reading or writing the files. */
  int def(FILE *source, FILE *dest, int level)
  {
      int ret, flush;
      unsigned have;
      z_stream strm;
      unsigned char in[CHUNK];
      unsigned char out[CHUNK];
  
      /* allocate deflate state */
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      ret = deflateInit(&strm, level);
      if (ret != Z_OK)
          return ret;
  
      /* compress until end of file */
      do
      {
          strm.avail_in = fread(in, 1, CHUNK, source);
          if (ferror(source))
          {
              (void)deflateEnd(&strm);
              return Z_ERRNO;
          }
          flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
          strm.next_in = in;
  
          /* run deflate() on input until output buffer not full, finish
             compression if all of source has been read in */
          do
          {
              strm.avail_out = CHUNK;
              strm.next_out = out;
              ret = deflate(&strm, flush);   /* no bad return value */
              assert(ret != Z_STREAM_ERROR); /* state not clobbered */
              have = CHUNK - strm.avail_out;
              if (fwrite(out, 1, have, dest) != have || ferror(dest))
              {
                  (void)deflateEnd(&strm);
                  return Z_ERRNO;
              }
          } while (strm.avail_out == 0);
          assert(strm.avail_in == 0); /* all input will be used */
  
          /* done when last data in file processed */
      } while (flush != Z_FINISH);
      assert(ret == Z_STREAM_END); /* stream will be complete */
  
      /* clean up and return */
      (void)deflateEnd(&strm);
      return Z_OK;
  }
  
  /* Decompress from file source to file dest until stream ends or EOF.
     inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
     allocated for processing, Z_DATA_ERROR if the deflate data is
     invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
     the version of the library linked do not match, or Z_ERRNO if there
     is an error reading or writing the files. */
  int inf(FILE *source, FILE *dest)
  {
      int ret;
      unsigned have;
      z_stream strm;
      unsigned char in[CHUNK];
      unsigned char out[CHUNK];
  
      /* allocate inflate state */
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      strm.avail_in = 0;
      strm.next_in = Z_NULL;
      ret = inflateInit(&strm);
      if (ret != Z_OK)
          return ret;
  
      /* decompress until deflate stream ends or end of file */
      do
      {
          strm.avail_in = fread(in, 1, CHUNK, source);
          if (ferror(source))
          {
              (void)inflateEnd(&strm);
              return Z_ERRNO;
          }
          if (strm.avail_in == 0)
              break;
          strm.next_in = in;
  
          /* run inflate() on input until output buffer not full */
          do
          {
              strm.avail_out = CHUNK;
              strm.next_out = out;
              ret = inflate(&strm, Z_NO_FLUSH);
              assert(ret != Z_STREAM_ERROR); /* state not clobbered */
              switch (ret)
              {
              case Z_NEED_DICT:
                  ret = Z_DATA_ERROR; /* and fall through */
              case Z_DATA_ERROR:
              case Z_MEM_ERROR:
                  (void)inflateEnd(&strm);
                  return ret;
              }
              have = CHUNK - strm.avail_out;
              if (fwrite(out, 1, have, dest) != have || ferror(dest))
              {
                  (void)inflateEnd(&strm);
                  return Z_ERRNO;
              }
          } while (strm.avail_out == 0);
  
          /* done when inflate() says it's done */
      } while (ret != Z_STREAM_END);
  
      /* clean up and return */
      (void)inflateEnd(&strm);
      return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
  }
  
  /* report a zlib or i/o error */
  void zerr(int ret)
  {
      fputs("zpipe: ", stderr);
      switch (ret)
      {
      case Z_ERRNO:
          if (ferror(stdin))
              fputs("error reading stdin\n", stderr);
          if (ferror(stdout))
              fputs("error writing stdout\n", stderr);
          break;
      case Z_STREAM_ERROR:
          fputs("invalid compression level\n", stderr);
          break;
      case Z_DATA_ERROR:
          fputs("invalid or incomplete deflate data\n", stderr);
          break;
      case Z_MEM_ERROR:
          fputs("out of memory\n", stderr);
          break;
      case Z_VERSION_ERROR:
          fputs("zlib version mismatch!\n", stderr);
      }
  }
  
  /* compress or decompress from stdin to stdout */
  int main(int argc, char **argv)
  {
      int ret;
  
      /* avoid end-of-line conversions */
      SET_BINARY_MODE(stdin);
      SET_BINARY_MODE(stdout);
  
      /* do compression if no arguments */
      if (argc == 1)
      {
          ret = def(stdin, stdout, Z_DEFAULT_COMPRESSION);
          if (ret != Z_OK)
              zerr(ret);
          return ret;
      }
  
      /* do decompression if -d specified */
      else if (argc == 2 && strcmp(argv[1], "-d") == 0)
      {
          ret = inf(stdin, stdout);
          if (ret != Z_OK)
              zerr(ret);
          return ret;
      }
  
      /* otherwise, report usage */
      else
      {
          fputs("zpipe usage: zpipe [-d] < source > dest\n", stderr);
          return 1;
      }
  }
  ```
  </details>

  

### 2.Json和Xml

#### 2.1 cJSON

- [cJSON例子](./code/json/cjson/test.c)

  <details>
  <summary>[cJSON例子](./code/json/cjson/test.c)</summary>

  ```C
  /*
   * @Author: gongluck 
   * @Date: 2020-11-18 21:27:47 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-18 21:41:36
   */

  // gcc *.c -lm

  #include <stdio.h>
  #include "cJSON.h"

  int main()
  {
      cJSON* root = cJSON_CreateObject();
      cJSON_AddBoolToObject(root, "bool", cJSON_False);
      cJSON_AddStringToObject(root, "str", "cJSON");
  
      char* str = cJSON_Print(root);
      printf("%s\n", str);
      cJSON_Delete(root);
      root = NULL;

      root = cJSON_Parse(str);
      cJSON* b = cJSON_GetObjectItem(root, "bool");
      if(b->type == cJSON_True || b->type == cJSON_False)
          printf("bool value : %d\n", b->type);
      cJSON* s = cJSON_GetObjectItem(root, "str");
      if(s->type == cJSON_String)
          printf("str value : %s\n", s->valuestring);
      cJSON_Delete(root);
      return 0;
  }
  ```
  </details>

#### 2.2 jsoncpp

- 编译安装

  ```shell
  # 下载
  git clone https://github.com/open-source-parsers/jsoncpp.git
  cd jsoncpp
  mkdir -p build/release
  cd build/release
  # 编译
  cmake -DCMAKE_BUILD_TYPE=release -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_INCLUDEDIR=include/jsoncpp -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" ../..
  make
  # 安装
  sudo make install
  ```

- [jsoncpp例子](./code/json/jsoncpp/test.cpp)

  <details>
  <summary>[jsoncpp例子](./code/json/cjson/test.c)</summary>
  
  ```C++
  /*
   * @Author: gongluck 
   * @Date: 2020-11-19 09:40:21 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-19 09:46:42
   */
  
  // g++ test.cpp -ljsoncpp
  
  #include <iostream>
  #include <string>
  #include <jsoncpp/json/json.h>
  
  void readJson()
  {
      std::string strValue = "{\"name\":\"json\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}]}";
  
      Json::Reader reader;
      Json::Value value;
  
      if (reader.parse(strValue, value))
      {
          std::string out = value["name"].asString();
          std::cout << out << std::endl;
          const Json::Value arrayObj = value["array"];
          for (unsigned int i = 0; i < arrayObj.size(); i++)
          {
              if (!arrayObj[i].isMember("cpp"))
                  continue;
              out = arrayObj[i]["cpp"].asString();
              std::cout << out;
              if (i != (arrayObj.size() - 1))
                  std::cout << std::endl;
          }
      }
  }
  
  void writeJson()
  {
      Json::Value root;
      Json::Value arrayObj;
      Json::Value item;
  
      item["cpp"] = "jsoncpp";
      item["java"] = "jsoninjava";
      item["php"] = "support";
      arrayObj.append(item);
  
      root["name"] = "json";
      root["array"] = arrayObj;
  
      root.toStyledString();
      std::string out = root.toStyledString();
      std::cout << out << std::endl;
  }
  
  int main(int argc, char **argv)
  {
      readJson();
      writeJson();
      return 0;
  }
  ```
  </details>
  

#### 2.3 TinyXML2

- 安装编译

  ```shell
  #下载 tinyxml2
  git clone https://github.com/leethomason/tinyxml2.git
  #进入相应目录并编译
  cd tinyxml2/
  cmake . 
  make -j 8
  #测试该版本的准确性
  ./xmltest
  #安装
  sudo make install
  ```

- [tinyxml2例子](./code/xml/tinyxml2/test.cpp)

  <details>
  <summary>[tinyxml2例子](./code/json/cjson/test.c)</summary>
  
  ```C++
  /*
   * @Author: gongluck 
   * @Date: 2020-11-19 17:11:24 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-19 17:27:23
   */
  
  // g++ test.cpp -ltinyxml2
  
  #include <iostream>
  #include "tinyxml2.h"
  
  int main(void)
  {
      const char *xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>   \
                          <note>                      \
                              <to>beijing</to>             \
                              <from>shenzhen</from>           \
                              <heading>Reminder</heading> \
                              <body>Don't forget the meeting!</body> \
                          </note>";
      tinyxml2::XMLDocument doc;
      doc.Parse(xml);
      std::cout << doc.ErrorID() << std::endl;
  
      // 1. 第一种刷新到本地
      FILE *fp = fopen("memory_1.xml", "wb");
      tinyxml2::XMLPrinter printer(fp);
      doc.Print(&printer); // 打印到文件
      fclose(fp);
  
      // 2. 第二种刷新到本地
      doc.SaveFile("memory_2.xml");
  
      return 0;
  }
  ```
  </details>
  

### 3.ProtoBuf

#### 3.1 序列化和反序列化

- 序列化：把对象转换为字节序列的过程称为对象。

  ![常用序列化协议比较](./images/常用序列化协议比较.png?raw=true)

  - TLV编码及其变体（tag, length和value的缩写）：比如Protobuf。
  - 文本流编码：比如XML/JSON。
  - 固定结构编码：基本原理是，协议约定了传输字段类型和字段含义，和TLV的方式类似，但是没有了tag和len，只有value，比如TCP/IP。
  - 内存dump：把内存中的数据直接输出，不做任何序列化操作。反序列化的时候，直接还原内存。

- 反序列化：把字节序列恢复为对象的过程称为对象的反序列化。

#### 3.2 编译安装ProtoBuf

```shell
#下载源码
wget https://github.com/protocolbuffers/protobuf/archive/v3.1.0.tar.gz
tar -zxvf v3.1.0.tar.gz
cd protobuf-3.1.0/
#编译
./autogen.sh
./configure
make -j 8
make check
sudo make install
# refresh shared library cache.
sudo ldconfig
```

#### 3.3 [ProtoBuf例子](./code/protobuf)

```protobuf
/*
 * @Author: gongluck 
 * @Date: 2020-11-20 18:16:05 
 * @Last Modified by:   gongluck 
 * @Last Modified time: 2020-11-20 18:16:05 
 */
 
// protoc --cpp_out=./ addressbook.proto

syntax = "proto3";

package Test;

message Person
{
    string name = 1;
    int32 id = 2;
    string email = 3;

    enum PhoneType
    {
        MOBLIE = 0;//首成员必须为0
        HOME = 1;
        WORK = 2;
    }
    message PhoneNumber
    {
        string number = 1;
        PhoneType type = 2;
    }
    repeated PhoneNumber phones = 4;
}

message AddressBook
{
    repeated Person people = 1;
}
```

<details>
<summary>ProtoBuf例子</summary>

```C++
/*
 * @Author: gongluck 
 * @Date: 2020-11-20 17:23:22 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-20 18:14:44
 */

// g++ *.cc *.cpp `pkg-config --cflags --libs protobuf`

#include <iostream>
#include "addressbook.pb.h"

using namespace std;

int main()
{
    char buf[1024];
    int len;

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    Test::Person obj;
    obj.set_name("gongluck");
    obj.set_id(1);
    *obj.mutable_email() = "https://github.com.cnpmjs.org/gongluck/CVIP";
    len = obj.ByteSize();
    cout << "len = " << len << endl;
    obj.SerializeToArray(buf, len);

    Test::Person obj2;
    obj2.ParseFromArray(buf, len);
    cout << "name = " << obj2.name() << endl;
    cout << "id = " << obj2.id() << endl;
    cout << "email = " << obj2.email() << endl;

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
```
</details>

### 4.消息队列

#### 4.1 消息队列比较

![消息队列比较](./images/消息队列比较.png?raw=true)

#### 4.2 zmq编译安装

```shell
#安装编译依赖
sudo apt-get install libtool pkg-config build-essential autoconf automake
#安装加密库
git clone git://github.com/jedisct1/libsodium.git
cd libsodium
./autogen.sh -s
./configure && make -j 8 && make check
sudo make install
sudo ldconfig
cd ..
#下载
git clone https://github.com/zeromq/libzmq.git
cd libzmq
#查看tag
#git tag
#版本 获取指定的版本
git checkout v4.3.2
./autogen.sh
./configure && make -j 8 && make check
sudo make install
sudo ldconfig
cd ..
```

#### 4.3 [zmq例子](./code/messagequeue/zmq)

- [服务端](./code/messagequeue/zmq/zmqserver.c)

  <details>
  <summary>服务端代码</summary>
  
  ```C
  /*
   * @Author: gongluck 
   * @Date: 2020-11-24 09:49:11 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-24 09:51:26
   */
  
  // gcc zmqserver.c -lzmq
  
  #include <zmq.h>
  #include <stdio.h>
  #include <unistd.h>
  #include <string.h>
  #include <assert.h>
  
  int main(void)
  {
      // Socket to talk to clients
      void *context = zmq_ctx_new();
      // 与客户端通信的套接字
      void *responder = zmq_socket(context, ZMQ_REP);
      int rc = zmq_bind(responder, "tcp://*:5555");
      assert(rc == 0);
  
      while (1)
      {
          // 等待客户端请求
          char buffer[10];
          zmq_recv(responder, buffer, 10, 0);
          printf("收到 %.*s\n", 10, buffer);
          // 返回应答
          zmq_send(responder, "RECVED", 6, 0);
      }
      return 0;
  }
  ```
  </details>

- [客户端](./code/messagequeue/zmq/zmqclient.c)

  <details>
  <summary>客户端代码</summary>
  
  ```C
  /*
   * @Author: gongluck 
   * @Date: 2020-11-24 09:52:58 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-24 09:56:14
   */
  
  //gcc zmqclient.c -lzmq
  
  #include <zmq.h>
  #include <string.h>
  #include <stdio.h>
  #include <unistd.h>
  
  int main(void)
  {
      printf("Connecting to zmq server...\n");
      void *context = zmq_ctx_new();
      // 连接⾄服务端的套接字
      void *requester = zmq_socket(context, ZMQ_REQ);
      zmq_connect(requester, "tcp://localhost:5555");
  
      char buffer[10];
      for (int request_nbr = 0; request_nbr != 10; request_nbr++)
      {  
          printf("正在发送 %d...\n", request_nbr);
          zmq_send(requester, "Hello", 5, 0);
          zmq_recv(requester, buffer, 10, 0);
          printf("接收到 %.*s\n", 10, buffer);
      }
      zmq_close(requester);
      zmq_ctx_destroy(context);
      return 0;
  }
  ```
  </details>

### 5.OpenSSL

#### 5.1 OpenSSL编译安装

```shell
wget https://www.openssl.org/source/old/1.1.0/openssl-1.1.0l.tar.gz
tar xzvf OpenSSL-1.1.0l.tar.gz
cd OpenSSL-1.1.0l
./config --prefix=/usr/local/OpenSSL
make -j 8
sudo make install
```

#### 5.2 [OpenSSL例子](./code/openssl/test.c)

<details>
<summary>OpenSSL例子</summary>

```C
/*
 * @Author: gongluck 
 * @Date: 2020-11-24 18:44:16 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-24 18:53:20
 */

// gcc test.c -lssl -lcrypto

#include <stdio.h>
#include <string.h>
#include <openssl/lhash.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>

#define NAME_LENGTH 32

typedef struct _Person
{
    char name[NAME_LENGTH];
    int high;
    char otherInfo[NAME_LENGTH];
} Person;

// 自定义比较函数
static int person_cmp(const void *a, const void *b)
{
    char *namea = ((Person *)a)->name;
    char *nameb = ((Person *)b)->name;
    return strcmp(namea, nameb);
}

void print_value(void *a)
{
    Person *p = (Person *)a;
    printf("name: %s\n", p->name);
    printf("high: %d\n", p->high);
    printf("other info : %s\n", p->otherInfo);
}

int main()
{
    // hash
    OPENSSL_LHASH *h = lh_new(NULL, person_cmp);
    if (h == NULL)
    {
        printf("err.\n");
        return -1;
    }

    Person p1 = {"gongluck", 170, "xxxx"};
    Person p2 = {"ben", 175, "xxxx"};
    Person p3 = {"ken", 170, "xxxx"};
    Person p4 = {"dio", 170, "xxxx"};

    lh_insert(h, &p1);
    lh_insert(h, &p2);
    lh_insert(h, &p3);
    lh_insert(h, &p4);

    lh_doall(h, print_value);

    void *data = lh_retrieve(h, (const char *)"dio"); //person_cmp return 0
    if (data == NULL)
    {
        return -1;
    }

    print_value(data);
    lh_free(h);

    printf("\n------------------------------\n");

    //bio
    BIO *b = BIO_new(BIO_s_mem());
    int len = BIO_write(b, "OpenSSL", 4);
    len = BIO_printf(b, "%s", "gongluck");
    printf("len: %d\n", len);
    char *out = OPENSSL_malloc(len);
    len = BIO_read(b, out, len);
    printf("%s, len: %d\n", out, len);
    OPENSSL_free(out);
    BIO_free(b);

    printf("\nsocket bio\n");
    int sock = BIO_get_accept_socket("8899", 0);
    BIO *bsock = BIO_new_socket(sock, BIO_NOCLOSE);
    char *addr = NULL;
    int ret = BIO_accept(sock, &addr);
    BIO_set_fd(bsock, ret, BIO_NOCLOSE);
    while (1)
    {
        char out[128] = {0};
        BIO_read(bsock, out, 128);
        if (out[0] = 'q')
            break;
        printf("%s\n", out);
    }
    BIO_free(bsock);

    printf("\n------------------------------\n");

    //base64
    unsigned char in[30], base64[40], decode[30];
    EVP_ENCODE_CTX *ectx = EVP_ENCODE_CTX_new();
    EVP_EncodeInit(ectx);
    for (int i = 0; i < 30; i++)
    {
        in[i] = i;
    }
    int outl, inl = 30;
    EVP_EncodeUpdate(ectx, base64, &outl, in, inl);
    EVP_EncodeFinal(ectx, base64 + outl, &outl);
    EVP_ENCODE_CTX_free(ectx);
    printf("%40s\n", base64);

    printf("\n------------------------------\n");

    //rsa
    unsigned char inbuf[] = "https://github.com/gongluck/CVIP";
    unsigned char outbuf[128] = {0};
    int n = strlen(in);
    MD4(inbuf, n, outbuf);
    printf("MD4 result: \n"); // 16 byte
    for (int i = 0; i < 16; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");
    MD5(inbuf, n, outbuf);
    printf("MD5 result: \n"); // 16 byte
    for (int i = 0; i < 16; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");
    // SHA(inbuf, n, outbuf);
    // printf("SHA result: \n"); // 20 byte
    // for (int i = 0; i < 20; i++)
    // {
    //     printf("%x", outbuf[i]);
    // }
    // printf("\n");
    SHA1(inbuf, n, outbuf);
    printf("SHA1 result: \n"); // 20 byte
    for (int i = 0; i < 20; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");
    SHA256(inbuf, n, outbuf);
    printf("SHA256 result: \n"); // 32 byte
    for (int i = 0; i < 32; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");
    SHA512(inbuf, n, outbuf);
    printf("SHA512 result: \n"); // 64 byte
    for (int i = 0; i < 64; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");

    return 0;
}
```
</details>

### 6.网络IO管理

#### 6.1 [信号驱动IO](./code/io/signalio.c)

<details>
<summary>信号驱动IO</summary>

```C
/*
 * @Author: gongluck 
 * @Date: 2020-11-26 08:05:17 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 08:19:07
 */

// test : nc -uv 127.0.0.1 9096

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

int sockfd = 0;

void do_sigio(int sig)
{
    struct sockaddr_in cli_addr;
    int clilen = sizeof(struct sockaddr_in);

    char buffer[256] = {0};
    int len = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    printf("Message : %s\r\n", buffer);
    int slen = sendto(sockfd, buffer, len, 0, (struct sockaddr *)&cli_addr, clilen);
}

int main(int argc, char *argv[])
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sigaction sigio_action;
    sigio_action.sa_flags = 0;
    sigio_action.sa_handler = do_sigio;
    sigaction(SIGIO, &sigio_action, NULL);//SIGIO call do_sigio

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9096);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    fcntl(sockfd, F_SETOWN, getpid());
    int flags = fcntl(sockfd, F_GETFL, 0);
    flags |= O_ASYNC | O_NONBLOCK;//异步非阻塞
    fcntl(sockfd, F_SETFL, flags);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    while (1)
        sleep(1);

    close(sockfd);

    return 0;
}
```
</details>

#### 6.2 [select IO](./code/io/select.c)

<details>
<summary>select IO</summary>

```C
/*
 * @Author: gongluck 
 * @Date: 2020-11-26 08:36:17 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 08:39:02
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <errno.h>
#include <fcntl.h>

#define BUFFER_LENGTH 1024

int main(int argc, char *argv[])
{
    int port = 9096;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        return -2;
    }
    if (listen(listenfd, 5) < 0)
    {
        perror("listen");
        return -3;
    }

    fd_set rfds, rset;
    FD_ZERO(&rfds);
    FD_SET(listenfd, &rfds);

    int max_fd = listenfd;
    int i = 0;

    while (1)
    {
        rset = rfds;
        int nready = select(max_fd + 1, &rset, NULL, NULL, NULL);
        if (nready < 0)
        {
            printf("select error : %d\n", errno);
            continue;
        }

        if (FD_ISSET(listenfd, &rset)) // listen的fd可读代表有连接到达
        {
            struct sockaddr_in client_addr = {0};
            socklen_t client_len = sizeof(client_addr);

            int clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
            if (clientfd <= 0)
                continue;

            char str[INET_ADDRSTRLEN] = {0};
            printf("recvived from %s at port %d, listenfd:%d, clientfd:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
                   ntohs(client_addr.sin_port), listenfd, clientfd);

            if (max_fd == FD_SETSIZE) // select的fd上限一般是1024
            {
                printf("clientfd --> out range\n");
                break;
            }
            FD_SET(clientfd, &rfds); // 将新连接fd加入到读取队列中

            if (clientfd > max_fd)
                max_fd = clientfd;

            printf("listenfd:%d, max_fd:%d, clientfd:%d\n", listenfd, max_fd, clientfd);

            if (--nready == 0)
                continue;
        }

        for (i = listenfd + 1; i <= max_fd; i++)
        {
            if (FD_ISSET(i, &rset))
            {
                char buffer[BUFFER_LENGTH] = {0};
                int ret = recv(i, buffer, BUFFER_LENGTH, 0);
                if (ret < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) // 实际可能出现被其他线程读取掉数据的情况
                    {
                        printf("read all data");
                    }
                    FD_CLR(i, &rfds);
                    close(i);
                }
                else if (ret == 0)
                {
                    printf("disconnect %d\n", i);
                    FD_CLR(i, &rfds);
                    close(i);
                    break;
                }
                else
                {
                    printf("Recv: %s, %d Bytes\n", buffer, ret);
                }
                if (--nready == 0)
                    break;
            }
        }
    }

    return 0;
}
```
</details>

#### 6.3 [poll IO](./code/io/poll.c)

<details>
<summary>poll IO</summary>

```C
/*
 * @Author: gongluck 
 * @Date: 2020-11-26 08:36:17 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 08:44:12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/poll.h>

#include <errno.h>
#include <fcntl.h>

#define BUFFER_LENGTH 1024
#define POLL_SIZE 1024

int main(int argc, char *argv[])
{
    int port = 9096;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        return -2;
    }
    if (listen(listenfd, 5) < 0)
    {
        perror("listen");
        return -3;
    }

    struct pollfd fds[POLL_SIZE] = {0};
    fds[0].fd = listenfd;
    fds[0].events = POLLIN;

    int max_fd = 0, i = 0;
    for (i = 1; i < POLL_SIZE; i++)
    {
        fds[i].fd = -1;
    }

    while (1)
    {
        int nready = poll(fds, max_fd + 1, 5);
        if (nready <= 0)
            continue;

        if ((fds[0].revents & POLLIN) == POLLIN) // 判断listenfd是否有数据可读(新连接)
        {
            struct sockaddr_in client_addr = {0};
            socklen_t client_len = sizeof(client_addr);

            int clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
            if (clientfd <= 0)
                continue;

            char str[INET_ADDRSTRLEN] = {0};
            printf("recvived from %s at port %d, sockfd:%d, clientfd:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
                   ntohs(client_addr.sin_port), listenfd, clientfd);

            fds[clientfd].fd = clientfd;
            fds[clientfd].events = POLLIN;

            if (clientfd > max_fd)
                max_fd = clientfd;

            if (--nready == 0)
                continue;
        }

        for (i = listenfd + 1; i <= max_fd; i++)
        {
            if (fds[i].revents & (POLLIN | POLLERR))
            {
                char buffer[BUFFER_LENGTH] = {0};
                int ret = recv(i, buffer, BUFFER_LENGTH, 0);
                if (ret < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        printf("read all data");
                    }

                    close(i);
                    fds[i].fd = -1;
                }
                else if (ret == 0)
                {
                    printf(" disconnect %d\n", i);

                    close(i);
                    fds[i].fd = -1;
                    break;
                }
                else
                {
                    printf("Recv: %s, %d Bytes\n", buffer, ret);
                }
                if (--nready == 0)
                    break;
            }
        }
    }

    return 0;
}
```
</details>

#### 6.4 [epoll IO](./code/io/epoll.c)

<details>
<summary>epoll IO</summary>

```C
/*
 * @Author: gongluck 
 * @Date: 2020-11-26 08:36:17 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 08:52:01
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include <errno.h>
#include <fcntl.h>

#define BUFFER_LENGTH 1024
#define EPOLL_SIZE 1024

int main(int argc, char *argv[])
{
    int port = 9096;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        return -2;
    }
    if (listen(listenfd, 5) < 0)
    {
        perror("listen");
        return -3;
    }

    int epoll_fd = epoll_create(EPOLL_SIZE);
    struct epoll_event ev, events[EPOLL_SIZE] = {0};

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev);

    while (1)
    {
        int nready = epoll_wait(epoll_fd, events, EPOLL_SIZE, -1);
        if (nready == -1)
        {
            printf("epoll_wait\n");
            break;
        }

        for (int i = 0; i < nready; i++)
        {
            if (events[i].data.fd == listenfd)
            {
                struct sockaddr_in client_addr = {0};
                socklen_t client_len = sizeof(client_addr);

                int clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
                if (clientfd <= 0)
                    continue;

                char str[INET_ADDRSTRLEN] = {0};
                printf("recvived from %s at port %d, sockfd:%d, clientfd:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
                       ntohs(client_addr.sin_port), listenfd, clientfd);

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientfd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &ev);
            }
            else
            {
                int clientfd = events[i].data.fd;

                char buffer[BUFFER_LENGTH] = {0};
                int ret = recv(clientfd, buffer, BUFFER_LENGTH, 0);
                if (ret < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        printf("read all data");
                    }

                    close(clientfd);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = clientfd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientfd, &ev);
                }
                else if (ret == 0)
                {
                    printf(" disconnect %d\n", clientfd);

                    close(clientfd);

                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = clientfd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientfd, &ev);

                    break;
                }
                else
                {
                    printf("Recv: %s, %d Bytes\n", buffer, ret);
                }
            }
        }
    }

    return 0;
}
```
</details>

## 五、网络服务专题

### 1.Reactor反应堆

#### 1.1 Reactor模型

![Reactor模型](./images/Reactor模型.png?raw=true)

- Reactor释义“反应堆”，是一种事件驱动机制。和普通函数调用的不同之处在于：应用程序不是主动的调用某个API完成处理，而是恰恰 相反，Reactor逆置了事件处理流程，应用程序需要提供相应的接口并注册到Reactor上，如果相应的时间发生，Reactor将主动调用应用程序注册的接口，这些接口又称为“回调函数”。

- Reactor模式是处理并发I/O比较常见的一种模式，用于同步I/O，中心思想是将所有要处理的I/O事件注册到一个中心I/O多路复用器上，同时主线程/进程阻塞在多路复用器上；一旦有I/O事件到来或是准备就绪（文件描述符或socket可读、写），多路复用器返回并将事先注册的相应I/O事件分发到对应的处理器中。

- Reactor模型有三个重要的组件：

  ​	![Reactor组件](./images/Reactor组件.png?raw=true)

  - 多路复用器：由操作系统提供，在linux上一般是select、poll、epoll等系统调用。
  - 事件分发器：将多路复用器中返回的就绪事件分到对应的处理函数中。
  - 事件处理器：负责处理特定事件的处理函数。

#### 1.2 [Reactor实现](./code/io/reactor.c)

<details>
<summary>Reactor实现</summary>

```C
/*
 * @Author: gongluck 
 * @Date: 2020-11-26 09:41:40 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-26 16:58:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_LENGTH 4096
#define MAX_EPOLL_EVENTS 1024
#define SERVER_PORT 8888

typedef int NCALLBACK(int, int, void *);

struct ntyevent
{
    int fd;
    int events;
    void *arg;
    int (*callback)(int fd, int events, void *arg);

    int status;//是否已经添加到epoll中
    char buffer[BUFFER_LENGTH];
    int length;
    long last_active;
};

struct ntyreactor
{
    int epfd;
    struct ntyevent events[MAX_EPOLL_EVENTS];
};

void nty_event_set(struct ntyevent *ev, int fd, NCALLBACK callback, void *arg)
{
    ev->fd = fd;
    ev->callback = callback;
    ev->events = 0;
    ev->arg = arg;
    ev->last_active = time(NULL);

    return;
}

int nty_event_add(int epfd, int events, struct ntyevent *ev)
{
    struct epoll_event ep_ev = {0, {0}};
    ep_ev.data.ptr = ev;
    ep_ev.events = ev->events = events;

    int op;
    if (ev->status == 1)
    {
        op = EPOLL_CTL_MOD;
    }
    else
    {
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }

    if (epoll_ctl(epfd, op, ev->fd, &ep_ev) < 0)
    {
        printf("event add failed [fd=%d], events[%d]\n", ev->fd, events);
        return -1;
    }

    return 0;
}

int nty_event_del(int epfd, struct ntyevent *ev)
{
    struct epoll_event ep_ev = {0, {0}};

    if (ev->status != 1)
    {
        return -1;
    }

    ep_ev.data.ptr = ev;
    ev->status = 0;
    epoll_ctl(epfd, EPOLL_CTL_DEL, ev->fd, &ep_ev);

    return 0;
}

int recv_cb(int fd, int events, void *arg);
int send_cb(int fd, int events, void *arg)
{
    struct ntyreactor *reactor = (struct ntyreactor *)arg;
    struct ntyevent *ev = reactor->events + fd;

    int len = send(fd, ev->buffer, ev->length, 0);
    if (len > 0)
    {
        printf("send[fd=%d], [%d]%s\n", fd, len, ev->buffer);

        nty_event_del(reactor->epfd, ev);
        nty_event_set(ev, fd, recv_cb, reactor);
        nty_event_add(reactor->epfd, EPOLLIN, ev);
    }
    else
    {
        close(ev->fd);

        nty_event_del(reactor->epfd, ev);
        printf("send[fd=%d] error %s\n", fd, strerror(errno));
    }

    return len;
}

int recv_cb(int fd, int events, void *arg)
{
    struct ntyreactor *reactor = (struct ntyreactor *)arg;
    struct ntyevent *ev = reactor->events + fd;

    int len = recv(fd, ev->buffer, BUFFER_LENGTH, 0);
    nty_event_del(reactor->epfd, ev);
    if (len > 0)
    {
        ev->length = len;
        ev->buffer[len] = '\0';

        printf("C[%d]:%s\n", fd, ev->buffer);

        nty_event_set(ev, fd, send_cb, reactor);
        nty_event_add(reactor->epfd, EPOLLOUT, ev);
    }
    else if (len == 0)
    {
        close(ev->fd);
        printf("[fd=%d] pos[%ld], closed\n", fd, ev - reactor->events);
    }
    else
    {
        close(ev->fd);
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
    }

    return len;
}

int accept_cb(int fd, int events, void *arg)
{
    struct ntyreactor *reactor = (struct ntyreactor *)arg;
    if (reactor == NULL)
        return -1;

    struct sockaddr_in client_addr = {0};
    socklen_t len = sizeof(client_addr);

    int clientfd;
    if ((clientfd = accept(fd, (struct sockaddr *)&client_addr, &len)) == -1)
    {
        if (errno != EAGAIN && errno != EINTR)
        {
        }
        printf("accept: %s\n", strerror(errno));
        return -1;
    }

    int i = 0;
    do
    {
        for (i = 0; i < MAX_EPOLL_EVENTS; i++)
        {
            if (reactor->events[i].status == 0)
            {
                break;
            }
        }
        if (i == MAX_EPOLL_EVENTS)
        {
            printf("%s: max connect limit[%d]\n", __func__, MAX_EPOLL_EVENTS);
            break;
        }

        if (fcntl(clientfd, F_SETFL, O_NONBLOCK) < 0)
        {
            printf("%s: fcntl nonblocking failed, %d\n", __func__, MAX_EPOLL_EVENTS);
            break;
        }

        nty_event_set(&reactor->events[clientfd], clientfd, recv_cb, reactor);
        nty_event_add(reactor->epfd, EPOLLIN, &reactor->events[clientfd]);
    } while (0);

    printf("new connect [%s:%d][time:%ld], pos[%d]\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), reactor->events[i].last_active, i);

    return 0;
}

int init_sock(short port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (listen(fd, 20) < 0)
    {
        printf("listen failed : %s\n", strerror(errno));
    }

    return fd;
}

int ntyreactor_init(struct ntyreactor *reactor)
{
    if (reactor == NULL)
        return -1;
    memset(reactor, 0, sizeof(struct ntyreactor));

    reactor->epfd = epoll_create(1);
    if (reactor->epfd <= 0)
    {
        printf("create epfd in %s err %s\n", __func__, strerror(errno));
        return -2;
    }
}

int ntyreactor_destory(struct ntyreactor *reactor)
{
    close(reactor->epfd);
    
    return 0;
}

int ntyreactor_addlistener(struct ntyreactor *reactor, int sockfd, NCALLBACK *acceptor)
{
    if (reactor == NULL)
        return -1;

    nty_event_set(&reactor->events[sockfd], sockfd, acceptor, reactor);
    nty_event_add(reactor->epfd, EPOLLIN, &reactor->events[sockfd]);

    return 0;
}

int ntyreactor_run(struct ntyreactor *reactor)
{
    if (reactor == NULL)
        return -1;
    if (reactor->epfd < 0)
        return -1;

    struct epoll_event events[MAX_EPOLL_EVENTS + 1];
    int checkpos = 0, i;
    while (1)
    {
        long now = time(NULL);
        for (i = 0; i < MAX_EPOLL_EVENTS/10; i++, checkpos++)
        {
            if (checkpos == MAX_EPOLL_EVENTS)
            {
                checkpos = 0;
            }

            if (reactor->events[checkpos].status != 1)
            {
                continue;
            }

            long duration = now - reactor->events[checkpos].last_active;
            if (duration >= 60)
            {
                close(reactor->events[checkpos].fd);
                printf("[fd=%d] timeout\n", reactor->events[checkpos].fd);
                nty_event_del(reactor->epfd, &reactor->events[checkpos]);
            }
        }

        int nready = epoll_wait(reactor->epfd, events, MAX_EPOLL_EVENTS, 1000);
        if (nready < 0)
        {
            printf("epoll_wait error, exit\n");
            continue;
        }

        for (i = 0; i < nready; i++)
        {
            struct ntyevent *ev = (struct ntyevent *)events[i].data.ptr;
            if ((events[i].events & EPOLLIN) && (ev->events & EPOLLIN))
            {
                ev->callback(ev->fd, events[i].events, ev->arg);
            }
            if ((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT))
            {
                ev->callback(ev->fd, events[i].events, ev->arg);
            }
        }
    }
}

int main()
{
    unsigned short port = SERVER_PORT;
    int sockfd = init_sock(port);

    struct ntyreactor reactor;
    ntyreactor_init(&reactor);

    ntyreactor_addlistener(&reactor, sockfd, accept_cb);
    ntyreactor_run(&reactor);

    ntyreactor_destory(&reactor);
    close(sockfd);

    return 0;
}
```
</details>

### 2.日志模块

#### 2.1 log4cpp模块

![log4cpp模块图](./images/log4cpp模块图.png?raw=true)

- log4cpp有且只⼀个根Category，可以有多个⼦Category组成树型结构。

  ![log4cpp的appender种类](./images/log4cpp的appender种类.png?raw=true)

- Appender负责将⽇志写⼊相应的设备，⽐如控制台、⽂件、调试器、Windows⽇志、syslog等。

- Layout控制输出⽇志的显示样式。Log4cpp内置了4种Layout：

  - PassThroughLayout：直通布局。
  - SimpleLayout：简单布局。它只会为你添加“优先级”的输出。
  - BasicLayout：基本布局。它会为你添加“时间”、“优先级”、“种类”、“NDC”。
  - PatternLayout：格式化布局。它的使⽤⽅式类似C语⾔中的printf，使⽤格式化它符串来描述输出格式。

#### 2.2 log4cpp编译安装

```shell
wget https://sourceforge.net/projects/log4cpp/files/log4cpp-1.1.x%20%28new%29/log4cpp-1.1/log4cpp-1.1.3.tar.gz
tar zxf log4cpp-1.1.3.tar.gz
cd log4cpp
./configure
make -j 8
sudo make install
sudo ldconfig
```

#### 2.3 [log4cpp例子](./code/log/log4cpp)

- [例子1](./code/log/log4cpp/log1.cpp)

  <details>
  <summary>例子1</summary>
  
  ```C++
  /*
   * @Author: gongluck 
   * @Date: 2020-11-27 21:12:58 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-27 21:30:54
   */

  // g++ log1.cpp -llog4cpp -lpthread

  #include "log4cpp/Category.hh"
  #include "log4cpp/FileAppender.hh"
  #include "log4cpp/OstreamAppender.hh"
  #include "log4cpp/BasicLayout.hh"

  int main()
  {
      // 实例化一个layout对象
      log4cpp::Layout *layout = new log4cpp::BasicLayout();
      // 初始化一个appender对象
      log4cpp::Appender *appender = new log4cpp::FileAppender("FileAppender","./log1.log");
      log4cpp::Appender *osappender = new log4cpp::OstreamAppender("OstreamAppender",&std::cout);
      // 把layout对象附着在appender对象上，一个layout格式样式对应一个appender
      appender->setLayout(layout);
      // 实例化一个category对象
      log4cpp::Category &warn_log = log4cpp::Category::getInstance("gongluck"); // 单例工厂
      // 设置additivity为false，替换已有的appender，不继承父类appender
      warn_log.setAdditivity(false);
      // 把appender对象附到category上
      warn_log.setAppender(appender);
      warn_log.addAppender(osappender);
      // 设置category的优先级，低于此优先级的日志不被记录
      warn_log.setPriority(log4cpp::Priority::INFO);
      // 记录一些日志
      warn_log.info("Program info which cannot be wirten");
      warn_log.debug("This debug message will fail to write");
      warn_log.alert("Alert info");
      // 其他记录日志方式
      warn_log.log(log4cpp::Priority::WARN, "This will be a logged warning");

      log4cpp::Priority::PriorityLevel priority = log4cpp::Priority::DEBUG;
      warn_log.log(priority, "Importance depends on context");
      warn_log.critStream() << "This will show up << as " << 1 << " critical message";
      // clean up and flush all appenders
      log4cpp::Category::shutdown();
      return 0;
  }
  ```
  </details>

- [例子2](./code/log/log4cpp/log2.cpp)

  <details>
  <summary>例子2</summary>
  
  ```C++
  /*
   * @Author: gongluck 
   * @Date: 2020-11-27 21:31:03 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-11-27 21:44:29
   */
  
  // g++ log2.cpp -llog4cpp -lpthread
  
  #include "log4cpp/Category.hh"
  #include "log4cpp/PropertyConfigurator.hh"
  
  int main()
  {
      try
      {
          log4cpp::PropertyConfigurator::configure("./log2.conf");
      }
      catch (log4cpp::ConfigureFailure &f)
      {
          std::cout << "Configure Problem " << f.what() << std::endl;
          return -1;
      }
  
      // 实例化category对象
      log4cpp::Category &root = log4cpp::Category::getRoot();
  
      log4cpp::Category &category1 = log4cpp::Category::getInstance(std::string("category1"));
      log4cpp::Category &category3 = log4cpp::Category::getInstance(std::string("category1.category2"));
     
      category1.info("This is some info");
      category1.alert("A warning");
  
      category3.debug("This debug message will fail to write");
      category3.alert("All hands abandon ship");
      category3.critStream() << "This will show up << as " << 1 << " critical message";
      category3 << log4cpp::Priority::ERROR<< "And this will be an error";
      category3.log(log4cpp::Priority::WARN, "This will be a logged warning");
      
      log4cpp::Category::shutdown();
      return 0;
  }
  ```
  </details>
  

### 3.百万并发

#### 3.1 C10K

- 从资源上来说，对2GB内存和千兆网卡的服务器来说，同时处理10000个请求，只要每个请求处理占用不到200KB（2GB/10000）的内存和100Kbit（1000Mbit/10000）的网络带宽就可以。所以，物理资源是足够的，接下来自然是软件的问题，特别是网络的 I/O 模型问题。

- 在C10K以前，Linux中网络处理都用同步阻塞的方式，也就是每个请求都分配一个进程或者线程。请求数只有100个时，这种方式自然没问题，但增加到10000个请求时，10000个进程或线程的调度、上下文切换乃至它们占用的内存，都会成为瓶颈。

-  **使用非阻塞I/O和水平触发通知，比如使用 select 或者 poll**

  - 应用软件使用select和poll时，需要对这些文件描述符列表进行轮询，这样，请求数多的时候就会比较耗时。并且，select和poll还有一些其他的限制。
  - select使用固定长度的位相量，表示文件描述符的集合，因此会有最大描述符数量的限制。比如，在32位系统中，默认限制是1024。并且，在select内部，检查套接字状态是用轮询的方法，再加上应用软件使用时的轮询，就变成了一个O(n^2)的关系。
  - poll改进了select的表示方法，换成了一个没有固定长度的数组，这样就没有了最大描述符数量的限制（当然还会受到系统文件描述符限制）。但应用程序在使用poll时，同样需要对文件描述符列表进行轮询，这样，处理耗时跟描述符数量就是O(N)的关系。
  - 应用程序每次调用select和poll时，还需要把文件描述符的集合，从用户空间传入内核空间，由内核修改后，再传出到用户空间中。这一来一回的内核空间与用户空间切换，也增加了处理成本。

-  **使用非阻塞I/O和边缘触发通知，比如epoll**

  - epoll使用红黑树，在内核中管理文件描述符的集合，这样，就不需要应用程序在每次操作时都传入、传出这个集合。
  - epoll使用事件驱动的机制，只关注有I/O事件发生的文件描述符，不需要轮询扫描整个集合。
  - 由于边缘触发只在文件描述符可读或可写事件发生时才通知，那么应用程序就需要尽可能多地执行I/O，并要处理更多的异常事件。

- **使用异步I/O（Asynchronous I/O，简称为 AIO）**
- 异步I/O允许应用程序同时发起很多I/O操作，而不用等待这些操作完成。而在I/O完成后，系统会用事件通知（比如信号或者回调函数）的方式，告诉应用程序。这时，应用程序才会去查询I/O操作的结果。

#### 3.2 C1000K

- 基于C10K的这些理论，epoll配合线程池，再加上CPU、内存和网络接口的性能和容量提升。大部分情况下，C100K很自然就可以达到。
- 从软件资源上来说，大量的连接也会占用大量的软件资源，比如文件描述符的数量、连接状态的跟踪（CONNTRACK）、网络协议栈的缓存大小（比如套接字读写缓存、TCP 读写缓存）等等。最后，大量请求带来的中断处理，也会带来非常高的处理成本。这样，就需要多队列网卡、中断负载均衡、CPU 绑定、RPS/RFS（软中断负载均衡到多个 CPU 核上），以及将网络包的处理卸载（Offload）到网络设备（如 TSO/GSO、LRO/GRO、VXLAN OFFLOAD）等各种硬件和软件的优化。
- C1000K 的解决方法，本质上还是构建在epoll的非阻塞I/O模型上。只不过，除了I/O模型之外，还需要从应用程序到Linux内核、再到CPU、内存和网络等各个层次的深度优化，特别是需要借助硬件，来卸载那些原来通过软件处理的大量功能。

#### 3.3 C10M

- Linux内核协议栈做了太多太繁重的工作。从网卡中断带来的硬中断处理程序开始，到软中断中的各层网络协议处理，最后再到应用程序，这个路径实在是太长了，就会导致网络包的处理优化，到了一定程度后，就无法更进一步了。
- 要解决这个问题，最重要就是跳过内核协议栈的冗长路径，把网络包直接送到要处理的应用程序那里去。这里有两种常见的机制，DPDK和XDP。

## 六、性能测试专题

### 1.性能分析工具

#### 1.1 Valgrind

- Valgrind是一套Linux下，开放源代码（GPL V2）的仿真调试工具的集合。Valgrind由内核（core）以及基于内核的其他调试工具组成。内核类似于一个框架（framework）， 它模拟了一个CPU环境，并提供服务给其他工具；而其他工具则类似于插件(plug-in)，利用内核提供的服务完成各种特定的内存调试任务。

  ![valgrind体系结构](./images/valgrind体系结构.png?raw=true)

- 编译安装

  ```shell
  wget https://sourceware.org/pub/valgrind/valgrind-3.16.1.tar.bz2
  tar -jxvf valgrind-3.16.1.tar.bz2
  cd valgrind-3.16.1
  ./configure
  make -j 8
  sudo make install
  ```

- 使用valgrind

  - 为了使valgrind发现的错误更精确，如能够定位到源代码行，建议在编译时加上**-g**参数，编译优化选项请选择**-O0**，虽然这会降低程序的执行效率。

    ```shell
    gcc -g -O0 test.c
    ```

  - 使用命令

    ```shell
    valgrind [valgrind-options] your-prog [your-progoptions]
    valgrind --log-file=./valgrind_report.log --leak-check=full --show-leak-kinds=all --showreachable=no --track-origins=yes your-prog [your-progoptions]
    ```

#### 1.2 GDB

- 一般要调试某个程序，为了能清晰地看到调试的每一行代码、调用的堆栈信息、变量名和函数名等信息，需要调试程序含有调试符号信息。使用**gcc**编译程序时，如果加上**-g**选项即可在编译后的程序中保留调试符号信息。

- 除了不加**-g**选项，也可以使用Linux的**strip**命令移除掉某个程序中的调试信息。

  ```shell
  strip a.out
  ```

- gdb直接调试目标程序

  ```shell
  gdb a.out
  ```

- gdb附加进程

  ```shell
  sudo gdb attach pid
  ```

- gdb调试core文件

  ```shell
  gdb filename corename
  ```

  - **Linux**系统默认是不开启程序崩溃产生**core**文件这一机制的，我们可以使用**ulimit -a**命令来查看系统是否开启了这一机制。开启使用命令：

    ```shell
    # 将ulimit -c unlimited放入/etc/profile中，然后执行source /etc/profile即可立即生效。
    ulimit -c unlimited
    ```

  - 系统默认**corefile**是生成在程序的执行目录下或者程序启动调用了**chdir**之后的目录，我们可以通过设置生成**corefile**的格式来控制它，让其生成在固定的目录下，并让每次启动后自动生效。

    ```shell
    # 打开/etc/sysctl.conf
    sudo vi /etc/sysctl.conf
    # 末尾添加
    kernel.core_pattern=/home/gongluck/core_dump/core-%e-%p-%t
    # 创建目录
    mkdir /home/gongluck/core_dump
    # 执行生效
    sudo sysctl -p /etc/sysctl.conf
    ```

- gdb命令

  | 命令        | 说明                                                         |
  | ----------- | ------------------------------------------------------------ |
  | run         | 运行一个程序                                                 |
  | continue    | 让暂停的程序继续运行                                         |
  | next        | 运行到下一行                                                 |
  | step        | 如果有调用函数，进入调用的函数内部，相当于step into          |
  | until       | 运行到指定行停下来                                           |
  | finish      | 结束当前调用函数，到上一层函数调用处                         |
  | return      | 结束当前调用函数并返回指定值，到上一层函数调用处             |
  | jump        | 将当前程序执行流跳转到指定行或地址                           |
  | print       | 打印变量或寄存器值<br>当使用**print**命令打印一个字符串或者字符数组时，如果该字符串太长，**print**命令默认显示不全的，可以通过在**GDB**中输入**set print element 0**命令设置一下，这样再次使用 **print**命令就能完整地显示该变量的所有字符串了。<br>可以使用**p strerror(errno)**将错误码对应的文字信息打印出来<br>**print**命令同时也可以修改变量的值，**p ennro = 0** |
  | backtrace   | 查看当前线程的调用堆栈                                       |
  | frame       | 切换到当前调用线程的指定堆栈，具体堆栈通过堆栈序号指定       |
  | thread      | 切换到指定线程<br>**GDB**提供了一个在调试时将程序执行流锁定在当前调试线程的命令**set scheduler-locking on**。当然也可以关闭这一选项，使用**set scheduler-locking off** |
  | break       | 添加断点<br>条件断点的命令是**break [lineNo] if [condition]** |
  | tbreak      | 添加临时断点                                                 |
  | delete      | 删除断点                                                     |
  | enable      | 启用某个断点                                                 |
  | disable     | 禁用某个断点                                                 |
  | watch       | 监视某一个变量或内存地址的值是否发生变化                     |
  | list        | 显示源码                                                     |
  | info        | 查看断点/线程等信息。<br>**info functions**这个命令会显示程序中所有函数的名词，参数格式，返回值类型以及函数处于哪个代码文件中<br>**info threads**查看线程信息<br>**info args**查看当前函数的参数值 |
  | ptype       | 查看变量类型                                                 |
  | disassemble | 查看汇编代码                                                 |
  | set args    | 设置程序启动命令行参数                                       |
  | show args   | 查看设置的命令行参数                                         |

  - **GDB**调试器提供了一个选项叫**follow-fork**，可以使用**show follow-fork mode**查看当前值，也可以通过**set follow-fork mode**来设置是当一个进程**fork**出新的子进程时，**GDB**是继续调试父进程还是子进程（取值是**child**），默认是父进程（ 取值是**parent**）。

- 开启**GDB TUI**模式

  - 使用**gdbtui**命令或者**gdb-tui**命令开启一个调试。

    ```shell
    gdbtui -q 需要调试的程序名
    ```

  - 直接使用**GDB**调试代码，在需要的时候使用切换键**Ctrl + x**，然后按**a**，进入常规**GDB**和**GDB TUI**的来回切换。

### 2.Linux系统命令

#### 2.1 基础命令和工具

- **grep**搜索字符

  | 参数 | 作用                           |
  | ---- | ------------------------------ |
  | -c   | 仅显示找到的行数               |
  | -i   | 忽略大小写                     |
  | -n   | 显示行号                       |
  | -v   | 反向选择，仅列出没有关键词的行 |
  | -r   | 递归搜索文件目录               |
  | -C n | 打印匹配行的前后n行            |

- **find**查找文件

  ```shell
  find [指定查找目录] [查找规则] [查找完后执行的action]
  ```

  | 参数             | 作用                          |
  | ---------------- | ----------------------------- |
  | -name FILE_NAME  | 搜索文件名                    |
  | -iname FILE_NAME | 忽略文件名称大小写            |
  | -maxdepth n      | 最多查找n层                   |
  | -mindepth n      | 最少查找n层                   |
  | -size nK         | 查找文件大小近似nk的文件      |
  | -size +nK        | 查找文件大小大于nk的文件      |
  | -size -nK        | 查找文件大小小于nk的文件      |
  | -type d/f        | 查找目录/文件                 |
  | -cmin n/+n/-n    | 查找n分钟左右/以上/以下修改的 |
  | -ctime n/+n/-n   | 查找n天左右/以上/以下修改的   |

- **ls**显示文件

  | 参数 | 作用                       |
  | ---- | -------------------------- |
  | -t   | 可以查看最新修改的时间     |
  | -l   | 每行显示一个条目           |
  | -h   | 可以结合显示文件的GB、MB等 |
  | -R   | 递归显示                   |
  | -n   | 显示组id和gid              |

- **wc**计算字数

  ```shell
  wc [-clw][--help][--version][文件...]
  ```

  | 参数                 | 作用          |
  | -------------------- | ------------- |
  | -c或--bytes或--chars | 只显示Bytes数 |
  | -l或--lines          | 只显示行数    |
  | -w或--words          | 只显示字数    |
  | --help               | 在线帮助      |
  | --version            | 显示版本信息  |

- **uptime**机器启动时间+负载

- **ulimit**用户资源

  ```shell
  ulimit -a
  ```

- **curl**请求http

  | 参数                        | 说明                     |
  | --------------------------- | ------------------------ |
  | -i url                      | 打印请求响应头信息       |
  | -I url                      | 仅返回http头             |
  | -v url                      | 打印更多的调试信息       |
  | -d 'k=v' url                | 使用post方法提交http请求 |
  | curl -sw '%{http_code}' url | 打印http响应码           |

- **scp**远程拷贝

  ```shell
  //下载192.168.1.1的文件
  scp gongluck@192.168.1.1:/home/192.168.1.1/test.txt . 
  
  //上传文件到192.168.1.1
  scp test.txt gongluck@192.168.1.1:/home/gongluck/
  
  //下载test整个目录到本地
  scp -r gongluck@192.168.1.1:/home/gongluck/test . 
  
  //上传本地test整个目录到192.168.1.1
  scp -r test gongluck@192.168.1.1:/home/gongluck/ 
  ```

- **dos2unix**和**unix2dos**转换换行符

  ```shell
  dos2unix filename
  unix2dos filename
  ```

- **sed**字符替换

  ```shell
  sed 's/原字符串/新字符串/' 文件
  sed 's/原字符串/新字符串/g' 文件
  ```

#### 2.2 查看活动进程的命令

- **ps**进程信息

  ```shell
  ps -elf
  ps -ef
  ```

- **top**进程cpu内存信息

  ```shell
  top
  top -Hp pid
  ```

- **pidstat**进程资源

  | 参数 | 作用                                                         |
  | ---- | ------------------------------------------------------------ |
  | -u   | 查看cpu相关的性能指标                                        |
  | -w   | 查看上下文切换情况                                           |
  | -t   | 查看线程相关的信息，默认是进程的；常与**-w**结合使用（cpu的上下文切换包括进程的切换、线程的切换、中断的切换） |
  | -d   | 展示磁盘I/O统计数据                                          |
  | -p   | 指明进程号                                                   |

#### 2.3 内存命令

- **free**内存使用情况

  ```shell
  free
  ```

#### 2.4 CPU使用情况监控命令

- vmstat
- mpstat

#### 2.5 监控磁盘I/O的命令

- **iostat**IO 状态

  ```shell
  iostat -x
  iostat -d -k n
  iostat -dkx m n
  ```

- **swapon**查看分区使用情况

  ```shell
  swapon -s
  ```

- **df**硬盘使用情况

  ```shell
  df -h
  ```

- **du**目录文件大小

  | 参数          | 作用                                                         |
  | ------------- | ------------------------------------------------------------ |
  | -h            | 以人类可读的方式显示，显示M或K                               |
  | -a            | 显示目录占用的磁盘空间大小，还要显示其下目录和文件占用磁盘空间的大小 |
  | -s            | 显示目录占用的磁盘空间大小，不显示其下子目录和文件占用的磁盘空间大小 |
  | -c            | 显示几个目录或文件占用的磁盘空间大小，还要统计它们的总和     |
  | --max-depth=n | 查看当前目录下n级子文件和子目录占用的磁盘容量                |

#### 2.6 查看网络信息和网络监控的命令

- **ifconfig**查看和设置网络设备

  ```shell
  # 启动关闭指定网卡
  ifconfig eth0 up
  ifconfig eth0 down
  
  # 给eth0网卡配置IP地址
  ifconfig eth0 192.168.1.2
  #给eth0网卡配置IP地址，并加上子掩码
  ifconfig eth0 192.168.1.2 netmask 255.255.255.0
  #给eth0网卡配置IP地址，加上子掩码，加上个广播地址
  ifconfig eth0 192.168.1.56 netmask 255.255.255.0 broadcast 192.168.1.255
  ```

- **ping**

  ```shell
  ping [-dfnqrRv][-c<完成次数>][-i<间隔秒数>][-I<网络界面>][-l<前置载入>][-p<范本样
  式>][-s<数据包大小>][-t<存活数值>][主机名称或IP地址]
  ```

- **telnet**

  ```shell
  telnet IP PORT
  ```

- **nc**

  | 参数 | 作用                                                         |
  | ---- | ------------------------------------------------------------ |
  | -l   | 用于指定nc将处于侦听模式。指定该参数，则意味着nc被当作 server，侦听并接受连接，而非向其它地址发起连接。 |
  | -p   | 暂未用到（老版本的nc可能需要在端口号前加-p参数）             |
  | -s   | 指定发送数据的源IP地址，适用于多网卡机                       |
  | -u   | 指定nc使用**UDP**协议，默认为**TCP**                         |
  | -v   | 输出交互或出错信息，新手调试时尤为有用                       |
  | -w   | 超时秒数，后面跟数字                                         |

- **mtr**连通性测试

  ```shell
  mtr url
  
  #模拟丢包
  sudo tc qdisc add dev eth0 root netem loss 10%
  ```

- **nslookup**域名解析

  ```shell
  nslookup url
  ```

- **traceroute**路由

  ```shell
  traceroute url
  ```

- **sar**监控工具

  | 参数 | 作用                        |
  | ---- | --------------------------- |
  | -A   | 所有报告的总和              |
  | -u   | CPU利用率                   |
  | -v   | 进程、I节点、文件和锁表状态 |
  | -d   | 硬盘的使用报告              |
  | -r   | 没有使用的内存页面和硬盘快  |
  | -g   | 串口I/O的情况               |
  | -b   | 缓冲区的使用情况            |
  | -a   | 文件的读写情况              |
  | -c   | 系统的调用情况              |
  | -R   | 进程的活动情况              |
  | -y   | 终端设备的活动情况          |
  | -w   | 系统的交换活动              |

- **netstat**查看网络信息

  ```shell
  netstat -nap
  ```

- **iptraf**网络监控

  - **iptraf**是一个实时监控网络流量的交互式的彩色文本屏幕界面。它监控的数据比较全面，可以输出**TCP**连接、网络接口、协议、端口、网络包大小等信息，但是耗费的系 统资源比较多，且需要管理员权限。

- **tcpdump**网络分析

  | 参数                   | 作用                                                      |
  | ---------------------- | --------------------------------------------------------- |
  | -D                     | 列举所有网卡设备                                          |
  | -i                     | 选择网卡设备                                              |
  | -c                     | 抓取多少条报文                                            |
  | --time-stamp-precision | 指定捕获时的时间精度，默认微妙**micro**，可选纳秒**nano** |
  | -s                     | 指定每条报文的最大字节数，默认**262144**字节              |

- **lsof**列出打开文件

  ```shell
  # 显示端口被某个程序占用
  lsof -i:port
  # 看进程打开了哪些文件
  lsof -p pid
  # 显示abc进程现在打开的文件
  lsof -c abc
  # 显示打开文件abc.txt的进程
  lsof abc.txt
  ```

#### 2.7 Linux系统的高级工具

- **pstack**查看进程调用栈

  ```shell
  pstack pid
  ```

- **strace**跟踪系统调用

  ```shell
  strace -p pid
  ```

- **proc**文件系统

  ```shell
  # 显示CPU信息
  cat /proc/cpuinfo
  # 显示内存信息
  cat /proc/meminfo
  # 显示详细的内存映射信息
  cat /proc/zoneinfo
  # 显示磁盘映射信息
  cat /proc/mounts
  # 查看系统平均负载命令
  cat /proc/loadavg
  ```


## 七、代码工程化专题

### 1.代码版本工具

#### 1.1 git

![git原理](./images/git原理.png?raw=true)

![git文件状态](./images/git文件状态.png?raw=true)

- 常用命令

  ```shell
  # 查看分支
  git branch
  # 创建develop分支
  git branch develop
  # 创建FT-123456的一个feature分支
  git checkout –b feature/FT-123456
  # 切换分支
  git checkout develop
  # 合并分支
  git merge feature/FT-123456
  # 删除FT-123456的feature分支
  git branch –d feature/FT-123456
  # 推送分支
  git push –u origin hotfix/ISSUE-345678
  # 整理commit
  git rebase -i
  # 子模块
  git submodule add giturl loaclpath
  git submodule update --init --recurise
  ```


### 2.工程管理

#### 2.1 [CMake](./code/cmake)

- **CMake**命令

  - 生成工程

    ```shell
    cmake -S [源码目录] -B [生成目录] -G [目标工程(Unix Makefiles/Visual Studio 16 2019/...)] -A [附加选项(win32/x64)] -T [工具集(ClangCL/...)] -D[宏(编译开关)名称]=[宏值] -DCMAKE_TOOLCHAIN_FILE=[工具链配置文件(linux.clangtoolchain.cmake/...)] -DCMAKE_BUILD_TYPE=[生成选项(debug/release)] -DCMAKE_MAKE_PROGRAM=[make工具目录] -DANDROID_NDK=[NDK目录] -DANDROID_ABI=[安卓ABI(armeabi/...)] -DANDROID_PLATFORM=[安卓API等级(android-22/...)]
    # vs工程
    cmake -S . -B build -G "Visual Studio 16 2019" -A win32
    cmake -S . -B build -G "Visual Studio 16 2019" -A x64
    #vs下的安卓工程问题很多！可能需要安装"NVIDIA Nsight Tegra Visual Studio"
    cmake -S . -B build -G "Visual Studio 16 2019" -A ARM -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK_HOME%\build\cmake\android.toolchain.cmake" -DANDROID_NDK="%ANDROID_NDK_HOME%"
    # ndk工程
    cmake -G "Unix Makefiles" -S . -B build -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK_HOME%/build/cmake/android.toolchain.cmake" -DCMAKE_BUILD_TYPE=debug -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK_HOME%/prebuilt/windows-x86_64/bin/make.exe" -DANDROID_NDK="%ANDROID_NDK_HOME%" -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-22
    ```

  - 编译工程

    ```shell
    cmake --build [项目目录] --clean-first --config [选项(debug/release/...)] --target [项目(all/ALL_BUILD/...)]
    ```
    
  - 打包

    ```shell
    cpack -C [选项(debug/release/...)] --config [打包脚本(CPackSourceConfig.cmake/CPackConfig.cmake/...)] -B [保存目录]
    ```

- [**CMakeLists.txt**语法](./code/cmake/CMakeLists.txt)

  ```cmake
  # CMake最低版本要求
  cmake_minimum_required(VERSION 3.0)
  
  # 添加版本号
  set(VERSION_MAJOR 1)
  set(VERSION_MINOR 0)
  set(VERSION_PATCH 1)
  
  # 获取当前文件夹名
  STRING(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
  
  # 项目名称
  project(${CURRENT_FOLDER} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
  
  # 处理版本文件
  if(MSVC)
  # PROJECT_SOURCE_DIR为包含PROJECT()的最近一个CMakeLists.txt文件所在的文件夹
      set(VERSIONINFO_RC "${PROJECT_SOURCE_DIR}/RC.rc")
      # 处理RC.rc.in中的${PROJECT_VERSION_MAJOR}、${PROJECT_VERSION_MINOR}、${PROJECT_VERSION_PATCH}成${VERSION_MAJOR}、${VERSION_MINOR}、${VERSION_PATCH}
      configure_file("${PROJECT_SOURCE_DIR}/RC.rc.in"
                     "${VERSIONINFO_RC}")
  endif()
  
  # 使用C++11
  set(CMAKE_CXX_STANDARD 11)
  
  # 添加函数检查功能
  include(CheckFunctionExists)   
  check_function_exists(printf HAVEPRINTF)
  if(HAVEPRINTF)
    # 添加宏定义
    add_definitions(-DHAVEPRINTF)
  endif()
  
  # 共享变量(传递变量到子模块)
  if(BUILDMD)
  set(RUN_TIME "MD" CACHE INTERNAL "RUN_TIME")
  else()
  set(RUN_TIME "MT" CACHE INTERNAL "RUN_TIME")
  endif()
  
  # 目标平台判断
  if(WIN32)
  # 分别设置了Debug版本和Release版本可执行文件的输出目录(经测试,windows环境有效)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
  # 分别设置了Debug版本和Release版本库文件的输出目录(经测试,windows环境有效)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
  # 生成pdb
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
  set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/MP /${RUN_TIME} /Zi /GL")
  set(CMAKE_CXX_FLAGS_DEBUG "/MP /${RUN_TIME}d /Zi /Od /Ob0")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
  set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
  # 设置生成pdb路径
  set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
  elseif(ANDROID)
  
  # 设置可执行文件的输出目录(经测试,linux环境有效)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
  # 设置库文件的输出目录(经测试,linux环境有效)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -Wl,--strip-debug")
  set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} -Wl,--strip-debug")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-debug")
  #静态库使用
  #set(CMAKE_SYSROOT "${TOOLSCHAIN_PATH}/sysroot")
  #二进制执行模块使用
  set(CMAKE_SYSROOT "$ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -fstack-protector -fpic -frtti")
  set(CMAKE_CXX_FLAGS_DEBUG "-g2 -gdwarf-2 -O0")
  set(CMAKE_CXX_FLAGS_RELEASE "-gline-tables-only -O3 -ffunction-sections -fdata-sections")
  #arm二进制执行模块使用
  link_directories("$ENV{ANDROID_NDK_HOME}/platforms/android-22/arch-arm64/usr/lib")
  link_directories("$ENV{ANDROID_NDK_HOME}/sources/cxx-stl/llvm-libc++/libs/arm-v8a")
  link_directories("$ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/aarch64-linux-android/22")
  endif(ANDROID)
  
  # 字符串变量判断
  if(TEST STREQUAL "test")
  endif()
  
  # 自动添加当前源码目录和生成目录到包含目录
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
  # 加入一个配置头文件用于处理CMake对源码的设置
  configure_file(
    ${PROJECT_SOURCE_DIR}/config.h.in
    ${PROJECT_BINARY_DIR}/config.h
  )
  
  # 自定义编译选项
  option(USESUBMODULE "use submodule" ON)
  if (USESUBMODULE)
      # 设置变量
      set(SUBMODULE myfun)
      
      # 添加包含路径
      include_directories(${SUBMODULE})
       
      # 添加子目录
      # 必须放在aux_source_directory前,否则同名变量SRCS会冲突
      add_subdirectory(${SUBMODULE})
      
      # 设置附加库变量
      set(EXTRA_LIBS ${EXTRA_LIBS} ${SUBMODULE})
  endif (USESUBMODULE)
  
  # 指定第三方库路径
  link_directories("thirdparty")
  
  # 查找当前目录下所有源文件并保存到变量
  aux_source_directory(. SRCS)
  
  # 搜索源码文件保存到变量
  FILE(GLOB SRCS ./*.h ./*.c ./*.cpp)
  
  # 指定生成可执行文件
  add_executable(${PROJECT_NAME} ${SRCS})
  
  # 指定生成静态库
  add_library(${PROJECT_NAME} 
  	STATIC
  	${SRCS}
  )
  
  # 指定生成动态库
  add_library(${PROJECT_NAME} 
  	SHARED
  	${SRCS}
  )
  
  # 添加链接库
  target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
  target_link_libraries(${PROJECT_NAME} optimized "thirdparty" debug "thirdparty_d")
  
  # 设置调试版本文件后缀名
  set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX _d)
  
  # 添加编译完成后脚本
  if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME}
  	POST_BUILD
      COMMAND xcopy /S /Y /I test.h ..\\
  	COMMAND xcopy /S /Y /I ${PROJECT_NAME}* ..\\
  )
  elseif(ANDROID)
  add_custom_command(TARGET ${PROJECT_NAME}
  	POST_BUILD
      COMMAND xcopy /S /Y /I test.h ..\\ && xcopy /S /Y /I ${PROJECT_NAME}* ..\\
  )
  endif(WIN32)
  
  # 指定安装路径
  install(TARGETS ${PROJECT_NAME} DESTINATION bin)
  install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)
  install(DIRECTORY "${PROJECT_BINARY_DIR}/lib/" DESTINATION lib)
  
  # 定义一个宏，用来简化测试工作
  macro(do_test mycommand myret)
  add_test(NAME test_${mycommand}_${myret} COMMAND ${mycommand} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  # 检查测试输出是否包含"${myret}"
  set_tests_properties(test_${mycommand}_${myret} PROPERTIES PASS_REGULAR_EXPRESSION ${myret})
  endmacro(do_test)
  
  # 启用测试
  enable_testing()
  
  # 测试程序
  do_test(mydemo "cmake")
  
  # 构建一个CPack安装包
  include(InstallRequiredSystemLibraries)
  # 设置安装包版本号
  set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
  include(CPack)
  ```

## 八、分布式存储专题

### 1.FastDFS

![FastDFS架构](./images/FastDFS架构.png?raw=true)

#### 1.1 编译安装

```shell
# 下载
wget https://github.com/happyfish100/libfastcommon/archive/V1.0.43.tar.gz
# 解压
sudo tar -xzvf V1.0.43.tar.gz libfastcommon-1.0.43
# 进入解压后的目录
cd libfastcommon-1.0.43
# 编译代码
sudo ./make.sh
# 安装
sudo ./make.sh install
#export LD_LIBRARY_PATH=/usr/lib64/:$LD_LIBRARY_PATH
sudo ln -s /usr/lib64/libfastcommon.so /usr/lib/libfastcommon.so

# 下载
wget https://github.com/happyfish100/fastdfs/archive/V6.06.tar.gz
# 解压
sudo tar -zxvf V6.06.tar.gz fastdfs-6.06
# 进入解压后的目录
cd fastdfs-6.06
# 编译代码
sudo ./make.sh
# 安装
sudo ./make.sh install
```

#### 1.2 [tracker.conf](./code/fastfds/tracker.conf)

<details>
<summary>tracker.conf</summary>

```shell
# is this config file disabled
# false for enabled
# true for disabled
disabled = false

# bind an address of this host
# empty for bind all addresses of this host
bind_addr = 172.28.123.249  #tracker 所在服务器的 ip

# the tracker server port
port = 22122    #服务器端口

# connect timeout in seconds
# default value is 30
# Note: in the intranet network (LAN), 2 seconds is enough.
connect_timeout = 5

# network timeout in seconds for send and recv
# default value is 30
network_timeout = 60

# the base path to store data and log files
base_path = /mnt/e/ubuntu/tracker    #表示tracker日志存放的路径，此路径必须已经存在

# max concurrent connections this server support
# you should set this parameter larger, eg. 10240
# default value is 256
max_connections = 1024

# accept thread count
# default value is 1 which is recommended
# since V4.07
accept_threads = 1

# work thread count
# work threads to deal network io
# default value is 4
# since V2.00
work_threads = 4

# the min network buff size
# default value 8KB
min_buff_size = 8KB

# the max network buff size
# default value 128KB
max_buff_size = 128KB

# the method for selecting group to upload files
# 0: round robin
# 1: specify group
# 2: load balance, select the max free space group to upload file
store_lookup = 2

# which group to upload file
# when store_lookup set to 1, must set store_group to the group name
store_group = group2

# which storage server to upload file
# 0: round robin (default)
# 1: the first server order by ip address
# 2: the first server order by priority (the minimal)
# Note: if use_trunk_file set to true, must set store_server to 1 or 2
store_server = 0

# which path (means disk or mount point) of the storage server to upload file
# 0: round robin
# 2: load balance, select the max free space path to upload file
store_path = 0

# which storage server to download file
# 0: round robin (default)
# 1: the source storage server which the current file uploaded to
download_server = 0

# reserved storage space for system or other applications.
# if the free(available) space of any stoarge server in 
# a group <= reserved_storage_space, no file can be uploaded to this group.
# bytes unit can be one of follows:
### G or g for gigabyte(GB)
### M or m for megabyte(MB)
### K or k for kilobyte(KB)
### no unit for byte(B)
### XX.XX% as ratio such as: reserved_storage_space = 10%
reserved_storage_space = 20%

#standard log level as syslog, case insensitive, value list:
### emerg for emergency
### alert
### crit for critical
### error
### warn for warning
### notice
### info
### debug
log_level = info

#unix group name to run this program, 
#not set (empty) means run by the group of current user
run_by_group=

#unix username to run this program,
#not set (empty) means run by current user
run_by_user =

# allow_hosts can ocur more than once, host can be hostname or ip address,
# "*" (only one asterisk) means match all ip addresses
# we can use CIDR ips like 192.168.5.64/26
# and also use range like these: 10.0.1.[0-254] and host[01-08,20-25].domain.com
# for example:
# allow_hosts=10.0.1.[1-15,20]
# allow_hosts=host[01-08,20-25].domain.com
# allow_hosts=192.168.5.64/26
allow_hosts = *

# sync log buff to disk every interval seconds
# default value is 10 seconds
sync_log_buff_interval = 1

# check storage server alive interval seconds
check_active_interval = 120

# thread stack size, should >= 64KB
# default value is 256KB
thread_stack_size = 256KB

# auto adjust when the ip address of the storage server changed
# default value is true
storage_ip_changed_auto_adjust = true

# storage sync file max delay seconds
# default value is 86400 seconds (one day)
# since V2.00
storage_sync_file_max_delay = 86400

# the max time of storage sync a file
# default value is 300 seconds
# since V2.00
storage_sync_file_max_time = 300

# if use a trunk file to store several small files
# default value is false
# since V3.00
use_trunk_file = false 

# the min slot size, should <= 4KB
# default value is 256 bytes
# since V3.00
slot_min_size = 256

# the max slot size, should > slot_min_size
# store the upload file to trunk file when it's size <=  this value
# default value is 16MB
# since V3.00
slot_max_size = 1MB

# the alignment size to allocate the trunk space
# default value is 0 (never align)
# since V6.05
# NOTE: the larger the alignment size, the less likely of disk
#       fragmentation, but the more space is wasted.
trunk_alloc_alignment_size = 256

# if merge contiguous free spaces of trunk file
# default value is false
# since V6.05
trunk_free_space_merge = true

# if delete / reclaim the unused trunk files
# default value is false
# since V6.05
delete_unused_trunk_files = false

# the trunk file size, should >= 4MB
# default value is 64MB
# since V3.00
trunk_file_size = 64MB

# if create trunk file advancely
# default value is false
# since V3.06
trunk_create_file_advance = false

# the time base to create trunk file
# the time format: HH:MM
# default value is 02:00
# since V3.06
trunk_create_file_time_base = 02:00

# the interval of create trunk file, unit: second
# default value is 38400 (one day)
# since V3.06
trunk_create_file_interval = 86400

# the threshold to create trunk file
# when the free trunk file size less than the threshold,
# will create he trunk files
# default value is 0
# since V3.06
trunk_create_file_space_threshold = 20G

# if check trunk space occupying when loading trunk free spaces
# the occupied spaces will be ignored
# default value is false
# since V3.09
# NOTICE: set this parameter to true will slow the loading of trunk spaces 
# when startup. you should set this parameter to true when neccessary.
trunk_init_check_occupying = false

# if ignore storage_trunk.dat, reload from trunk binlog
# default value is false
# since V3.10
# set to true once for version upgrade when your version less than V3.10
trunk_init_reload_from_binlog = false

# the min interval for compressing the trunk binlog file
# unit: second, 0 means never compress
# FastDFS compress the trunk binlog when trunk init and trunk destroy
# recommand to set this parameter to 86400 (one day)
# default value is 0
# since V5.01
trunk_compress_binlog_min_interval = 86400

# the interval for compressing the trunk binlog file
# unit: second, 0 means never compress
# recommand to set this parameter to 86400 (one day)
# default value is 0
# since V6.05
trunk_compress_binlog_interval = 86400

# compress the trunk binlog time base, time format: Hour:Minute
# Hour from 0 to 23, Minute from 0 to 59
# default value is 03:00
# since V6.05
trunk_compress_binlog_time_base = 03:00

# max backups for the trunk binlog file
# default value is 0 (never backup)
# since V6.05
trunk_binlog_max_backups = 7

# if use storage server ID instead of IP address
# if you want to use dual IPs for storage server, you MUST set
# this parameter to true, and configure the dual IPs in the file
# configured by following item "storage_ids_filename", such as storage_ids.conf
# default value is false
# since V4.00
use_storage_id = false

# specify storage ids filename, can use relative or absolute path
# this parameter is valid only when use_storage_id set to true
# since V4.00
storage_ids_filename = storage_ids.conf

# id type of the storage server in the filename, values are:
## ip: the ip address of the storage server
## id: the server id of the storage server
# this paramter is valid only when use_storage_id set to true
# default value is ip
# since V4.03
id_type_in_filename = id

# if store slave file use symbol link
# default value is false
# since V4.01
store_slave_file_use_link = false

# if rotate the error log every day
# default value is false
# since V4.02
rotate_error_log = false

# rotate error log time base, time format: Hour:Minute
# Hour from 0 to 23, Minute from 0 to 59
# default value is 00:00
# since V4.02
error_log_rotate_time = 00:00

# if compress the old error log by gzip
# default value is false
# since V6.04
compress_old_error_log = false

# compress the error log days before
# default value is 1
# since V6.04
compress_error_log_days_before = 7

# rotate error log when the log file exceeds this size
# 0 means never rotates log file by log file size
# default value is 0
# since V4.02
rotate_error_log_size = 0

# keep days of the log files
# 0 means do not delete old log files
# default value is 0
log_file_keep_days = 0

# if use connection pool
# default value is false
# since V4.05
use_connection_pool = true

# connections whose the idle time exceeds this time will be closed
# unit: second
# default value is 3600
# since V4.05
connection_pool_max_idle_time = 3600

# HTTP port on this tracker server
http.server_port = 8080

# check storage HTTP server alive interval seconds
# <= 0 for never check
# default value is 30
http.check_alive_interval = 30

# check storage HTTP server alive type, values are:
#   tcp : connect to the storge server with HTTP port only, 
#        do not request and get response
#   http: storage check alive url must return http status 200
# default value is tcp
http.check_alive_type = tcp

# check storage HTTP server alive uri/url
# NOTE: storage embed HTTP server support uri: /status.html
http.check_alive_uri = /status.html
```
</details>

#### 1.3 [storage.conf](./code/fastfds/storage.conf)

<details>
<summary>storage.conf</summary>

```shell
# is this config file disabled
# false for enabled
# true for disabled
disabled = false

# the name of the group this storage server belongs to
#
# comment or remove this item for fetching from tracker server,
# in this case, use_storage_id must set to true in tracker.conf,
# and storage_ids.conf must be configured correctly.
group_name = group1 #组，默认设置即可

# bind an address of this host
# empty for bind all addresses of this host
bind_addr = 172.28.123.249  #storage 所在服务器 ip

# if bind an address of this host when connect to other servers 
# (this storage server as a client)
# true for binding the address configured by the above parameter: "bind_addr"
# false for binding any address of this host
client_bind = true

# the storage server port
port = 23000    #服务器端口，默认设置即可

# connect timeout in seconds
# default value is 30
# Note: in the intranet network (LAN), 2 seconds is enough.
connect_timeout = 5

# network timeout in seconds for send and recv
# default value is 30
network_timeout = 60

# the heart beat interval in seconds
# the storage server send heartbeat to tracker server periodically
# default value is 30
heart_beat_interval = 30

# disk usage report interval in seconds
# the storage server send disk usage report to tracker server periodically
# default value is 300
stat_report_interval = 60

# the base path to store data and log files
# NOTE: the binlog files maybe are large, make sure
#       the base path has enough disk space,
#       eg. the disk free space should > 50GB
base_path = /mnt/e/ubuntu/storage    #用于storage存放日志，此目录必须存在

# max concurrent connections the server supported,
# you should set this parameter larger, eg. 10240
# default value is 256
max_connections = 1024

# the buff size to recv / send data from/to network
# this parameter must more than 8KB
# 256KB or 512KB is recommended
# default value is 64KB
# since V2.00
buff_size = 256KB

# accept thread count
# default value is 1 which is recommended
# since V4.07
accept_threads = 1

# work thread count
# work threads to deal network io
# default value is 4
# since V2.00
work_threads = 4

# if disk read / write separated
##  false for mixed read and write
##  true for separated read and write
# default value is true
# since V2.00
disk_rw_separated = true

# disk reader thread count per store path
# for mixed read / write, this parameter can be 0
# default value is 1
# since V2.00
disk_reader_threads = 1

# disk writer thread count per store path
# for mixed read / write, this parameter can be 0
# default value is 1
# since V2.00
disk_writer_threads = 1

# when no entry to sync, try read binlog again after X milliseconds
# must > 0, default value is 200ms
sync_wait_msec = 50

# after sync a file, usleep milliseconds
# 0 for sync successively (never call usleep)
sync_interval = 0

# storage sync start time of a day, time format: Hour:Minute
# Hour from 0 to 23, Minute from 0 to 59
sync_start_time = 00:00

# storage sync end time of a day, time format: Hour:Minute
# Hour from 0 to 23, Minute from 0 to 59
sync_end_time = 23:59

# write to the mark file after sync N files
# default value is 500
write_mark_file_freq = 500

# disk recovery thread count
# default value is 1
# since V6.04
disk_recovery_threads = 3

# store path (disk or mount point) count, default value is 1
store_path_count = 1

# store_path#, based on 0, to configure the store paths to store files
# if store_path0 not exists, it's value is base_path (NOT recommended)
# the paths must be exist.
#
# IMPORTANT NOTE:
#       the store paths' order is very important, don't mess up!!!
#       the base_path should be independent (different) of the store paths

store_path0 = /mnt/e/ubuntu/storage/fastdfs0  #真正存储数据的路径，此路径必须存在
#store_path1 = /home/yuqing/fastdfs2

# subdir_count  * subdir_count directories will be auto created under each 
# store_path (disk), value can be 1 to 256, default value is 256
subdir_count_per_path = 256

# tracker_server can ocur more than once for multi tracker servers.
# the value format of tracker_server is "HOST:PORT",
#   the HOST can be hostname or ip address,
#   and the HOST can be dual IPs or hostnames seperated by comma,
#   the dual IPS must be an inner (intranet) IP and an outer (extranet) IP,
#   or two different types of inner (intranet) IPs.
#   for example: 192.168.2.100,122.244.141.46:22122
#   another eg.: 192.168.1.10,172.17.4.21:22122

tracker_server = 172.28.123.249:22122  #指定 tracker 服务器地址和端口。不能是 127.0.0.1
#tracker_server = 192.168.209.122:22122

#standard log level as syslog, case insensitive, value list:
### emerg for emergency
### alert
### crit for critical
### error
### warn for warning
### notice
### info
### debug
log_level = info

#unix group name to run this program, 
#not set (empty) means run by the group of current user
run_by_group =

#unix username to run this program,
#not set (empty) means run by current user
run_by_user =

# allow_hosts can ocur more than once, host can be hostname or ip address,
# "*" (only one asterisk) means match all ip addresses
# we can use CIDR ips like 192.168.5.64/26
# and also use range like these: 10.0.1.[0-254] and host[01-08,20-25].domain.com
# for example:
# allow_hosts=10.0.1.[1-15,20]
# allow_hosts=host[01-08,20-25].domain.com
# allow_hosts=192.168.5.64/26
allow_hosts = *

# the mode of the files distributed to the data path
# 0: round robin(default)
# 1: random, distributted by hash code
file_distribute_path_mode = 0

# valid when file_distribute_to_path is set to 0 (round robin).
# when the written file count reaches this number, then rotate to next path.
# rotate to the first path (00/00) after the last path (such as FF/FF).
# default value is 100
file_distribute_rotate_count = 100

# call fsync to disk when write big file
# 0: never call fsync
# other: call fsync when written bytes >= this bytes
# default value is 0 (never call fsync)
fsync_after_written_bytes = 0

# sync log buff to disk every interval seconds
# must > 0, default value is 10 seconds
sync_log_buff_interval = 1

# sync binlog buff / cache to disk every interval seconds
# default value is 60 seconds
sync_binlog_buff_interval = 1

# sync storage stat info to disk every interval seconds
# default value is 300 seconds
sync_stat_file_interval = 300

# thread stack size, should >= 512KB
# default value is 512KB
thread_stack_size = 512KB

# the priority as a source server for uploading file.
# the lower this value, the higher its uploading priority.
# default value is 10
upload_priority = 10

# the NIC alias prefix, such as eth in Linux, you can see it by ifconfig -a
# multi aliases split by comma. empty value means auto set by OS type
# default values is empty
if_alias_prefix =

# if check file duplicate, when set to true, use FastDHT to store file indexes
# 1 or yes: need check
# 0 or no: do not check
# default value is 0
check_file_duplicate = 0

# file signature method for check file duplicate
## hash: four 32 bits hash code
## md5: MD5 signature
# default value is hash
# since V4.01
file_signature_method = hash

# namespace for storing file indexes (key-value pairs)
# this item must be set when check_file_duplicate is true / on
key_namespace = FastDFS

# set keep_alive to 1 to enable persistent connection with FastDHT servers
# default value is 0 (short connection)
keep_alive = 0

# you can use "#include filename" (not include double quotes) directive to 
# load FastDHT server list, when the filename is a relative path such as 
# pure filename, the base path is the base path of current/this config file.
# must set FastDHT server list when check_file_duplicate is true / on
# please see INSTALL of FastDHT for detail
##include /home/yuqing/fastdht/conf/fdht_servers.conf

# if log to access log
# default value is false
# since V4.00
use_access_log = false

# if rotate the access log every day
# default value is false
# since V4.00
rotate_access_log = false

# rotate access log time base, time format: Hour:Minute
# Hour from 0 to 23, Minute from 0 to 59
# default value is 00:00
# since V4.00
access_log_rotate_time = 00:00

# if compress the old access log by gzip
# default value is false
# since V6.04
compress_old_access_log = false

# compress the access log days before
# default value is 1
# since V6.04
compress_access_log_days_before = 7

# if rotate the error log every day
# default value is false
# since V4.02
rotate_error_log = false

# rotate error log time base, time format: Hour:Minute
# Hour from 0 to 23, Minute from 0 to 59
# default value is 00:00
# since V4.02
error_log_rotate_time = 00:00

# if compress the old error log by gzip
# default value is false
# since V6.04
compress_old_error_log = false

# compress the error log days before
# default value is 1
# since V6.04
compress_error_log_days_before = 7

# rotate access log when the log file exceeds this size
# 0 means never rotates log file by log file size
# default value is 0
# since V4.02
rotate_access_log_size = 0

# rotate error log when the log file exceeds this size
# 0 means never rotates log file by log file size
# default value is 0
# since V4.02
rotate_error_log_size = 0

# keep days of the log files
# 0 means do not delete old log files
# default value is 0
log_file_keep_days = 0

# if skip the invalid record when sync file
# default value is false
# since V4.02
file_sync_skip_invalid_record = false

# if use connection pool
# default value is false
# since V4.05
use_connection_pool = true

# connections whose the idle time exceeds this time will be closed
# unit: second
# default value is 3600
# since V4.05
connection_pool_max_idle_time = 3600

# if compress the binlog files by gzip
# default value is false
# since V6.01
compress_binlog = true

# try to compress binlog time, time format: Hour:Minute
# Hour from 0 to 23, Minute from 0 to 59
# default value is 01:30
# since V6.01
compress_binlog_time = 01:30

# if check the mark of store path to prevent confusion
# recommend to set this parameter to true
# if two storage servers (instances) MUST use a same store path for
# some specific purposes, you should set this parameter to false
# default value is true
# since V6.03
check_store_path_mark = true

# use the ip address of this storage server if domain_name is empty,
# else this domain name will ocur in the url redirected by the tracker server
http.domain_name =

# the port of the web server on this storage server
http.server_port = 8888
```
</details>

#### 1.4 [client.conf](./code/fastfds/client.conf)

<details>
<summary>client.conf</summary>

```shell
# connect timeout in seconds
# default value is 30s
# Note: in the intranet network (LAN), 2 seconds is enough.
connect_timeout = 5

# network timeout in seconds
# default value is 30s
network_timeout = 60

# the base path to store log files
base_path = /mnt/e/ubuntu/client

# tracker_server can ocur more than once for multi tracker servers.
# the value format of tracker_server is "HOST:PORT",
#   the HOST can be hostname or ip address,
#   and the HOST can be dual IPs or hostnames seperated by comma,
#   the dual IPS must be an inner (intranet) IP and an outer (extranet) IP,
#   or two different types of inner (intranet) IPs.
#   for example: 192.168.2.100,122.244.141.46:22122
#   another eg.: 192.168.1.10,172.17.4.21:22122

tracker_server = 172.28.123.249:22122
#tracker_server = 192.168.0.197:22122

#standard log level as syslog, case insensitive, value list:
### emerg for emergency
### alert
### crit for critical
### error
### warn for warning
### notice
### info
### debug
log_level = info

# if use connection pool
# default value is false
# since V4.05
use_connection_pool = false

# connections whose the idle time exceeds this time will be closed
# unit: second
# default value is 3600
# since V4.05
connection_pool_max_idle_time = 3600

# if load FastDFS parameters from tracker server
# since V4.05
# default value is false
load_fdfs_parameters_from_tracker = false

# if use storage ID instead of IP address
# same as tracker.conf
# valid only when load_fdfs_parameters_from_tracker is false
# default value is false
# since V4.05
use_storage_id = false

# specify storage ids filename, can use relative or absolute path
# same as tracker.conf
# valid only when load_fdfs_parameters_from_tracker is false
# since V4.05
storage_ids_filename = storage_ids.conf


#HTTP settings
http.tracker_server_port = 80

#use "#include" directive to include HTTP other settiongs
##include http.conf
```

- 启动

  ```shell
  sudo fdfs_trackerd /mnt/e/Code/CVIP/code/fastfds/tracker.conf
  sudo fdfs_storaged /mnt/e/Code/CVIP/code/fastfds/storage.conf
  ```

- 测试

  ```shell
  # 查看服务
  fdfs_monitor /mnt/e/Code/CVIP/code/fastfds/client.conf
  # 上传
  fdfs_upload_file /mnt/e/Code/CVIP/code/fastfds/client.conf ./README.md
  # 下载
  fdfs_download_file /mnt/e/Code/CVIP/code/fastfds/client.conf group1/M00/00/00/rBx7-V_MGuWAF1sJAANQ-lXzICY3210.md
  # 删除
  fdfs_delete_file /mnt/e/Code/CVIP/code/fastfds/client.conf group1/M00/00/00/rBx7-V_MGuWAF1sJAANQ-lXzICY3210.md
  ```
  </details>
  

### 2.FastCGI

#### 2.1 CGI工作方式

- 传统**CGI**（Common Gateway Interface）方式是客户端有多少个请求，就开辟多少个子进程，每个子进程都需要启动自己的解释器、加载配置，连接其他服务器等初始化工作，这是**CGI**进程性能低下的主要原因。当用户请求非常多的时候，会占用大量的内存、cpu等资源，造成性能低下。

  ![cgi工作方式](./images/cgi工作方式.png?raw=true)

- 与为每个请求创建一个新的进程不同，**FastCGI**使用持续的进程来处理一连串的请求。这些进程由 **FastCGI**进程管理器管理，而不是**web**服务器。

  ![fastcgi工作方式](./images/fastcgi工作方式.png?raw=true)

- 由于**FastCGI**程序并不需要不断的产生新进程，可以大大降低服务器的压力并且产生较高的应用效率。它的速度效率最少要比**CGI**技术提高5倍以上。它还支持分布式的部署，即**FastCGI**程序可以在**web**服务器以外的主机上执行。

#### 2.2 spawn-fcgi

- **spawn-fcgi**是一个通用的**FastCGI**进程管理器，简单小巧。

- **spawn-fcgi**使用**pre-fork**模型，功能主要是打开监听端口，绑定地址，然后**fork-and-exec**创建我们编写的**FastCGI**应用程序进程，退出完成工作。**FastCGI**应用程序初始化，然后进入死循环侦听**socket**的连接请求。

- 编译安装**spawn-fcgi**

  ```shell
  wget http://download.lighttpd.net/spawn-fcgi/releases-1.6.x/spawn-fcgi-1.6.4.tar.gz
  tar -zxvf spawn-fcgi-1.6.4.tar.gz
  cd spawn-fcgi-1.6.4/
  ./configure
  make -j 8
  sudo make install
  #sudo ln -s /usr/local/lib/libfcgi.so.0 /usr/lib/libfcgi.so.0
  ```

#### 2.3 fcgi

- 使用**C/C++**编写**FastCGI**应用程序，可以使用**FastCGI**软件开发套件或者其它开发框架，如**fcgi**。

- 编译安装**fcgi**

  ```shell
  wget https://fossies.org/linux/www/old/fcgi-2.4.0.tar.gz
  tar -zxvf fcgi-2.4.0.tar.gz
  cd fcgi-2.4.0/
  ./configure
  make -j 8
  sudo make install
  ```

- [fcgil例子](./code/fcgi/test.c)

  <details>
  <summary>fcgi例子</summary>
  
  ```C
  /*
   * @Author: gongluck 
   * @Date: 2020-12-06 09:31:16 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-12-06 10:36:28
   */
  
  // gcc test.c -lfcgi
  // spawn-fcgi -a 127.0.0.1 -p 8001 -f ./a.out
  /* nginx conf add
  location /test {
  			fastcgi_pass 127.0.0.1:8001;
  			fastcgi_index test;
  			include /usr/local/nginx/conf/fastcgi.conf;
  		}	
  */
  
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include "fcgi_stdio.h"
  
  int main(int argc, char *argv[])
  {
      int count = 0;
      //阻塞等待并监听某个端口，等待Nginx将数据发过来
      while (FCGI_Accept() >= 0)
      {
          //如果想得到数据，需要从stdin去读，实际上从Nginx上去读
          //如果想上传数据，需要往stdout写，实际上是给Nginx写数据
          printf("Content-type: text/html\r\n");
          printf("\r\n");
          printf("<title>Fast CGI Hello!</title>");
          printf("<h1>Fast CGI Hello!</h1>");
          //SERVER_NAME：得到server的host名称
          printf("Request number %d running on host <i>%s</i>\n",
                 ++count, getenv("SERVER_NAME"));
      }
      return 0;
  }
  ```
  </details>
  

## 九、源码分析专题

### 1.Nginx架构和模块

#### 1.1 Nginx架构

- **nginx**在启动后，在**unix**系统中会以**daemon**的方式在后台运行，后台进程包含一个**master**进程和多个**worker**进程。
- **master**进程主要用来管理**worker**进程，包含：
  - 接收来自外界的信号，
  - 向各**worker**进程发送信号，
  - 监控**worker**进程的运行状态，
  - 当**worker**进程退出后(异常情况下)，会自动重新启动新的**worker**进程。

- 而基本的网络事件，则是放在**worker**进程中来处理了。多个**worker**进程之间是对等的，他们同等竞争来自客户端的请求，各进程互相之间是独立的。

  ![nginx进程模型](./images/nginx进程模型.png?raw=true)

- 每个**worker**进程都是从**master**进程**fork**过来，在**master**进程里面，先建立好需要**listen**的**socket**（**listenfd**）之后，然后再**fork**出多个**worker**进程。所有**worker**进程的**listenfd**会在新连接到来时变得可读，为保证只有一个进程处理该连接，所有**worker **进程在注册**listenfd**读事件前抢**accept_mutex**，抢到互斥锁的那个进程注册**listenfd**读事件，在读事件里调用**accept**接受该连接。当一个**worker**进程在**accept**这个连接之后，就开始读取请求，解析请求，处理请求，产生数据后，再返回给客户端，最后才断开连接，这样一个完整的请求就是这样的了。
- 对于每个**worker**进程来说，独立的进程，不需要加锁，所以省掉了锁带来的开销，同时在编程以及问题查找时，也会方便很多。其次，采用独立的进程，可以让互相之间不会影响，一个进程退出后，其它进程还在工作，服务不会中断，**master**进程则很快启动新的**worker**进程。
- **nginx**采用了异步非阻塞的方式来处理请求。非阻塞就是，事件没有准备好，马上返回**EAGAIN**。
- **nginx**里面的定时器事件是放在一颗维 护定时器的红黑树里面，每次在进入**epoll_wait**前，先从该红黑树里面拿到所有定时器事件的最小时间，在计算出**epoll_wait**的超时时间后进入**epoll_wait**。所以，当没有事件产生，也没有中断信号时，**epoll_wait**会超时，也就是说，定时器事件到了。这时，**nginx**会检查所有的超时事件，将他们的状态设置为超时，然后再去处理网络事件。

#### 1.2 Nginx基础概念

- **connection**

  <details>
  <summary>ngx_connection_s</summary>
  
  ```C
  struct ngx_connection_s {
      void               *data;
      ngx_event_t        *read;
      ngx_event_t        *write;
  
      ngx_socket_t        fd;
  
      ngx_recv_pt         recv;
      ngx_send_pt         send;
      ngx_recv_chain_pt   recv_chain;
      ngx_send_chain_pt   send_chain;
  
      ngx_listening_t    *listening;
  
      off_t               sent;
  
      ngx_log_t          *log;
  
      ngx_pool_t         *pool;
  
      int                 type;
  
      struct sockaddr    *sockaddr;
      socklen_t           socklen;
      ngx_str_t           addr_text;
  
      ngx_proxy_protocol_t  *proxy_protocol;
  
  #if (NGX_SSL || NGX_COMPAT)
      ngx_ssl_connection_t  *ssl;
  #endif
  
      ngx_udp_connection_t  *udp;
  
      struct sockaddr    *local_sockaddr;
      socklen_t           local_socklen;
  
      ngx_buf_t          *buffer;
  
      ngx_queue_t         queue;
  
      ngx_atomic_uint_t   number;
  
      ngx_uint_t          requests;
  
      unsigned            buffered:8;
  
      unsigned            log_error:3;     /* ngx_connection_log_error_e */
  
      unsigned            timedout:1;
      unsigned            error:1;
      unsigned            destroyed:1;
  
      unsigned            idle:1;
      unsigned            reusable:1;
      unsigned            close:1;
      unsigned            shared:1;
  
      unsigned            sendfile:1;
      unsigned            sndlowat:1;
      unsigned            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
      unsigned            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */
  
      unsigned            need_last_buf:1;
  
  #if (NGX_HAVE_AIO_SENDFILE || NGX_COMPAT)
      unsigned            busy_count:2;
  #endif
  
  #if (NGX_THREADS || NGX_COMPAT)
      ngx_thread_task_t  *sendfile_task;
  #endif
  };
  ```
  </details>

  - **nginx**在实现时，是通过一个连接池来管理的，每个**worker**进程都有一个独立的连接池，连接池的大小是**worker_connections**。这里的连接池里面保存的其实不是真实的连接，它只是一个**worker_connections**大小的一个**ngx_connection_t**结构的数组。并且，**nginx**会通过一个链表**free_connections**来保存所有的空闲**ngx_connection_t**，每次获取一个连接时，就从空闲连接链表中获取一个，用完后，再放回空闲连接链表里面。
  - 一个**nginx**能建立的最大连接数，应该是**worker_connections * worker_processes**。当然，这里说的是最大连接数，对于**HTTP**请求本地资源来说，能够支持的最大并发数量是 **worker_connections * worker_processes**，而如果是 HTTP 作为反向代理来说，最大并发数量应该是**worker_connections * worker_processes / 2**。因为作为反向代理服务器，每个并发会建立与客户端的连接和与后端服务的连接，会占用两个连接。
  - **nginx**使用一个叫**ngx_accept_disabled**的变量来控制是否去竞争**accept_mutex**锁。计算 **ngx_accept_disabled**的值，这个值是**nginx**单进程的所有连接总数的八分之一，减去剩下的空闲连接数量，得到的这个**ngx_accept_disabled**有一个规律，当剩余连接数小于总连接数的八分之一时，其值才大于 0，而且剩余的连接数越小，这个值越大。当**ngx_accept_disabled**大于0时，不会去尝试获取**accept_mutex**锁，并且将**ngx_accept_disabled**减1，于是，每次执行到此处时，都会去减1，直到小于0。

- **request**

  <details>
  <summary>ngx_http_request_s</summary>
  
  ```C
  struct ngx_http_request_s {
      uint32_t                          signature;         /* "HTTP" */
  
      ngx_connection_t                 *connection;
  
      void                            **ctx;
      void                            **main_conf;
      void                            **srv_conf;
      void                            **loc_conf;
  
      ngx_http_event_handler_pt         read_event_handler;
      ngx_http_event_handler_pt         write_event_handler;
  
  #if (NGX_HTTP_CACHE)
      ngx_http_cache_t                 *cache;
  #endif
  
      ngx_http_upstream_t              *upstream;
      ngx_array_t                      *upstream_states;
                                           /* of ngx_http_upstream_state_t */
  
      ngx_pool_t                       *pool;
      ngx_buf_t                        *header_in;
  
      ngx_http_headers_in_t             headers_in;
      ngx_http_headers_out_t            headers_out;
  
      ngx_http_request_body_t          *request_body;
  
      time_t                            lingering_time;
      time_t                            start_sec;
      ngx_msec_t                        start_msec;
  
      ngx_uint_t                        method;
      ngx_uint_t                        http_version;
  
      ngx_str_t                         request_line;
      ngx_str_t                         uri;
      ngx_str_t                         args;
      ngx_str_t                         exten;
      ngx_str_t                         unparsed_uri;
  
      ngx_str_t                         method_name;
      ngx_str_t                         http_protocol;
      ngx_str_t                         schema;
  
      ngx_chain_t                      *out;
      ngx_http_request_t               *main;
      ngx_http_request_t               *parent;
      ngx_http_postponed_request_t     *postponed;
      ngx_http_post_subrequest_t       *post_subrequest;
      ngx_http_posted_request_t        *posted_requests;
  
      ngx_int_t                         phase_handler;
      ngx_http_handler_pt               content_handler;
      ngx_uint_t                        access_code;
  
      ngx_http_variable_value_t        *variables;
  
  #if (NGX_PCRE)
      ngx_uint_t                        ncaptures;
      int                              *captures;
      u_char                           *captures_data;
  #endif
  
      size_t                            limit_rate;
      size_t                            limit_rate_after;
  
      /* used to learn the Apache compatible response length without a header */
      size_t                            header_size;
  
      off_t                             request_length;
  
      ngx_uint_t                        err_status;
  
      ngx_http_connection_t            *http_connection;
      ngx_http_v2_stream_t             *stream;
  
      ngx_http_log_handler_pt           log_handler;
  
      ngx_http_cleanup_t               *cleanup;
  
      unsigned                          count:16;
      unsigned                          subrequests:8;
      unsigned                          blocked:8;
  
      unsigned                          aio:1;
  
      unsigned                          http_state:4;
  
      /* URI with "/." and on Win32 with "//" */
      unsigned                          complex_uri:1;
  
      /* URI with "%" */
      unsigned                          quoted_uri:1;
  
      /* URI with "+" */
      unsigned                          plus_in_uri:1;
  
      /* URI with " " */
      unsigned                          space_in_uri:1;
  
      unsigned                          invalid_header:1;
  
      unsigned                          add_uri_to_alias:1;
      unsigned                          valid_location:1;
      unsigned                          valid_unparsed_uri:1;
      unsigned                          uri_changed:1;
      unsigned                          uri_changes:4;
  
      unsigned                          request_body_in_single_buf:1;
      unsigned                          request_body_in_file_only:1;
      unsigned                          request_body_in_persistent_file:1;
      unsigned                          request_body_in_clean_file:1;
      unsigned                          request_body_file_group_access:1;
      unsigned                          request_body_file_log_level:3;
      unsigned                          request_body_no_buffering:1;
  
      unsigned                          subrequest_in_memory:1;
      unsigned                          waited:1;
  
  #if (NGX_HTTP_CACHE)
      unsigned                          cached:1;
  #endif
  
  #if (NGX_HTTP_GZIP)
      unsigned                          gzip_tested:1;
      unsigned                          gzip_ok:1;
      unsigned                          gzip_vary:1;
  #endif
  
  #if (NGX_PCRE)
      unsigned                          realloc_captures:1;
  #endif
  
      unsigned                          proxy:1;
      unsigned                          bypass_cache:1;
      unsigned                          no_cache:1;
  
      /*
       * instead of using the request context data in
       * ngx_http_limit_conn_module and ngx_http_limit_req_module
       * we use the bit fields in the request structure
       */
      unsigned                          limit_conn_status:2;
      unsigned                          limit_req_status:3;
  
      unsigned                          limit_rate_set:1;
      unsigned                          limit_rate_after_set:1;
  
  #if 0
      unsigned                          cacheable:1;
  #endif
  
      unsigned                          pipeline:1;
      unsigned                          chunked:1;
      unsigned                          header_only:1;
      unsigned                          expect_trailers:1;
      unsigned                          keepalive:1;
      unsigned                          lingering_close:1;
      unsigned                          discard_body:1;
      unsigned                          reading_body:1;
      unsigned                          internal:1;
      unsigned                          error_page:1;
      unsigned                          filter_finalize:1;
      unsigned                          post_action:1;
      unsigned                          request_complete:1;
      unsigned                          request_output:1;
      unsigned                          header_sent:1;
      unsigned                          expect_tested:1;
      unsigned                          root_tested:1;
      unsigned                          done:1;
      unsigned                          logged:1;
  
      unsigned                          buffered:4;
  
      unsigned                          main_filter_need_in_memory:1;
      unsigned                          filter_need_in_memory:1;
      unsigned                          filter_need_temporary:1;
      unsigned                          preserve_body:1;
      unsigned                          allow_ranges:1;
      unsigned                          subrequest_ranges:1;
      unsigned                          single_range:1;
      unsigned                          disable_not_modified:1;
      unsigned                          stat_reading:1;
      unsigned                          stat_writing:1;
      unsigned                          stat_processing:1;
  
      unsigned                          background:1;
      unsigned                          health_check:1;
  
      /* used to parse HTTP headers */
  
      ngx_uint_t                        state;
  
      ngx_uint_t                        header_hash;
      ngx_uint_t                        lowcase_index;
      u_char                            lowcase_header[NGX_HTTP_LC_HEADER_LEN];
  
      u_char                           *header_name_start;
      u_char                           *header_name_end;
      u_char                           *header_start;
      u_char                           *header_end;
  
      /*
       * a memory that can be reused after parsing a request line
       * via ngx_http_ephemeral_t
       */
  
      u_char                           *uri_start;
      u_char                           *uri_end;
      u_char                           *uri_ext;
      u_char                           *args_start;
      u_char                           *request_start;
      u_char                           *request_end;
      u_char                           *method_end;
      u_char                           *schema_start;
      u_char                           *schema_end;
      u_char                           *host_start;
      u_char                           *host_end;
      u_char                           *port_start;
      u_char                           *port_end;
  
      unsigned                          http_minor:16;
      unsigned                          http_major:16;
  };
  ```
  </details>
  
  - 对于**nginx**来说，一个请求是从**ngx_http_init_request**开始的，在这个函数中，会设置读事件为**ngx_http_process_request_line**，也就是说，接下来的网络事件，会由**ngx_http_process_request_line**来执行，处理请求行。通过**ngx_http_read_request_header**来读取请求数据。然后调用**ngx_http_parse_request_line**函数来解析请求行。
  
  - **nginx**为提高效率，采用状态机来解析请求行，而且在进行**method**的比较时，没有直接使用字符串比较，而是将四个字符转换成一个整型，然后一次比较以减少**cpu**的指令数。
  
  - 整个请求行解析到的参数，会保存到**ngx_http_request_t**结构当中。
  
  - 在解析完请求行后，**nginx**会设置读事件的**handler**为**ngx_http_process_request_headers**，然后后续的请求就在**ngx_http_process_request_headers**中进行读取与解析。 
  
  - **ngx_http_process_request_headers**函数用来读取请求头，跟请求行一样，还是调用**ngx_http_read_request_header**来读取请求头，调用**ngx_http_parse_header_line**来解析一行请求头，解析到的请求头会保存到**ngx_http_request_t**的域**headers_in**中，**headers_in**是一个链表结构，保存所有的请求头。
  
  - 而**HTTP**中有些请求是需要特别处理的，这些请求头与请求处理函数存放在一个映射表里面，即**ngx_http_headers_in**，在初始化时，会生成一个**hash**表，当每解析到一个请求头后，就会先在这个**hash**表中查找，如果有找到，则调用相应的处理函数来处理这个请求头。
  
  - 当**nginx**解析到两个回车换行符时，就表示请求头的结束，此时就会调用**ngx_http_process_request**来处理请求了。**ngx_http_process_request**会设置当前的连接的读写事件处理函数为**ngx_http_request_handler**，然后再调用**ngx_http_handler**来真正开始处理一个完整的**http**请求。读写事件处理函数都是**ngx_http_request_handler**，其实在这个函数中，会根据当前事件是读事件还是写事件，分别调用**ngx_http_request_t**中的**read_event_handler**或者是**write_event_handler**。由于此时，我们的请求头已经读取完成了，**nginx**先不读取请求**body**，设置**read_event_handler**为**ngx_http_block_reading**，即不读取数据了。
  
  - 真正开始处理数据，是在**ngx_http_handler**这个函数里面，这个函数会设置 **write_event_handler**为**ngx_http_core_run_phases**，并执行**ngx_http_core_run_phases**函数。**ngx_http_core_run_phases**这个函数将执行多阶段请求处理，**nginx**将一个**http**请求的处理分为多个阶段，那么这个函数就是执行这些阶段来产生数据。最终是调用**ngx_http_core_run_phases**来处理请求，产生的响应头会放在**ngx_http_request_t**的**headers_out**中。
  
  - **nginx**的各种阶段会对请求进行处理，最后会调用**filter**来过滤数据，对数据进行加工。这里的**filter**包括**header filter**与**body filter**，即对响应头或响应体进行处理。**filter**是一个链表结构，分别有**header filter**与**body filter**，先执行**header filter**中的所有**filter**，然后再执行**body filter**中的所有**filter**。在**header filter**中的最后一个**filter**，即**ngx_http_header_filter**， 这个**filter**将会遍历所有的响应头，最后需要输出的响应头在一个连续的内存，然后调用**ngx_http_write_filter**进行输出。**ngx_http_write_filter**是**body filter**中的最后一个，所以**nginx**首先的**body**信息，在经过一系列的**body filter**之后，最后也会调用**ngx_http_write_filter**来进行输出。**nginx**会将整个请求头都放在一个**buffer**里面，这个**buffer**的大小通过配置项**client_header_buffer_size**来设置，如果用户的请求头太大，这个 **buffer**装不下，那**nginx**就会重新分配一个新的更大的**buffer**来装请求头，这个大**buffer**可以通过**large_client_header_buffers**来设置，这个**large_buffer**这一组**buffer**。
  
  - 为了保存请求行或请求头的完整性，一个完整的请求行或请求头，需要放在一个连续的内存里面，所以，一个完整的请求行或请求头，只会保存在一个**buffer**里面。这样，如果请求行大于一个**buffer**的大小，就会返回**414**错误，如果一个请求头大小大于一个**buffer**大小，就会返回**400**错误。
  
    ![nginx处理http流程](./images/nginx处理http流程.png?raw=true)

####  1.3 Handler模块

- **Handler**模块就是接受来自客户端的请求并产生输出的模块。

- [自定义模块](./code/nginx/module)

  <details>
  <summary>自定义模块</summary>
  
  ```C
  /*
   * @Author: gongluck 
   * @Date: 2020-12-08 19:10:29 
   * @Last Modified by: gongluck
   * @Last Modified time: 2020-12-08 19:11:06
   */
  
  // ./configure --add-module=/mnt/e/Code/CVIP/code/nginx/module
  // sudo ./objs/nginx -c /mnt/e/Code/CVIP/code/nginx/module/hello.conf
  
  #include <ngx_http.h>
  #include <ngx_config.h>
  #include <ngx_core.h>
  
  static char *ngx_http_hello_module_set(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
  static ngx_int_t ngx_http_hello_module_handler(ngx_http_request_t *r);
  
  // 模块配置结构
  typedef struct
  {
      ngx_str_t hello_string;
  } ngx_http_hello_loc_conf_t;
  
  // 模块配置指令
  static ngx_command_t hello_commands[] = {
      {
          ngx_string("hello"),                 //配置指令的名称
          NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS, //该配置指令属性的集合
          ngx_http_hello_module_set,           //当nginx在解析配置的时候，如果遇到这个配置指令，将会把读取到的值传递给这个函数进行分解处理
          NGX_HTTP_LOC_CONF_OFFSET,            //指定当前配置项存储的内存位置,实际上是使用哪个内存池的问题。
          0,                                   //指定该配置项值的精确存放位置，一般指定为某一个结构体变量的字段偏移。
          NULL                                 //可以指向任何一个在读取配置过程中需要的数据，以便于进行配置读取的处理。
      },
      ngx_null_command};
  
  // 模块上下文结构
  static ngx_http_module_t hello_ctx = {
      NULL, //在创建和读取该模块的配置信息之前被调用
      NULL, //在创建和读取该模块的配置信息之后被调用
  
      NULL, //调用该函数创建本模块位于http block的配置信息存储结构。该函数成功的时候，返回创建的配置对象。失败的话，返回NULL。
      NULL, //调用该函数初始化本模块位于http block 的配置信息存储结构。该函数成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串。
  
      NULL, //调用该函数创建本模块位于http server block的配置信息存储结构，每个server block会创建一个。该函数成功的时候，返回创建的配置对象。失败的话，返回NULL。
      NULL, //因为有些配置指令既可以出现在http block，也可以出现在http server block中。那么遇到这种情况，每个server都会有自己存储结构来存储该server的配置，但是在这种情况下http block中的配置与server block中的配置信息发生冲突的时候，就需要调用此函数进行合并，该函数并非必须提供，当预计到绝对不会发生需要合并的情况的时候，就无需提供。当然为了安全起见还是建议提供。该函数执行成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串。
  
      NULL, //调用该函数创建本模块位于location block的配置信息存储结构。每个在配置中指明的location创建一个。该函数执行成功，返回创建的配置对象。失败的话，返回NULL。
      NULL, //与merge_srv_conf类似，这个也是进行配置值合并的地方。该函数成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串。
  };
  
  // ngx_http_hello_module
  ngx_module_t ngx_http_hello_module = {
      NGX_MODULE_V1,
      &hello_ctx,      /* module context */
      hello_commands,  /* module directives */
      NGX_HTTP_MODULE, /* module type */
      NULL,            /* init master */
      NULL,            /* init module */
      NULL,            /* init process */
      NULL,            /* init thread */
      NULL,            /* exit thread */
      NULL,            /* exit process */
      NULL,            /* exit master */
      NGX_MODULE_V1_PADDING};
  
  static char *ngx_http_hello_module_set(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
  {
      ngx_http_core_loc_conf_t *corecf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
      corecf->handler = ngx_http_hello_module_handler;
  
      return NGX_CONF_OK;
  }
  
  static ngx_int_t ngx_http_hello_module_handler(ngx_http_request_t *r)
  {
      ngx_str_t str = ngx_string("Hello, Nginx!");
      ngx_buf_t *b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
      if (b == NULL)
      {
          return NGX_HTTP_INTERNAL_SERVER_ERROR;
      }
  
      ngx_chain_t out;
      out.buf = b;
      out.next = NULL;
      b->pos = str.data;
      b->last = b->pos + str.len;
      b->memory = 1;   /* this buffer is in memory */
      b->last_buf = 1; /* this is the last buffer in the buffer chain */
  
      /* set the status line */
      r->headers_out.status = NGX_HTTP_OK;
      r->headers_out.content_length_n = str.len;
  
      /* send the headers of your response */
      ngx_int_t rc = ngx_http_send_header(r);
      if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
      {
          return rc;
      }
  
      /* send the buffer chain of your response */
      return ngx_http_output_filter(r, &out);
  }
  ```
  </details>
  

#### 1.4 Filter模块

- **过滤（filter）**模块是过滤响应头和内容的模块，可以对回复的头和内容进行处理。它的处理时间在获取回复内容之后，向用户发送响应之前。它的处理过程分为两个阶段，过滤**HTTP**回复的头部和主体，在这两个阶段可以分别对头部和主体进行修改。

- 所有模块的响应内容要返回给客户端，都必须调用这两个接口

  ```C
  //分别对头部和主体进行过滤的函数
  ngx_http_top_header_filter(r); 
  ngx_http_top_body_filter(r, in); 
  ```

- 过滤模块的调用是有顺序的，它的顺序在编译的时候就决定了。控制编译的脚本位于**auto/modules**中，当编译完**Nginx**以后，可以在**objs**目录下面看到一个**ngx_modules.c**的文件。

  ```C
  ngx_module_t *ngx_modules[] = {
      &ngx_core_module,
      &ngx_errlog_module,
      &ngx_conf_module,
      &ngx_regex_module,
      &ngx_events_module,
      &ngx_event_core_module,
      &ngx_epoll_module,
      &ngx_http_module,
      &ngx_http_core_module,
      &ngx_http_log_module,
      &ngx_http_upstream_module,
      &ngx_http_static_module,
      &ngx_http_autoindex_module,
      &ngx_http_index_module,
      &ngx_http_mirror_module,
      &ngx_http_try_files_module,
      &ngx_http_auth_basic_module,
      &ngx_http_access_module,
      &ngx_http_limit_conn_module,
      &ngx_http_limit_req_module,
      &ngx_http_geo_module,
      &ngx_http_map_module,
      &ngx_http_split_clients_module,
      &ngx_http_referer_module,
      &ngx_http_rewrite_module,
      &ngx_http_proxy_module,
      &ngx_http_fastcgi_module,
      &ngx_http_uwsgi_module,
      &ngx_http_scgi_module,
      &ngx_http_memcached_module,
      &ngx_http_empty_gif_module,
      &ngx_http_browser_module,
      &ngx_http_upstream_hash_module,
      &ngx_http_upstream_ip_hash_module,
      &ngx_http_upstream_least_conn_module,
      &ngx_http_upstream_random_module,
      &ngx_http_upstream_keepalive_module,
      &ngx_http_upstream_zone_module,
      &ngx_http_hello_module,
      &ngx_http_write_filter_module,
      &ngx_http_header_filter_module,
      &ngx_http_chunked_filter_module,
      &ngx_http_range_header_filter_module,
      &ngx_http_gzip_filter_module,
      &ngx_http_postpone_filter_module,
      &ngx_http_ssi_filter_module,
      &ngx_http_charset_filter_module,
      &ngx_http_userid_filter_module,
      &ngx_http_headers_filter_module,//
      &ngx_http_copy_filter_module,//
      &ngx_http_range_body_filter_module,
      &ngx_http_not_modified_filter_module,
      NULL
  };
  ```

- 从**write_filter**到**not_modified_filter**，模块的执行顺序是反向的。所有第三方的模块只能加入到**copy_filter**和**headers_filter**模块之间执行。

- 在过滤模块中，所有输出的内容都是通过一条单向链表所组成。这种单向链表的设计，正好应和了**Nginx**流式的输出模式。每次**Nginx**都是读到一部分的内容，就放到链表，然后输出出去。这种设计的好处是简单，非阻塞，但是相应的问题就是跨链表的内容操作非常麻烦， 如果需要跨链表，很多时候都只能缓存链表的内容。单链表负载的就是**ngx_buf_t**，这个结构体使用非常广泛。

- 响应头过滤函数主要的用处就是处理**HTTP**响应的头，可以根据实际情况对于响应头进行修改或者添加删除。响应头过滤函数先于响应体过滤函数，而且只调用一次，所以一般可作过滤模块的初始化工作。

  ```C
  ngx_int_t
  ngx_http_send_header(ngx_http_request_t *r)
  {
      if (r->post_action) {
          return NGX_OK;
      }
  
      if (r->header_sent) {
          ngx_log_error(NGX_LOG_ALERT, r->connection->log, 0,
                        "header already sent");
          return NGX_ERROR;
      }
  
      if (r->err_status) {
          r->headers_out.status = r->err_status;
          r->headers_out.status_line.len = 0;
      }
  
      return ngx_http_top_header_filter(r);//
  }
  ```

- 可以把**HTTP**响应头的存储方式想象成一个**hash**表，在**Nginx**内部可以很方便地查找和修改各个响应头 部，**ngx_http_header_filter_module**过滤模块把所有的**HTTP**头组合成一个完整的**buffer**，最终**ngx_http_write_filter_module**过滤模块把**buffer**输出。

- 响应体过滤函数是过滤响应主体的函数。**ngx_http_top_body_filter**这个函数每个请求可能会被执行多次，它的入口函数是**ngx_http_output_filter**。

  ```C
  ngx_int_t
  ngx_http_output_filter(ngx_http_request_t *r, ngx_chain_t *in)
  {
      ngx_int_t          rc;
      ngx_connection_t  *c;
  
      c = r->connection;
  
      ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                     "http output filter \"%V?%V\"", &r->uri, &r->args);
  
      rc = ngx_http_top_body_filter(r, in);
  
      if (rc == NGX_ERROR) {
          /* NGX_ERROR may be returned by any filter */
          c->error = 1;
      }
  
      return rc;
  }
  ```

#### 1.5 Upstream模块

- 从本质上说，**upstream**属于**handler**，只是他不产生自己的内容，而是通过请求后端服务器得到内容，所以才称为**upstream**（上游）。
- 请求并取得响应内容的整个过程已经被封装到**nginx**内部，所以**upstream**模块只需要开发若干回调函数，完成构造请求和解析响应等具体的工作。
- **upstream**模块使用的就是**handler**模块的接入方式。同时，**upstream**模块的指令系统的设计也是遵循**handler**模块的基本规则：配置该模块才会执行该模块。

#### 1.6 负载均衡模块

- 负载均衡模块用于从**upstream**指令定义的后端主机列表中选取一台主机。**nginx**先使用负载均衡模块找到一台主机，再使用**upstream**模块实现与这台主机的交互。
- 核心指令**ip_hash**只能在**upstream {}**中使用。这条指令用于通知**nginx**使用**ip hash**负载均衡算法。如果没加这条指令，**nginx**会使用默认的**round robin**负载均衡模块。

### 2.Skynet

#### 2.1 环境安装

```shell
git clone https://github.com/cloudwu/skynet.git
cd skynet
make linux -j 8
```

#### 2.2 Skynet工作模型

![Skynet工作模型](./images/Skynet工作模型.png?raw=true)

- **skynet**中的**actor**模型 

  - 结构组成 

    - 隔离的环境（内存块或**lua**虚拟机）
    - 消息队列
    - 回调函数

  - 实现
    - **logger**服务**service-src/service_logger.c**
    - **lua**服务启动器**service-src/service_snlua.c**

#### 2.3 [Skynet Lua例子](./code/skynet)

<details>
<summary>Skynet lua</summary>

```lua
-- ./skynet /mnt/e/Code/CVIP/code/skynet/test.conf

local skynet = require "skynet"
local socket = require "skynet.socket"

local function event_loop(clientfd)
    while true do
        local data = socket.readline(clientfd)--从网络获取 以\n为分隔符的数据包
        if not data then
            return
        end
        print(clientfd, "recv:", data)
        socket.write(clientfd, data.."\n")
    end
end

local function accept(clientfd, addr)-- 回调函数的作用 就是可以将 fd绑定到其他actor
    print("accept a connect:", clientfd, addr)
    socket.start(clientfd) -- 将clientfd注册到epoll
    skynet.fork(event_loop, clientfd) -- 实现一个简单的echo服务，可以通过 telnet 127.0.0.1 8001来连接skynet
end

skynet.start(function ()
    local listenfd = socket.listen("0.0.0.0", 8001) -- socket bind listen 
    socket.start(listenfd, accept) -- 将listenfd注册到epoll，收到连接会回调accept函数
end)
```
</details>

### 3.ZeroMQ

#### 3.1 环境安装

```shell
git clone https://github.com/zeromq/libzmq.git
cd libzmq/
./autogen.sh
./configure --enable-debug
make -j 8
sudo make install
sudo ldconfig
```

#### 3.2 [例子代码](./code/zeromq)

<details>
<summary>server.c</summary>

```C
/*
 * @Author: gongluck 
 * @Date: 2020-12-10 02:27:44 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-12-10 02:37:16
 */

// gcc -o server server.c -lzmq -g -O0

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main()
{
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");

    while (1)
    {
        char buffer[10];
        int ret = zmq_recv(responder, buffer, 10, 0);
        printf("收到%.*s\n", ret, buffer);
        zmq_send(responder, "server recved.", strlen("server recved."), 0);
    }
    
    return 0;
}
```
</details>

<details>
<summary>client.c</summary>

```C
/*
 * @Author: gongluck 
 * @Date: 2020-12-10 02:33:38 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-12-10 02:37:00
 */

// gcc -o client client.c -lzmq -g -O0

#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");

    for (int request_nbr = 0; request_nbr != 10; request_nbr++)
    {
        char buffer[20] = {0};
        zmq_send(requester, "Hello", 5, 0);
        zmq_recv(requester, buffer, 20, 0);
        printf("接收到 %.20s\n", buffer);
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}
```
</details>

### 4.Redis

#### 4.1 Redis的元素结构

- **hash**值取余的**table**数组+**hash**表

- **hash**表

  ```C
  /* This is our hash table structure. Every dictionary has two of this as we
   * implement incremental rehashing, for the old to the new table. */
  typedef struct dictht {
      dictEntry **table;//table属性是⼀个数组，数组中的每个元素都是⼀个指向dict.h/dictEntry 结构的指针，每个dictEntry结构保存着⼀个键值对
      unsigned long size;//size属性记录了哈希表的⼤⼩，也即是table数组的⼤⼩
      unsigned long sizemask;//sizemask属性的值总是等于size - 1， 这个属性和哈希值⼀起决定⼀个键应该被放到table数组的哪个索引上⾯
      unsigned long used;//used属性则记录了哈希表⽬前已有节点（键值对）的数量
  } dictht;
  ```

- **hash**表节点

  ```C
  typedef struct dictEntry {
      void *key;
      union {
          void *val;
          uint64_t u64;
          int64_t s64;
          double d;
      } v;
      struct dictEntry *next;//next属性是指向另⼀个哈希表节点的指针，这个指针可以将多个哈希值相同的键值对连接在⼀次，以此来解决键冲突（collision）的问题。
  } dictEntry;
  ```

![redis哈希表结构](./images/redis哈希表结构.png?raw=true)

- 字典

  ```C
  typedef struct dict {
      dictType *type;//type属性是⼀个指向dictType结构的指针，每个dictType结构保存了⼀簇⽤于操作特定类型键值对的函数，Redis会为⽤途不同的字典设置不同的类型特定函数。
      void *privdata;//privdata属性保存了需要传给dictType类型特定函数的可选参数。
      dictht ht[2];//ht属性是⼀个包含两个项的数组，数组中的每个项都是⼀个dictht哈希表，⼀般情况下， 字典只使⽤ht[0]哈希表，ht[1]哈希表只会在对ht[0]哈希表进⾏rehash时使⽤。
      long rehashidx;//记录了rehash⽬前的进度，如果⽬前没有在进⾏rehash，那么它的值为-1。
      unsigned long iterators; /* number of iterators currently running */
  } dict;
  typedef struct dictType {
      uint64_t (*hashFunction)(const void *key);
      void *(*keyDup)(void *privdata, const void *key);
      void *(*valDup)(void *privdata, const void *obj);
      int (*keyCompare)(void *privdata, const void *key1, const void *key2);
      void (*keyDestructor)(void *privdata, void *key);
      void (*valDestructor)(void *privdata, void *obj);
  } dictType;
  ```

  ![redis字典结构](./images/redis字典结构.png?raw=true)

#### 4.2 Rehash

- 随着操作的不断执⾏，哈希表保存的键值对会逐渐地增多或者减少，为了让哈希表的负载因⼦（ratio）维持在⼀个合理的范围之内，当哈希表保存的键值对数量太多或者太少时，程序需要对哈希表的⼤⼩进⾏相应的扩展或者收缩。

  ```C
  ratio = ht[0].used / ht[0].size
  ```

- 扩展和收缩哈希表的⼯作可以通过执⾏**rehash**（重新散列）操作来完成，**Redis**对字典的哈希表执⾏**rehash**的策略如下：

  - 如果**ratio**⼩于0.1，则会对**hash**表进⾏收缩操作
  - 服务器⽬前没有在执⾏**BGSAVE**命令或者**BGREWRITEAOF**命令，并且哈希表的负载因⼦⼤于/等于1，则扩容**hash**表，扩容⼤⼩为当前**ht[0].used*2**
  - 服务器⽬前正在执⾏**BGSAVE**命令或者**BGREWRITEAOF**命令，并且哈希表的负载因⼦⼤于/等于5，则扩容**hash**表，并且扩容⼤⼩为当前**ht[0].used*2**

- 扩容的步骤如下

  - 为字典**ht[1]**哈希表分配合适的空间
  - 将**ht[0]**中所有的键值对**rehash**到**ht[1]**（**rehash**指的是重新计算键的哈希值和索引值，然后将键值对放置到**ht[1]**哈希表的指定位置上）
  - 当**ht[0**包含的所有键值对都迁移到了**ht[1]**之后（**ht[0]**变为空表），释放**ht[0]**，将**ht[1]**设置为**ht[0]**，并在**ht[1]**新创建⼀个空⽩哈希表，为下⼀次**rehash**做准备

- 为了避免**rehash**对服务器性能造成影响，服务器不是⼀次性将**ht[0]**⾥⾯的所有键值对全部**rehash**到**ht[1]**，⽽是分多次、渐进式地将**ht[0]**⾥⾯的键值对慢慢地**rehash**到**ht[1]**

  - 为**ht[1]**分配空间，让字典同时持有**ht[0]**和**ht[1]**两个哈希表
  - 在字典中维持⼀个索引计数器变量**rehashidx**，并将它的值设置为0，表示**rehash**⼯作正式开始
  - 在**rehash**进⾏期间，每次对字典执⾏添加、删除、查找或者更新操作（甚至后台启动定时器）时，程序除了执⾏指定的操作以外，还会顺带将**ht[0]**哈希表在**rehashidx**索引上的所有键值对**rehash**到**ht[1]**，当**rehash**⼯作完成之后，程序将**rehashidx**属性的值增⼀
  - 随着字典操作的不断执⾏，最终在某个时间点上，**ht[0]**的所有键值对都会被**rehash**⾄**ht[1]**，这时程序将 **rehashidx**属性的值设为**-1**，表示rehash操作已完成

#### 4.3 主从复制

- **redis**为了实现⾼可⽤（⽐如解决单点故障的问题），会把数据复制多个副本部署到其他节点上，通过复制，实现**Redis**的⾼可⽤性，实现对数据的冗余备份，保证数据和服务的可靠性。

- 如何配置：

  - 配置⽂件：在从服务器的配置⽂件中加⼊：**slaveof**
  - 启动命令：**redis-server**启动命令后加⼊**--slaveof**
  - 客户端命令：**Redis**服务器启动后，直接通过客户端执⾏命令：**slaveof**，则该**Redis**实例成为从节点。
    - PS：通过**info replication**命令可以看到复制的⼀些信息
    - ⽆论是通过哪⼀种⽅式来建⽴主从复制，都是从节点来执⾏**slaveof**命令

- 主从复制的作⽤

  - 数据冗余：主从复制实现了数据的热备份，是持久化之外的⼀种数据冗余⽅式。
  - 故障恢复：当主节点出现问题时，可以由从节点提供服务，实现快速的故障恢复；实际上是⼀种服务的 冗余。
  - 负载均衡：在主从复制的基础上，配合读写分离，可以由主节点提供写服务，由从节点提供读服务（即 写**Redis**数据时应⽤连接主节点，读**Redis**数据时应⽤连接从节点），分担服务器负载；尤其是在写少读多的场景下，通过多个从节点分担读负载，可以⼤⼤提⾼**Redis**服务器的并发量。
  - 读写分离：可以⽤于实现读写分离，主库写、从库读，读写分离不仅可以提⾼服务器的负载能⼒，同时 可根据需求的变化，改变从库的数量。
  - ⾼可⽤基⽯：除了上述作⽤以外，主从复制还是哨兵和集群能够实施的基础，因此说主从复制是**Redis** ⾼可⽤的基础。

- **Redis**的主从复制功能除了⽀持⼀个**Master**节点对应多个**Slave**节点的同时进⾏复制外，还⽀持**Slave**节点向其它多个**Slave**节点进⾏复制。这样就使得架构师能够灵活组织业务缓存数据的传播，例如使⽤多个**Slave**作为数据读取服务的同时，专⻔使⽤⼀个**Slave**节点为流式分析⼯具服务。**Redis**的主从复制功能分为两种数据同步模式进⾏：全量数据同步和增量数据同步。

  - 先执⾏⼀次全同步

    - 请求**master** **BgSave**出⾃⼰的⼀个**RDB Snapshot**⽂件发给**slave**，**slave**接收完毕后，清除掉⾃⼰的旧数据，然后将**RDB**载⼊内存。

    - 当**Slave**节点给定的**replication id**和**Master的replication id**不⼀致时，或者**Slave**给定的上⼀次增量同步的**offset**的位置在**Master**的环形内存中（**replication backlog**）⽆法定位时，**Master**就会对**Slave**发起全量同步操作。这时⽆论是否在**Master**打开了**RDB**快照功能，它和**Slave**节点的每⼀次全量同步操作过程都会更新/创建**Master**上的**RDB**⽂件。在**Slave**连接到**Master**，并完成第⼀次全量数据同步后，接下来**Master**到**Slave**的数据同步过程⼀般就是增量同步形式了（也称为部分同步）。增量同步过程不再主要依赖**RDB**⽂件，**Master**会将新产⽣的数据变化操作存放在**replication backlog**这个内存缓存区，这个内存区域是⼀个环形缓冲区，也就是说是⼀个**FIFO**的队列。

      ![redis完全同步](./images/redis完全同步.png?raw=true)

  - 之后执行增量同步

    - **master**作为⼀个普通的**client**连⼊**slave**，将所有写操作转发给**slave**，没有特殊的同步协议。

      ![redis增量同步](./images/redis增量同步.png?raw=true)

      

## 十、中间件开发专题

### 1.用户态协议栈

#### 1.1 Netmap

- **Netmap**是一个高性能收发原始数据包的框架，由**Luigi Rizzo**等人开发完成，其包含了内核模块以及用户态库函数。其目标是，不修改现有操作系统软件以及不需要特殊硬件支持，实现用户态和网卡之间数据包的高性能传递。数据包不经过操作系统内核进行处理，用户空间程序收发数据包时，直接与网卡进行通信。

  ![netmap原理](./images/netmap原理.png?raw=true)

- 网卡通过循环队列（即**NIC**环）来管理数据包的传输，每个网卡至少维护一对**NIC**环，分别用以管理发送和接收两个方向的数据包。每一个环被划分为很多槽，每一个槽都包含存储数据包的缓冲区的信息：缓冲区长度、缓冲区的物理地址等。在通常情况下，主机网络堆栈可以访问**NIC**环上指定的槽，也就可以访问到存放数据包的缓冲区，实现数据包的发送和接收。

  ![内核处理网络数据包框架](./images/内核处理网络数据包框架.png?raw=true)

- 网卡所管理的内存空间是内核空间，运行在用户态的应用程序正常情况下无权访问内核空间，因此，需要进行从内核空间到用户空间的拷贝，零拷贝就是减少或消除这种拷贝，如直接缓存访问（**direct buffer access**)。**DBA**为了节省内核态和用户态之间的拷贝，可以将应用程序直接跑在内核态，如内核态的**Click**。也可以选择将内核中的数据包缓存区直接暴露给用户态程序，如**PF_RING**和**Linux**的**PACKET_MMAP**。

- **netmap**也是一个基于零拷贝思想的高速网络**I/O**架构。当网卡运行在**netmap**模式下，**NIC**环会与主机协议栈进行断开，**netmap**会拷贝一份**NIC**环，被称作**netmap**环。同时，**netmap**还会维护一对环，用于与主机协议栈进行交互。这些环所指向的用于存储数据包内容的缓存位于共享空间，网卡直接将数据包存入这些缓存。应用程序可以通过调用**netmap API**访问**netmap**环中的数据包内容，也就可以访问用于存储数据包的缓存，也就是说，当应用程序需要访问数据包内容时，无需从内核空间到用户空间的拷贝，可以直接访问，从而实现了网络数据包的零拷贝。此外，**netmap**并不会将网卡寄存器和内核的关键内存区域暴露给应用程序，因而用户态的应用程序并不会导致操作系统崩溃，所以相对一些其他的零拷贝架构，**netmap**更为安全。

  ![netmap数据结构](./images/netmap数据结构.png?raw=ture)

- **netmap**还会通过以下几种手段来增加网络I/O的速度：

  - 1）预分配固定大小的数据包存储空间，以消除每个数据包存储时动态分配内存所导致的系统开销；
  - 2）让用户态程序直接访问到网络数据包缓冲区，以减少数据拷贝的开销；
  - 3）使用一个轻量级的元数据表示，以屏蔽硬件相关的一些特性。该元数据表示支持在一次系统调用中处理大量数据包，从而可以减少系统调用的开销。以发包速度为例，**netmap**可以在900MHz单核**CPU**上处理10G以太网的线速（14.88Mpps）。

- 编译安装

  ```shell
  git clone https://github.com/luigirizzo/netmap.git
  ./configure
  make -j 8
  sudo make install
  sudo insmod netmap.ko
  ```

#### 1.2 用户态协议栈

- 以太网帧格式

  ![以太网帧格式](./images/以太网帧格式.png?raw=ture)

- **IP**协议头格式

  ![ip协议头格式](./images/ip协议头格式.png?raw=true)

- **UDP**头部格式

  ![udp头部格式](./images/udp头部格式.png?raw=true)

- **TCP**头部格式

  ![tcp头部格式](./images/tcp头部格式.png?raw=true)

### 2.定时器

### 3.分布式锁

### 4.协程

#### 4.1 知识准备

- 现代操作系统是分时操作系统，资源分配的基本单位是进程，CPU调度的基本单位是线程。
- C++程序运行时会有一个运行时栈，一次函数调用就会在栈上生成一个record。
- 运行时内存空间分为全局变量区（存放函数，全局变量）,栈区，堆区。栈区内存分配从高地址往低地址分配，堆区从低地址往高地址分配。
- 下一条指令地址存在于指令寄存器**IP**，**ESP**寄存值指向当前栈顶地址，**EBP**指向当前活动栈帧的基地址。
- 发生函数调用时操作为：将参数从右往左一次压栈，将返回地址压栈，将当前**EBP**寄存器的值压栈，在栈区分配当前函数局部变量所需的空间，表现为修改**ESP**寄存器的值。
- 协程的上下文包含属于他的栈区和寄存器里面存放的值。
- 协程是为了使用异步的优势，异步操作是为了避免**IO**操作阻塞线程。那么协程挂起的时刻应该是当前协程发起异步操作的时候，而唤醒应该在其他协程退出，并且他的异步操作完成时。

#### 4.2 协程实现

- 协程发起异步操作的时刻是该挂起协程的时刻，为了保证唤醒时能正常运行，需要正确保存并恢复其运行时的上下文：
  - 保存当前协程的上下文（运行栈，返回地址，寄存器状态）
  - 设置将要唤醒的协程的入口指令地址到**IP**寄存器
  - 恢复将要唤醒的协程的上下文

### 5.连接池

#### 5.1 资源重用

- 由于数据库连接得到重用，避免了频繁的创建、释放连接引起的性能开销，在减少系统消耗的基础上，另一方面也增进了系统运行环境的平稳性（减少内存碎片以及数据库临时进程/线程的数量）。 

#### 5.2 更快的系统响应速度

- 数据库连接池在初始化过程中，往往已经创建了若干数据库连接置于池中备用。此时连接的初始化工作均已完成。对于业务请求处理而言，直接利用现有可用连接，避免了从数据库连接初始化和释放过程的开销，从而缩减了系统整体响应时间。 

#### 5.3 统一的连接管理，避免数据库连接泄露

- 在较为完备的数据库连接池实现中，可根据预先的连接占用超时设定，强制收回被占用连接。从而避免了常规数据库连接操作中可能出现的资源泄露。

### 6.RPC（远程过程调用）

#### 6.1 RPC框架

![rpc框架](./images/rpc框架.png?raw=true)

#### 6.2 RPC实现

- 基于**TCP**协议实现的**RPC**调用，由于**TCP**协议处于协议栈的下层，能够更加灵活地对协议字段进行定制，减少网络开销，提高性能，实现更大的吞吐量和并发数。
- 基于**HTTP**协议实现的**RPC**则可以使用**JSON**和**XML**格式的请求或响应数据。**HTTP**协议是上层协议，发送包含同等内容的信息，使用**HTTP**协议传输所占用的字节数会比使用**TCP**协议传输 所占用的字节数更高。

### 7.异常捕获

#### 7.1 异常捕获原理

- 从原理上来讲，**throw**其实就是一个跳转，跳转到由**try-catch**块包围的**catch**块处。在这里，我们用两个函数来实现这个功能：

  ```C
  int setjmp(jmp_buf env);
  void longjmp(jmp_buf env, int val);
  ```

  - **setjmp**函数记录调用时的当前状态，如**IP**、**ESP**等，并且返回0。状态被写在一个**jmp_buf**对象中，这个对象其实是一个**int**数组。比较有趣的是**longjmp**函数，这个函数传入一个**jmp_buf**以及一个整形。它会立即跳转到当时的**setjmp**处，并且返回值是**longjmp**中的第二个参数。也就是说，这个函数可能会被调用两次，从某种程度上来说，有点像**fork()**。

    ![setjmp_longjmp](./images/setjmp_longjmp.png?raw=true)

- 在**try-catch**中，**try**函数充当着**setjmp**的功能。当**setjmp**返回0时（也就是第一次执行），执行**try**块的代码，当返回非0时，说明有**longjmp**被调用，此时发生异常，跳入**catch**块。同时，**throw**就相当于**longjmp**，可以跳转到包含它的**catch**块中。
- **longjmp**的第一个参数**jmp_buf**，其实是在**try**块中创建的。我们怎么来获取到上一个**try**块中创建的**jmp_buf**呢？我们可以如同操作系统创建一个运行时栈那样，我们也创建一个**try-catch**堆栈。在**try**时，我们把创建的**jmp_buf**压入，在**throw**时，我们把**jmp_buf**弹出。为了线程安全，我们得为每一个线程分配这样一个栈。

## 十一、Linux内核专题

### 1.OpenResty

#### 1.1 简介

- **openresty**是一个基于**nginx**与**lua**的高性能**web**平台，其内部集成了大量精良的**lua**库、第三方模块以及大多数的依赖项。用于方便搭建能够处理超高并发、扩展性极高的动态**web**应用、**web**服务和动态网关。
- **openresty**通过汇聚各种设计精良的**nginx**模块，从而将**nginx**有效地变成一个强大的通用 **Web**应用平台。这样，**Web**开发人员和系统工程师可以使用**Lua**脚本语言调动**Nginx**支持的各种**C**以及**Lua**模块，快速构造出足以胜任**10K**乃至**1000K**以上单机并发连接的高性能**Web**应用系统。
- **openresty**的目标是让**Web**服务直接跑在**Nginx**服务内部，充分利用**Nginx**的非阻塞**I/O**模型，不仅仅对**HTTP**客户端请求，甚至于对远程后端诸如**MySQL、PostgreSQL、Memcached**以及**Redis**等都进行一致的高性能响应。

#### 1.2 lua-nginx-module

![lua-nginx-module](./images/lua-nginx-module.png?raw=true)

#### 1.3 cosocket

![cosocket](./images/cosocket.png?raw=true)

#### 1.4 环境搭建

```Shell
apt-get install libpcre3-dev libssl-dev perl make build-essential curl -y
wget https://openresty.org/download/openresty-1.19.3.1.tar.gz
tar -xzvf openresty-1.19.3.1.tar.gz
cd openresty-1.19.3.1/
./configure
make -j 8
sudo make install
```

### 2.Linux内核编程

#### 2.1 Linux内核编译升级

```Shell
wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.10.tar.xz
tar -xvf linux-5.10.tar.xz
cd linux-5.10
cp /boot/config-xxx ./.config
make menuconfig
make -j 8
sudo su
make modules_install
make bzImage

cp arch/x86/boot/bzImage /boot/vmlinuz-4.4.16
cp .config /boot/config-4.4.16
cd /lib/modules/4.4.16/
update-initramfs –c –k 4.4.16
update-grub
```

#### 2.2 linux整体架构与子系统划分

![linux整体架构与子系统划分](./images/linux整体架构与子系统划分.png?raw=true)

## 十二、集群专题

### 1.Redis集群

#### 1.1 主从复制

- 主从模式的一个作用是备份数据，这样当一个节点损坏时，数据因为有备份，可以方便恢复。另一个作用是负载均衡，所有客户端都访问一个节点肯定会影响**Redis**工作效率，有了主 从以后，查询操作就可以通过查询从节点来完成。
- 默认配置下，**master**节点可以进行读和写，**slave**节点只能进行读操作，写操作被禁止。
- 不要修改配置让**slave**节点支持写操作，没有意义，原因一，写入的数据不会被同步到其他节点；原因二，当**master**节点修改同一条数据后，**slave**节点的数据会被覆盖掉。
- **slave**节点挂了不影响其他**slave**节点的读和**master**节点的读和写，重新启动后会将数据从**master**节点同步过来。**master**节点挂了以后，不影响**slave**节点的读，**Redis**将不再提供写服务，**master**节点启动后**Redis**将重新对外提供写服务。**master**节点挂了以后，**slave**节点重新不会选一个**master**。

#### 1.2 Sentinel模式

- **Sentinel**的主要功能包括主节点存活检测、主从运行情况检测、自动故障转移、主从切换。由一个或多个**Sentinel**实例组成的**Sentinel**系统可以监视任意多个主服务器，以及所有从服务器，并在被监视的主服务器进入下线状态时，自动将下线主服务器属下的某个从服务器升 级为新的主服务器，然后由新的主服务器代替已下线的主服务器继续处理命令请求。

  ![redis哨兵模式](./images/redis哨兵模式.png?raw=true)

- **Redis**的**Sentinel**最小配置是一主一从。**Redis**的**Sentinel**系统可以用来管理多个**Redis**服务器，该系统可以执行以下四个任务：
  - 监控
    - **Sentinel**会不断的检查主服务器和从服务器是否正常运行。 
  - 通知
    - 当被监控的某个**Redis**服务器出现问题，**Sentinel**通过**API**脚本向管理员或者其他的应用程序发送通知。
  - 自动故障转移
    - 当主节点不能正常工作时，**Sentinel**会开始一次自动的故障转移操作，它会将与失效主节点是主从关系的其中一个从节点升级为新的主节点，并且将其他的从节点指向新的主节点。
  - 配置提供者 
    - 在**Redis Sentinel**模式下，客户端应用在初始化时连接的是 **Sentinel**节点集合，从中获取主节点的信息。

#### 1.3 Cluster模式

- **redis cluster**是 **Redis**的分布式解决方案，在3.0版本推出后有效地解决了**redis**分布式方面的需求。自动将数据进行分片，每个**master**上放一部分数据。提供内置的高可用支持，部分**master**不可用时，还是可以继续工作的支撑N个**redis master node**，每个**master node**都可以挂载多个**slave node**高可用，因为每个**master**都有**salve**节点，那么如果**mater**挂掉，**redis cluster**这套机制，就会自动将某个**slave**切换成**master**。

  ![redis集群模式](./images/redis集群模式.png?raw=true)

### 2.去中心化技术

#### 2.1 去中心化网络

![去中心化网络](./images/去中心化网络.png?raw=true)

#### 2.2 网络地址映射

- 服务器发送数据与路由器公网**IP**时，能够将数据映射到私网中的机器；私网内的机器发送数据给服务器，路由器也能够映射为公网**IP**地址的过程，成为网络地址映射。

  ![NAT工作拓扑](./images/NAT工作拓扑.png?raw=true)
  
  

### 3.Docker

#### 3.1 镜像、容器、仓库

- **Docker**镜像（**Image**），就相当于是一个**root**文件系统。比如官方镜像**ubuntu:16.04**就包含了完整的一套**Ubuntu16.04**最小系统的**root**文件系统。 
- 容器是镜像运行时的实体。容器可以被创建、启动、停止、删除、暂停等。
- 仓库可看着一个代码控制中心，用来保存镜像。**Docker**使用客户端-服务器(**C/S**)架构模式，使用远程**API**来管理和创建**Docker**容器。 **Docker**容器通过**Docker**镜像来创建。

#### 3.2 Docker安装

```Shell
sudo apt-get remove docker docker-engine docker.io containerd runc
sudo apt-get update
sudo apt-get install apt-transport-https ca-certificates curl gnupg-agent software-properties-common
curl -fsSL https://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu/gpg | sudo apt-key add -
sudo apt-key fingerprint 0EBFCD88
sudo add-apt-repository "deb [arch=amd64] https://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu/ $(lsb_release -cs)  stable"

sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io
```

#### 3.3 Docker命令

```Shell
# 启动docker
sudo service docker start
# 停止docker
sudo service docker stop
# 重启docker
sudo service docker restart
```

### 4.MySQL集群

#### 4.1 Replication

- 主从复制是通过重放**binlog**实现主库数据的异步复制。即当主库执行了一条**sql**命令，那么在从库同样的执行一遍，从而达到主从复制的效果。

- 在这个过程中，**master**对数据的写操作记入二进制日志文件中(**binlog**)，生成一个**log dump**线程，用来给从库的**i/o**线程传**binlog。而从库的i/o线程去请求主库的binlog，并将得到的binlog**日志写到中继日志（**relaylog**）中，从库的**sql**线程，会读取**relaylog**文件中的日志，并解析成具体操作，通过主从的操作一致，而达到最终数据一致。

  ![mysql主从复制](./images/mysql主从复制.png?raw=true)

#### 4.2 Fabirc

- 在**MySQL Replication**的基础上，增加了故障检测与转移，自动数据分片功能。
- 依旧是一主多从的结构，**MySQL Fabirc**只有一个主节点，区别是当该主节点挂了以后，会从从节点中选择一个来当主节点。

#### 4.3 Cluster

- **MySQL Cluster**是多主多从结构的，高可用性优秀，99.999%的可用性，可以自动切分数据，能跨节点冗余数据（其数据集并不是存储某个特定的**MySQL**实例上，而是被分布在多个**Data Nodes**中，即一个**table**的数据可能被分散在多个物理节点上，任何数据都会在多个**Data Nodes**上冗余备份。任何一个数据变更操作，都将在一组**Data Nodes**上同步，以保证数据的一致性）。

### 5.Kubernetes

#### 5.1 K8s简介

- **Kubernetes**是一个开源的，用于管理云平台中多个主机上的容器化的应用，**Kubernetes**的目标是让部署容器化的应用简单并且高效，***Kubernetes***提供了应用部署，规划，更新，维护的一种机制。
- 传统的应用部署方式是通过插件或脚本来安装应用。这样做的缺点是应用的运行、配置、管理、所有生存周期将与当前操作系统绑定，这样做并不利于应用的升级更新/回滚等操作，当然也可以通过创建虚拟机的方式来实现某些功能，但是虚拟机非常重，并不利于可移植性。
- 通过部署容器方式，每个容器之间互相隔离，每个容器有自己的文件系统 ，容器之间进程不会相互影响，能区分计算资源。相对于虚拟机，容器能快速部署，由于容器与底层设施、机器文件系统解耦的，所以它能在不同云、不同版本操作系统间进行迁移。
- 容器占用资源少、部署快，每个应用可以被打包成一个容器镜像，每个应用与容器间成一对一关系也使容器有更大优势，使用容器可以在**build**或**release**的阶段，为应用创建容器镜像，因为每个应用不需要与其余的应用堆栈组合，也不依赖于生产环境基础结构，这使得从研发到测试、生产能提供一致环境。类似地，容器比虚拟机轻量、更“透明”，这更便于监控和管理。

#### 5.2 K8s集群

![K8s集群](./images/K8s集群.png?raw=true)

### 6.MongoDB集群

#### 6.1 Master-Slave

![mongodb主从集群](./images/mongodb主从集群.png?raw=true)

- 主机工作，备机处于监控准备状况；
- 当主机宕机时，备机接管主机的一切工作，待主机恢复正常后，按使用者的设定以自动或手动方式将服务切换到主机上运行，数据的一致性通过共享存储系统解决。
- 主(**Master**)可读可写，当数据有修改的时候，会将**oplog**同步到所有连接的**salve**上去。
- 从(**Slave**)只读不可写，自动从**Master**同步数据。

#### 6.2 Replica Set

![mongodb副本集集群](./images/mongodb副本集集群.png?raw=true)

- 为了防止单点故障就需要用副本（**Replication**），当发生硬件故障或者其它原因造成的宕机时，可以使用副本进行恢复，最好能够自动的故障转移（**failover**）。有时引入副本是为了读写分离，将读的请求分流到副本上，减轻主（**Primary**）的读压力。而**Mongodb**的**Replica Set**都能满足这些要求。
- **Replica Set**的一堆**mongod**的实例集合，它们有着同样的数据内容。包含三类角色：
  - 主节点（**Primary**）
    - 接收所有的写请求，然后把修改同步到所有**Secondary**。一个**Replica Set**只能有一个**Primary**节点，当**Primary**挂掉后，其他**Secondary**或者**Arbiter**节点会重新选举出来一个主节点。
    - 默认读请求也是发到**Primary**节点处理的，需要转发到**Secondary**需要客户端修改一下连接配置。
  - 副本节点（**Secondary**）
    - 与主节点保持同样的数据集。当主节点挂掉的时候，参与选主。
  - 仲裁者（**Arbiter**）
    - 不保有数据，不参与选主，只进行选主投票。使用**Arbiter**可以减轻数据存储的硬件需求，Arbiter跑起来几乎没什么大的硬件资源需求，但重要的一点是，在生产环境下它和其他数据节点不要部署在同一台机器上。
- 一个自动**failover**的**Replica Set**节点数必须为奇数，目的是选主投票的时候要有一个大多数才能进行选主决策。

#### 6.3 Sharding

![mongodb分片集群](./images/mongodb分片集群.png?raw=true)

- 当数据量比较大的时候，我们需要把数据分片运行在不同的机器中，以降低CPU、内存和IO的压力，**Sharding**就是这样的技术。
- 数据库主要由两种方式做**Sharding**：纵向，横向，纵向的方式就是添加更多的**CPU**，内存，磁盘空间等。横向就是上面说的方式。
- 数据分片（**Shards**）
  - 保存数据，保证数据的高可用性和一致性。可以是一个单独的**mongod**实例，也可以是一个副本集。
  - 在生产环境下**Shard**是一个**Replica Set**，以防止该数据片的单点故障。所有**Shard**中有一个**PrimaryShard**，里面包含未进行划分的数据集合。

- 查询路由（**Query Routers**）
  - **mongos**的实例，客户端直接连接**mongos**，由**mongos**把读写请求路由到指定的**Shard**上去。
  - 一个**Sharding**集群，可以有一个**mongos**，也可以有多**mongos**以减轻客户端请求的压力。

- 配置服务器（**Config servers**）
  - 保存集群的元数据（**metadata**），包含各个**Shard**的路由规则。

## 十三、分布式专题

### 1.UDP可靠传输

#### 1.1 TCP和UDP比较

![TCP和UDP](./images/TCP和UDP.png?raw=true)

#### 1.2 KCP

- **KCP**是一个快速可靠协议，能以比**TCP**浪费10%-20%的带宽的代价，换取平均延迟降低30%-40%，且最大延迟降低三倍的传输效果。纯算法实现，并不负责底层协议（如**UDP**）的收发，需要使用者自己定义下层数据包的发送方式，以**callback**的方式提供给**KCP**。 连时钟都需要外部传递进来，内部不会有任何一次系统调用。

- 整个协议只有**ikcp.h**，**ikcp.c**两个源文件，可以方便的集成到用户自己的协议栈中。

- **TCP**是为流量设计的（每秒内可以传输多少**KB**的数据），讲究的是充分利用带宽。而**KCP**是为流速设计的（单个数据包从一端发送到一端需要多少时间），以10%-20%带宽浪费的代价换取了比**TCP**快30%-40%的传输速度。**TCP**信道是一条流速很慢，但每秒流量很大的大运河，而**KCP**是水流湍急的小激流。**KCP**有正常模式和快速模式两种。

- **RTO**翻倍**vs**不翻倍：

  - **TCP**超时计算是**RTOx2**，这样连续丢三次包就变成**RTOx8**了，十分恐怖，而**KCP**启动快速模式后不**x2**，只是**x1.5**（实验证明1.5这个值相对比较好），提高了传输速度。

- 选择性重传**vs**全部重传：

  - **TCP**丢包时会全部重传从丢的那个包开始以后的数据，**KCP**是选择性重传，只重传真正丢失的数据包。

- 快速重传：

  - 发送端发送了1,2,3,4,5几个包，然后收到远端的**ACK**: 1, 3, 4, 5，当收到**ACK3**时，**KCP**知道2被跳过1次，收到**ACK4**时，知道2被跳过了2次，此时可以认为2号丢失，不用等超时，直接重传2号包，大大改善了丢包时的传输速度。

- 延迟**ACK** **vs** 非延迟**ACK**：

  - **TCP**为了充分利用带宽，延迟发送**ACK**（**NODELAY**都没用），这样超时计算会算出较大**RTT**时间，延长了丢包时的判断过程。**KCP**的**ACK**是否延迟发送可以调节。

- **UNA** **vs** **ACK+UNA**：

  - **ARQ**模型响应有两种，**UNA**（此编号前所有包已收到，如**TCP**）和**ACK**（该编号包已收到），光用UNA将导致全部重传，光用**ACK**则丢失成本太高，以往协议都是二选其一，而**KCP**协议中，除去单独的**ACK**包外，所有包都有**UNA**信息。

- 非退让流控：

  - **KCP**正常模式同**TCP**一样使用公平退让法则，即发送窗口大小由：发送缓存大小、接收端剩余接收缓存大小、丢包退让及慢启动这四要素决定。但传送及时性要求很高的小数据时，可选择通过配置跳过后两步，仅用前两项来控制发送频率。以牺牲部分公平性及带宽利用率之代价，换取了开着**BT**都能流畅传输的效果。
- [**KCP**使用例子](./code/kcp)

### 2.DPDK