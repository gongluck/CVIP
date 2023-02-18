# Redis数据结构

- [Redis数据结构](#redis数据结构)
  - [简单动态字符串](#简单动态字符串)
  - [链表](#链表)
  - [字典](#字典)
  - [跳表](#跳表)
  - [整数集合](#整数集合)
  - [压缩列表](#压缩列表)

## 简单动态字符串

  ![简单动态字符串](https://github.com/gongluck/images/blob/main/redis/简单动态字符串.png)

  - 常数复杂度获取字符串长度(`len`字段)。
  - 杜绝缓冲区溢出(`len`字段)。
  - 减少修改字符串长度时所需的内存重分配次数(`free`字段)。
  - 二进制安全(`len`字段)。
  - 兼容部分`c++`字符串函数(`buf`字段)。

  [sds.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/sds.h#L40)

  <details>
  <summary>简单动态字符串</summary>

  ```C++
  //指向简单动态字符串缓冲区
  typedef char *sds;

  //简单动态字符串
  struct sdshdr
  {
      unsigned int len;  //已使用缓冲区长度
      unsigned int free; //缓冲区空闲长度
      char buf[];        //缓冲区
  };

  //获取简单动态字符串已使用长度
  static inline size_t sdslen(const sds s)
  {
      //将指向缓冲区的指针偏移到简单动态字符串头结构
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      return sh->len;
  }

  //获取简单动态字符串未使用长度
  static inline size_t sdsavail(const sds s)
  {
      //将指向缓冲区的指针偏移到简单动态字符串头结构
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      return sh->free;
  }
  ```
  </details>

## 链表

  ![链表](https://github.com/gongluck/images/blob/main/redis/链表.png)

  - `Redis`链表是双向链表，每个节点都有指向前置节点和后置节点的指针。
  - 每个链表用一个`list`结构表示，结构中有表头节点指针、表尾节点指针，以及链表长度和类型特定函数指针等信息。
  - `Redis`链表是无环链表，表头节点的前置指针和表尾节点的后置指针都指向`NULL`。
  - 通过设置不同的类型特定函数，`Redis`链表可用保存不同类型的值。

  [adlist.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/adlist.h#L34)

  <details>
  <summary>链表</summary>

  ```C++
  //链表节点
  typedef struct listNode
  {
      struct listNode *prev; //前驱
      struct listNode *next; //后继
      void *value;           //值
  } listNode;

  //链表迭代器
  typedef struct listIter
  {
      listNode *next; //指向节点
      int direction;  //方向
  } listIter;

  //链表
  typedef struct list
  {
      listNode *head;                     //头节点
      listNode *tail;                     //尾节点
      void *(*dup)(void *ptr);            //复制方法
      void (*free)(void *ptr);            //释放方法
      int (*match)(void *ptr, void *key); //比较方法
      unsigned long len;                  //长度
  } list;
  ```
  </details>

## 字典

  ![字典](https://github.com/gongluck/images/blob/main/redis/字典.png)

  - 字典中有两个哈希表，一个平时使用，一个在`rehash`时使用。
  - 哈希表使用链地址法解决键冲突。
  - 对哈希表进行扩展或者收缩操作时，进行`rehash`将现有哈希表的所有键值对重新计算到新哈希表中，整个过程是渐进式的。

  [dict.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/dict.h#L48)

  <details>
  <summary>字典</summary>

  ```C++
	//字典节点
	typedef struct dictEntry
	{
			void *key; //键
			union
			{
					void *val;
					uint64_t u64;
					int64_t s64;
					double d;
			} v;                    //值
			struct dictEntry *next; //后继
	} dictEntry;

	//字典方法集合
	typedef struct dictType
	{
			unsigned int (*hashFunction)(const void *key);                         //哈希方法
			void *(*keyDup)(void *privdata, const void *key);                      //键复制
			void *(*valDup)(void *privdata, const void *obj);                      //值复制
			int (*keyCompare)(void *privdata, const void *key1, const void *key2); //键比较
			void (*keyDestructor)(void *privdata, void *key);                      //键销毁
			void (*valDestructor)(void *privdata, void *obj);                      //值销毁
	} dictType;

	//哈希表
	/* This is our hash table structure. Every dictionary has two of this as we
	* implement incremental rehashing, for the old to the new table. */
	typedef struct dictht
	{
			dictEntry **table;      //指向节点数组(拉链法)
			unsigned long size;     //桶数，哈希表长度
			unsigned long sizemask; //掩码，用来计算哈希值
			unsigned long used;     //节点数
	} dictht;

	//字典
	typedef struct dict
	{
			dictType *type; //操作方法集合
			void *privdata; //私有数据
			dictht ht[2];   //两个哈希表
			// rehash进行中的ht[0].table数组索引
			long rehashidx; /* rehashing not in progress if rehashidx == -1 */
			//迭代器
			int iterators; /* number of iterators currently running */
	} dict;

	//字典迭代器
	/* If safe is set to 1 this is a safe iterator, that means, you can call
	* dictAdd, dictFind, and other functions against the dictionary even while
	* iterating. Otherwise it is a non safe iterator, and only dictNext()
	* should be called while iterating. */
	typedef struct dictIterator
	{
			dict *d;    //指向字典
			long index; //迭代器当前所指向的哈希表索引位置
			int table /* 正在被迭代的哈希表 */, safe /* 标识这个迭代器是否安全 */;
			dictEntry *entry /* 当前节点 */, *nextEntry /* 下一个节点 */;
			/* unsafe iterator fingerprint for misuse detection. */
			long long fingerprint;
	} dictIterator;

	typedef void(dictScanFunction)(void *privdata, const dictEntry *de);

	/* This is the initial size of every hash table */
	#define DICT_HT_INITIAL_SIZE 4

	/* ------------------------------- Macros ------------------------------------*/
	//销毁节点值
	#define dictFreeVal(d, entry)     \
			if ((d)->type->valDestructor) \
			(d)->type->valDestructor((d)->privdata, (entry)->v.val)

	//设置节点值
	#define dictSetVal(d, entry, _val_)                                 \
			do                                                              \
			{                                                               \
					if ((d)->type->valDup)                                      \
							entry->v.val = (d)->type->valDup((d)->privdata, _val_); \
					else                                                        \
							entry->v.val = (_val_);                                 \
			} while (0)

	//设置节点值为有符号数
	#define dictSetSignedIntegerVal(entry, _val_) \
			do                                        \
			{                                         \
					entry->v.s64 = _val_;                 \
			} while (0)

	//设置节点值为无符号数
	#define dictSetUnsignedIntegerVal(entry, _val_) \
			do                                          \
			{                                           \
					entry->v.u64 = _val_;                   \
			} while (0)

	//设置节点值为浮点数
	#define dictSetDoubleVal(entry, _val_) \
			do                                 \
			{                                  \
					entry->v.d = _val_;            \
			} while (0)

	//销毁节点键
	#define dictFreeKey(d, entry)     \
			if ((d)->type->keyDestructor) \
			(d)->type->keyDestructor((d)->privdata, (entry)->key)

	//设置节点键
	#define dictSetKey(d, entry, _key_)                               \
			do                                                            \
			{                                                             \
					if ((d)->type->keyDup)                                    \
							entry->key = (d)->type->keyDup((d)->privdata, _key_); \
					else                                                      \
							entry->key = (_key_);                                 \
			} while (0)

	//比较键
	#define dictCompareKeys(d, key1, key2) \
			(((d)->type->keyCompare) ? (d)->type->keyCompare((d)->privdata, key1, key2) : (key1) == (key2))
  ```
  </details>

## 跳表

  ![跳表](https://github.com/gongluck/images/blob/main/redis/跳表.png)

  - 跳表节点的层高是`1~32`之间的随机数。
  - 多个节点可以包含相同的分值，但节点的成员对象必须是唯一的。
  - 节点按照分值排序，分值相同时按照对象的大小进行排序。

  [redis.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/redis.h#L599)

  <details>
  <summary>跳表</summary>

  ```C++
  //跳表节点
  /* ZSETs use a specialized version of Skiplists */
  typedef struct zskiplistNode
  {
    robj *obj;                      //对象
    double score;                   //排序比值
    struct zskiplistNode *backward; //后退指针
    struct zskiplistLevel
    {
      struct zskiplistNode *forward; //前进指针
      unsigned int span;             //距离
    } level[];                       //层
  } zskiplistNode;

  //跳表
  typedef struct zskiplist
  {
    struct zskiplistNode *header, *tail; //头尾节点
    unsigned long length;                //节点数
    int level;                           //最大层数
  } zskiplist;
  ```
  </details>

## 整数集合

  ![整数集合](https://github.com/gongluck/images/blob/main/redis/整数集合.png)

  - 整数集合底层是数组，这个数组以有序、无重复的方式保存集合元素。
  - 在插入元素时，如果新插入元素的类型比原集合中的元素都大，会对整数集合进行升级。
  - 整数集合只会进行升级，不会进行降级。

  [intset.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/intset.h#L35)

  <details>
  <summary>整数集合</summary>

  ```C++
  //整数集合
  typedef struct intset
  {
    uint32_t encoding; //编码(小端字节序)
    uint32_t length;   //元素个数(小端字节序)
    int8_t contents[]; //元素数组
  } intset;
  ```
  </details>

## 压缩列表

  ![压缩列表](https://github.com/gongluck/images/blob/main/redis/压缩列表.png)

  - 压缩列表包含多个节点，每个节点保存一个字节数组或者整数值。
  - 添加新元素或者删除元素，可能会出现连锁更新操作。

  [ziplist.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/ziplist.c#L115)

  <details>
  <summary>压缩列表</summary>

  ```C++
  #define ZIP_END 255    //压缩链表结束标记
  #define ZIP_BIGLEN 254 // 1字节表示节点长度的最大值

  /* Different encoding/length possibilities */
  #define ZIP_STR_MASK 0xc0 // 1100 0000
  #define ZIP_INT_MASK 0x30 // 0011 0000
  // 00xx xxxx
  // 01xx xxxx
  // 10xx xxxx
  //以上都为字符数组
  #define ZIP_STR_06B (0 << 6) // 0000 0000
  #define ZIP_STR_14B (1 << 6) // 0100 0000
  #define ZIP_STR_32B (2 << 6) // 1000 0000
  // 11xx xxxx才是整数
  #define ZIP_INT_16B (0xc0 | 0 << 4) // 1100 0000
  #define ZIP_INT_32B (0xc0 | 1 << 4) // 1101 0000
  #define ZIP_INT_64B (0xc0 | 2 << 4) // 1110 0000
  #define ZIP_INT_24B (0xc0 | 3 << 4) // 1111 0000
  #define ZIP_INT_8B 0xfe             // 1111 1110
  /* 4 bit integer immediate encoding */
  #define ZIP_INT_IMM_MASK 0x0f // 0000 1111
  #define ZIP_INT_IMM_MIN 0xf1  /* 11110001 */
  #define ZIP_INT_IMM_MAX 0xfd  /* 11111101 */
  #define ZIP_INT_IMM_VAL(v) (v & ZIP_INT_IMM_MASK)

  #define INT24_MAX 0x7fffff
  #define INT24_MIN (-INT24_MAX - 1)

  //判断是否字节数组编码
  //非11xx xxxx(小于1100 0000)即为字符数组
  /* Macro to determine type */
  #define ZIP_IS_STR(enc) (((enc)&ZIP_STR_MASK) < ZIP_STR_MASK)

  /* Utility macros */
  #define ZIPLIST_BYTES(zl) (*((uint32_t *)(zl)))                               //整个压缩列表的字节数 4字节
  #define ZIPLIST_TAIL_OFFSET(zl) (*((uint32_t *)((zl) + sizeof(uint32_t))))    //结束标记地址偏移
  #define ZIPLIST_LENGTH(zl) (*((uint16_t *)((zl) + sizeof(uint32_t) * 2)))     //压缩列表长度 2字节 可能会溢出
  #define ZIPLIST_HEADER_SIZE (sizeof(uint32_t) * 2 + sizeof(uint16_t))         //压缩链表头所占长度 等于以上3个字段所占总和
  #define ZIPLIST_ENTRY_HEAD(zl) ((zl) + ZIPLIST_HEADER_SIZE)                   //头节点
  #define ZIPLIST_ENTRY_TAIL(zl) ((zl) + intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl))) //结束标记
  #define ZIPLIST_ENTRY_END(zl) ((zl) + intrev32ifbe(ZIPLIST_BYTES(zl)) - 1)    //尾节点

  //增加压缩列表长度
  /* We know a positive increment can only be 1 because entries can only be
  * pushed one at a time. */
  #define ZIPLIST_INCR_LENGTH(zl, incr)                                             \
    {                                                                               \
      if (ZIPLIST_LENGTH(zl) < UINT16_MAX) /*长度未溢出的*/                   \
        ZIPLIST_LENGTH(zl) = intrev16ifbe(intrev16ifbe(ZIPLIST_LENGTH(zl)) + incr); \
    }

  //压缩列表节点
  //实际内存布局 prevlength + encoding + content
  typedef struct zlentry
  {
    unsigned int prevrawlensize /*prevrawlen字节大小*/, prevrawlen /*前置节点长度*/;
    unsigned int lensize /*len字节大小*/, len /*节点长度*/;
    unsigned int headersize; //节点头大小 等于 prevrawlensize + lensize
    unsigned char encoding;  //节点编码类型
    unsigned char *p;        //指向节点数据
  } zlentry;
  ```
  </details>