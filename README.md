## 一、算法与设计模式专栏

### 1.查找与排序/KMP算法，栈/队列

![排序比较](./images/排序比较.png)

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

#### 1.4 KMP算法

- **Knuth-Morris-Pratt字符串查找算法**（简称为**KMP算法**）可在一个**主文本字符串**`S`内查找一个**词**`W`的出现位置。此算法通过运用对这个词在不匹配时本身就包含足够的信息来确定下一个匹配将在哪里开始的发现，从而避免重新检查先前匹配的字符。

- **部分匹配表**，又称为**失配函数**，作用是让算法无需多次匹配`S`中的任何字符。能够实现线性时间搜索的关键是在主串的一些字段中检查模式串的*初始字段*，我们可以确切地知道在当前位置之前的一个潜在匹配的位置。换句话说，在不错过任何潜在匹配的情况下，我们"预搜索"这个模式串本身并将其译成一个包含所有可能失配的位置对应可以绕过最多无效字符的列表。

- 对于`W`中的任何位置，我们都希望能够查询那个位置前（不包括那个位置）有可能的`W`的最长初始字段的长度，而不是从`W[0]`开始失配的整个字段，这长度就是我们查找下一个匹配时回退的距离。因此`T[i]`是`W`的可能的*适当*初始字段同时也是结束于`W[i - 1]`的子串的最大长度。我们使空串长度是0。当一个失配出现在模式串的最开始，这是特殊情况（无法回退），我们设置`T[0] = -1`。

- [代码](./code/search/kmp.c)

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


### 2.二叉树与红黑树

#### 2.1 二叉树

- **二叉树**是n(n>=0)个结点的有限集合，该集合或者为空集（称为空二叉树），或者由一个根结点和两棵互不相交的、分别称为根结点的左子树和右子树组成。
- 二叉树特点
  - 每个结点最多有两颗子树，所以二叉树中不存在度大于2的结点。
  - 左子树和右子树是有顺序的，次序不能任意颠倒。
  - 即使树中某结点只有一棵子树，也要区分它是左子树还是右子树。

- [代码](./code/tree/btree.c)

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


#### 2.2 红黑树

![红黑树](./images/红黑树.jpg)

-  R-B Tree，全称是Red-Black Tree，又称为“红黑树”，它一种特殊的二叉查找树。红黑树的每个节点上都有存储位表示节点的颜色，可以是红(Red)或黑(Black)。
- 红黑树的特性
  - 每个节点或者是黑色，或者是红色。
  - 根节点是黑色。
  - 每个叶子节点（NIL）是黑色。这里叶子节点，是指为空(NIL或NULL)的叶子节点！
  - 如果一个节点是红色的，则它的子节点必须是黑色的。
  - 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。

- [代码](./code/tree/rbtree.c)

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

### 3.B-树

![B树](./images/B树.png)

- B树也称B-树，它是一颗多路平衡查找树。我们描述一颗B树时需要指定它的阶数，阶数表示了一个结点最多有多少个孩子结点，一般用字母m表示阶数。当m取2时，就是我们常见的二叉搜索树。
- 一颗m阶的B树定义
  - 每个结点最多有m-1个关键字。
  - 根结点最少可以只有1个关键字。
  - 非根结点至少有Math.ceil(m/2)-1个关键字。
  - 每个结点中的关键字都按照从小到大的顺序排列，每个关键字的左子树中的所有关键字都小于它，而右子树中的所有关键字都大于它。
  - 所有叶子结点都位于同一层，或者说根结点到每个叶子结点的长度都相同。

- [代码](./code/tree/b-tree.c)

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

  