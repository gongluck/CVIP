# Redis对象

- [Redis对象](#redis对象)
  - [Redis对象基础结构](#redis对象基础结构)
  - [字符串对象](#字符串对象)
  - [列表对象](#列表对象)
  - [集合对象](#集合对象)
  - [哈希对象](#哈希对象)
  - [有序集合对象](#有序集合对象)

## Redis对象基础结构

  [redis.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/redis.h#L413)

  <details>
  <summary>Redis对象</summary>

  ```C++
  // redis对象
  typedef struct redisObject
  {
    unsigned type : 4;             //类型
    unsigned encoding : 4;         //编码
    unsigned lru : REDIS_LRU_BITS; /* lru time (relative to server.lruclock) */
    int refcount;                  //引用计数
    void *ptr;                     //实体对象
  } robj;
  ```
  </details>

  <details>
  <summary>Redis对象类型</summary>

  ```C++
  /* Object types */
  #define REDIS_STRING 0 //字符串对象
  #define REDIS_LIST 1   //列表对象
  #define REDIS_SET 2    //集合对象
  #define REDIS_ZSET 3   //有序集合对象
  #define REDIS_HASH 4   //哈希对象
  ```
  </details>

  <details>
  <summary>Redis对象编码类型</summary>

  ```C++
  /* Objects encoding. Some kind of objects like Strings and Hashes can be
  * internally represented in multiple ways. The 'encoding' field of the object
  * is set to one of this fields for this object. */
  #define REDIS_ENCODING_RAW 0 /* Raw representation */                    //简单动态字符串
  #define REDIS_ENCODING_INT 1 /* Encoded as integer */                    //保存整数型的字符串
  #define REDIS_ENCODING_HT 2 /* Encoded as hash table */                  //字典实现的对象
  #define REDIS_ENCODING_ZIPMAP 3                                          /* Encoded as zipmap */
  #define REDIS_ENCODING_LINKEDLIST 4 /* Encoded as regular linked list */ //链表实现对象
  #define REDIS_ENCODING_ZIPLIST 5 /* Encoded as ziplist */                //压缩列表实现的对象
  #define REDIS_ENCODING_INTSET 6 /* Encoded as intset */                  //整数集合实现的对象
  #define REDIS_ENCODING_SKIPLIST 7 /* Encoded as skiplist */              //跳表实现的对象
  #define REDIS_ENCODING_EMBSTR 8 /* Embedded sds string encoding */       // embstr编码的简单动态字符串
  ```
  </details>

## 字符串对象

  ![字符串对象](https://github.com/gongluck/images/blob/main/redis/字符串对象.png)

  使用`REDIS_ENCODING_RAW`、`REDIS_ENCODING_INT`或`REDIS_ENCODING_EMBSTR`编码类型

  [object.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/object.c#L39)

  <details>
  <summary>字符串对象</summary>

  ```C++
  //创建redis对象
  robj *createObject(int type, void *ptr)
  {
    robj *o = zmalloc(sizeof(*o));
    o->type = type;
    o->encoding = REDIS_ENCODING_RAW; //默认简单动态字符串
    o->ptr = ptr;                     //数据
    o->refcount = 1;                  //引用计数

    /* Set the LRU to the current lruclock (minutes resolution). */
    o->lru = LRU_CLOCK();
    return o;
  }

  //创建底层为简单动态字符串的对象
  /* Create a string object with encoding REDIS_ENCODING_RAW, that is a plain
  * string object where o->ptr points to a proper sds string. */
  robj *createRawStringObject(char *ptr, size_t len)
  {
    return createObject(REDIS_STRING, sdsnewlen(ptr, len));
  }

  //创建embstr编码的简单动态字符串编码的对象
  /* Create a string object with encoding REDIS_ENCODING_EMBSTR, that is
  * an object where the sds string is actually an unmodifiable string
  * allocated in the same chunk as the object itself. */
  robj *createEmbeddedStringObject(char *ptr, size_t len)
  {
    // redisobject部分和sds部分内存连续
    robj *o = zmalloc(sizeof(robj) + sizeof(struct sdshdr) + len + 1);
    struct sdshdr *sh = (void *)(o + 1);

    o->type = REDIS_STRING;
    o->encoding = REDIS_ENCODING_EMBSTR;
    o->ptr = sh + 1;
    o->refcount = 1;
    o->lru = LRU_CLOCK();

    sh->len = len;
    sh->free = 0;
    if (ptr)
    {
      memcpy(sh->buf, ptr, len);
      sh->buf[len] = '\0';
    }
    else
    {
      memset(sh->buf, 0, len + 1);
    }
    return o;
  }

  //创建字符串对象
  /* Create a string object with EMBSTR encoding if it is smaller than
  * REIDS_ENCODING_EMBSTR_SIZE_LIMIT, otherwise the RAW encoding is
  * used.
  *
  * The current limit of 39 is chosen so that the biggest string object
  * we allocate as EMBSTR will still fit into the 64 byte arena of jemalloc. */
  #define REDIS_ENCODING_EMBSTR_SIZE_LIMIT 39
  robj *createStringObject(char *ptr, size_t len)
  {
    if (len <= REDIS_ENCODING_EMBSTR_SIZE_LIMIT)
      return createEmbeddedStringObject(ptr, len);
    else
      return createRawStringObject(ptr, len);
  }

  //通过long long整型创建字符串对象
  robj *createStringObjectFromLongLong(long long value)
  {
    robj *o;
    if (value >= 0 && value < REDIS_SHARED_INTEGERS /*0~10000为初始创建的共享的整型字符串对象*/)
    {
      incrRefCount(shared.integers[value]);
      o = shared.integers[value];
    }
    else
    {
      if (value >= LONG_MIN && value <= LONG_MAX) //未溢出8字节
      {
        o = createObject(REDIS_STRING, NULL);
        o->encoding = REDIS_ENCODING_INT;
        o->ptr = (void *)((long)value);
      }
      else
      {
        o = createObject(REDIS_STRING, sdsfromlonglong(value));
      }
    }
    return o;
  }

  //创建字符串对象
  /* Create a string object from a long double. If humanfriendly is non-zero
  * it does not use exponential format and trims trailing zeroes at the end,
  * however this results in loss of precision. Otherwise exp format is used
  * and the output of snprintf() is not modified.
  *
  * The 'humanfriendly' option is used for INCRBYFLOAT and HINCRBYFLOAT. */
  robj *createStringObjectFromLongDouble(long double value, int humanfriendly)
  {
    char buf[256];
    int len;

    if (isinf(value)) //溢出
    {
      /* Libc in odd systems (Hi Solaris!) will format infinite in a
      * different way, so better to handle it in an explicit way. */
      if (value > 0)
      {
        memcpy(buf, "inf", 3);
        len = 3;
      }
      else
      {
        memcpy(buf, "-inf", 4);
        len = 4;
      }
    }
    else if (humanfriendly)
    {
      /* We use 17 digits precision since with 128 bit floats that precision
      * after rounding is able to represent most small decimal numbers in a
      * way that is "non surprising" for the user (that is, most small
      * decimal numbers will be represented in a way that when converted
      * back into a string are exactly the same as what the user typed.) */
      len = snprintf(buf, sizeof(buf), "%.17Lf", value); //小数点后保留17位小数
      /* Now remove trailing zeroes after the '.' */
      //去除小数点后多余的0
      if (strchr(buf, '.') != NULL)
      {
        char *p = buf + len - 1;
        while (*p == '0')
        {
          p--;
          len--;
        }
        if (*p == '.')
          len--;
      }
    }
    else
    {
      len = snprintf(buf, sizeof(buf), "%.17Lg", value);
    }
    return createStringObject(buf, len);
  }

  //复制字符串对象
  /* Duplicate a string object, with the guarantee that the returned object
  * has the same encoding as the original one.
  *
  * This function also guarantees that duplicating a small integere object
  * (or a string object that contains a representation of a small integer)
  * will always result in a fresh object that is unshared (refcount == 1).
  *
  * The resulting object always has refcount set to 1. */
  robj *dupStringObject(robj *o)
  {
    robj *d;

    redisAssert(o->type == REDIS_STRING);

    switch (o->encoding)
    {
    case REDIS_ENCODING_RAW:
      return createRawStringObject(o->ptr, sdslen(o->ptr));
    case REDIS_ENCODING_EMBSTR:
      return createEmbeddedStringObject(o->ptr, sdslen(o->ptr));
    case REDIS_ENCODING_INT:
      d = createObject(REDIS_STRING, NULL);
      d->encoding = REDIS_ENCODING_INT;
      d->ptr = o->ptr;
      return d;
    default:
      redisPanic("Wrong encoding.");
      break;
    }
  }
  ```
  </details>

## 列表对象

  ![列表对象](https://github.com/gongluck/images/blob/main/redis/列表对象.png)

  使用`REDIS_ENCODING_ZIPLIST`或`REDIS_ENCODING_LINKEDLIST`编码类型

  [object.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/object.c#L220)

  <details>
  <summary>列表对象</summary>

  ```C++
  //创建链表列表对象
  robj *createListObject(void)
  {
    list *l = listCreate();
    robj *o = createObject(REDIS_LIST /*列表对象*/, l);
    listSetFreeMethod(l, decrRefCountVoid); //设置链表对象的释放函数
    o->encoding = REDIS_ENCODING_LINKEDLIST;
    return o;
  }

  //创建压缩列表对象
  robj *createZiplistObject(void)
  {
    unsigned char *zl = ziplistNew();
    robj *o = createObject(REDIS_LIST, /*列表对象*/ zl);
    o->encoding = REDIS_ENCODING_ZIPLIST;
    return o;
  }
  ```
  </details>

## 集合对象

  ![集合对象](https://github.com/gongluck/images/blob/main/redis/集合对象.png)

  使用`REDIS_ENCODING_INTSET`或`REDIS_ENCODING_HT`编码类型

  [object.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/object.c#L239)

  <details>
  <summary>集合对象</summary>

  ```C++
  //创建字典集合对象
  robj *createSetObject(void)
  {
    dict *d = dictCreate(&setDictType /*字典集合对象使用的自定义函数操作*/, NULL);
    robj *o = createObject(REDIS_SET /*集合对象*/, d);
    o->encoding = REDIS_ENCODING_HT;
    return o;
  }

  //创建整数集合对象
  robj *createIntsetObject(void)
  {
    intset *is = intsetNew();
    robj *o = createObject(REDIS_SET /*集合对象*/, is);
    o->encoding = REDIS_ENCODING_INTSET;
    return o;
  }
  ```
  </details>

## 哈希对象

  ![哈希对象](https://github.com/gongluck/images/blob/main/redis/哈希对象.png)

  使用`REDIS_ENCODING_ZIPLIST`或`REDIS_ENCODING_HT`编码类型

  [object.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/object.c#L257)

  <details>
  <summary>哈希对象</summary>

  ```C++
  //创建哈希对象
  robj *createHashObject(void)
  {
    unsigned char *zl = ziplistNew();
    robj *o = createObject(REDIS_HASH, zl);
    o->encoding = REDIS_ENCODING_ZIPLIST;
    return o;
  }
  ```
  </details>

## 有序集合对象

  ![哈希对象](https://github.com/gongluck/images/blob/main/redis/有序集合对象.png)

  使用`REDIS_ENCODING_ZIPLIST`或`REDIS_ENCODING_SKIPLIST`编码类型

  [object.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/object.c#L266)

  <details>
  <summary>有序集合对象</summary>

  ```C++
  //创建有序集合对象
  robj *createZsetObject(void)
  {
    zset *zs = zmalloc(sizeof(*zs));
    robj *o;

    zs->dict = dictCreate(&zsetDictType /*跳表有序集合对象使用的自定义函数操作*/, NULL);
    zs->zsl = zslCreate();
    o = createObject(REDIS_ZSET, zs);
    o->encoding = REDIS_ENCODING_SKIPLIST;
    return o;
  }

  //创建压缩列表有序集合对象
  robj *createZsetZiplistObject(void)
  {
    unsigned char *zl = ziplistNew();
    robj *o = createObject(REDIS_ZSET, zl);
    o->encoding = REDIS_ENCODING_ZIPLIST;
    return o;
  }
  ```
  </details>
