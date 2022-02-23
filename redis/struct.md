# ```Redis```数据结构

- [```Redis```数据结构](#redis数据结构)
  - [```Redis```基础数据结构](#redis基础数据结构)
    - [简单动态字符串](#简单动态字符串)
    - [链表](#链表)
    - [字典](#字典)
    - [跳表](#跳表)
    - [整数集合](#整数集合)

## ```Redis```基础数据结构

### 简单动态字符串

  ![简单动态字符串](https://github.com/gongluck/images/blob/main/redis/简单动态字符串.png)

  - 常数复杂度获取字符串长度(```len```字段)
  - 杜绝缓冲区溢出(```len```字段)
  - 减少修改字符串长度时所需的内存重分配次数(```free```字段)
  - 二进制安全(```len```字段)
  - 兼容部分```c++```字符串函数(```buf```字段)

  [sds.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/sds.h)

  <details>
  <summary>简单动态字符串</summary>

  ```c++
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

  [sds.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/sds.c)

  <details>
  <summary>简单动态字符串</summary>

  ```c++
  //创建新的简单动态字符串
  /* Create a new sds string with the content specified by the 'init' pointer
  * and 'initlen'.
  * If NULL is used for 'init' the string is initialized with zero bytes.
  *
  * The string is always null-termined (all the sds strings are, always) so
  * even if you create an sds string with:
  *
  * mystring = sdsnewlen("abc",3);
  *
  * You can print the string with printf() as there is an implicit \0 at the
  * end of the string. However the string is binary safe and can contain
  * \0 characters in the middle, as the length is stored in the sds header. */
  sds sdsnewlen(const void *init, size_t initlen)
  {
      struct sdshdr *sh;

      if (init)
      {
          //如果有初始值，使用zmalloc
          sh = zmalloc(sizeof(struct sdshdr) + initlen + 1); //+1是'\0'
      }
      else
      {
          //如果没有初始值，使用calloc，calloc会将内存初始化为0
          sh = zcalloc(sizeof(struct sdshdr) + initlen + 1); //+1是'\0'
      }
      if (sh == NULL)
          return NULL;
      sh->len = initlen;
      sh->free = 0;
      if (initlen && init)
          memcpy(sh->buf, init, initlen); //拷贝初始数据
      sh->buf[initlen] = '\0';            //填补结束标记
      return (char *)sh->buf;             //返回简单动态字符串缓冲区
  }

  //创建空字符串
  /* Create an empty (zero length) sds string. Even in this case the string
  * always has an implicit null term. */
  sds sdsempty(void)
  {
      return sdsnewlen("", 0);
  }

  //创建字符串
  /* Create a new sds string starting from a null termined C string. */
  sds sdsnew(const char *init)
  {
      size_t initlen = (init == NULL) ? 0 : strlen(init);
      return sdsnewlen(init, initlen);
  }

  //复制字符串
  /* Duplicate an sds string. */
  sds sdsdup(const sds s)
  {
      return sdsnewlen(s, sdslen(s));
  }

  //释放字符串
  /* Free an sds string. No operation is performed if 's' is NULL. */
  void sdsfree(sds s)
  {
      if (s == NULL)
          return;
      zfree(s - sizeof(struct sdshdr));
  }

  //更新字符串长度
  /* Set the sds string length to the length as obtained with strlen(), so
  * considering as content only up to the first null term character.
  *
  * This function is useful when the sds string is hacked manually in some
  * way, like in the following example:
  *
  * s = sdsnew("foobar");
  * s[2] = '\0';
  * sdsupdatelen(s);
  * printf("%d\n", sdslen(s));
  *
  * The output will be "2", but if we comment out the call to sdsupdatelen()
  * the output will be "6" as the string was modified but the logical length
  * remains 6 bytes. */
  void sdsupdatelen(sds s)
  {
      //获取字符串头结构
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      //重新计算字符串长度
      int reallen = strlen(s);
      //更新字符串长度
      sh->free += (sh->len - reallen);
      sh->len = reallen;
  }

  //清空字符串缓冲区
  /* Modify an sds string in-place to make it empty (zero length).
  * However all the existing buffer is not discarded but set as free space
  * so that next append operations will not require allocations up to the
  * number of bytes previously available. */
  void sdsclear(sds s)
  {
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      sh->free += sh->len;
      sh->len = 0;
      sh->buf[0] = '\0';
  }

  //扩充备用缓冲区
  /* Enlarge the free space at the end of the sds string so that the caller
  * is sure that after calling this function can overwrite up to addlen
  * bytes after the end of the string, plus one more byte for nul term.
  *
  * Note: this does not change the *length* of the sds string as returned
  * by sdslen(), but only the free buffer space we have. */
  sds sdsMakeRoomFor(sds s, size_t addlen)
  {
      struct sdshdr *sh, *newsh;
      //获取可用大小
      size_t free = sdsavail(s);
      size_t len, newlen;

      if (free >= addlen)
          return s;
      //获取已用长度
      len = sdslen(s);
      //偏移获取字符串头结构
      sh = (void *)(s - (sizeof(struct sdshdr)));
      //计算新总长度
      newlen = (len + addlen);
      if (newlen < SDS_MAX_PREALLOC) //新长度小于最大预分配长度
          newlen *= 2;               //新长度翻倍
      else
          newlen += SDS_MAX_PREALLOC; //新长度增加一个最大预分配长度
      //重新分配缓冲区
      newsh = zrealloc(sh, sizeof(struct sdshdr) + newlen + 1);
      if (newsh == NULL)
          return NULL;
      //更新可用缓冲区长度
      newsh->free = newlen - len;
      return newsh->buf;
  }

  //删除候备缓冲区
  /* Reallocate the sds string so that it has no free space at the end. The
  * contained string remains not altered, but next concatenation operations
  * will require a reallocation.
  *
  * After the call, the passed sds string is no longer valid and all the
  * references must be substituted with the new pointer returned by the call. */
  sds sdsRemoveFreeSpace(sds s)
  {
      struct sdshdr *sh;
      //获取字符串头结构
      sh = (void *)(s - (sizeof(struct sdshdr)));
      //重新分配缓冲区
      sh = zrealloc(sh, sizeof(struct sdshdr) + sh->len + 1);
      sh->free = 0;
      return sh->buf;
  }

  //获取简单动态字符串的总数据长度
  /* Return the total size of the allocation of the specifed sds string,
  * including:
  * 1) The sds header before the pointer.
  * 2) The string.
  * 3) The free buffer at the end if any.
  * 4) The implicit null term.
  */
  size_t sdsAllocSize(sds s)
  {
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));

      return sizeof(*sh) + sh->len + sh->free + 1;
  }

  //增加/减少已用缓冲区的边界
  /* Increment the sds length and decrements the left free space at the
  * end of the string according to 'incr'. Also set the null term
  * in the new end of the string.
  *
  * This function is used in order to fix the string length after the
  * user calls sdsMakeRoomFor(), writes something after the end of
  * the current string, and finally needs to set the new length.
  *
  * Note: it is possible to use a negative increment in order to
  * right-trim the string.
  *
  * Usage example:
  *
  * Using sdsIncrLen() and sdsMakeRoomFor() it is possible to mount the
  * following schema, to cat bytes coming from the kernel to the end of an
  * sds string without copying into an intermediate buffer:
  *
  * oldlen = sdslen(s);
  * s = sdsMakeRoomFor(s, BUFFER_SIZE);
  * nread = read(fd, s+oldlen, BUFFER_SIZE);
  * ... check for nread <= 0 and handle it ...
  * sdsIncrLen(s, nread);
  */
  void sdsIncrLen(sds s, int incr)
  {
      //获取字符串头结构
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));

      if (incr >= 0) //增加已用
          assert(sh->free >= (unsigned int)incr);
      else //增加未用
          assert(sh->len >= (unsigned int)(-incr));
      sh->len += incr;
      sh->free -= incr;
      s[sh->len] = '\0';
  }

  //增长已用长度，并填充默认值
  /* Grow the sds to have the specified length. Bytes that were not part of
  * the original length of the sds will be set to zero.
  *
  * if the specified length is smaller than the current length, no operation
  * is performed. */
  sds sdsgrowzero(sds s, size_t len)
  {
      //获取字符串头结构
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      size_t totlen, curlen = sh->len;

      //原长度足够，直接返回
      if (len <= curlen)
          return s;
      //扩充长度
      s = sdsMakeRoomFor(s, len - curlen);
      if (s == NULL)
          return NULL;

      //获取字符串头结构
      /* Make sure added region doesn't contain garbage */
      sh = (void *)(s - (sizeof(struct sdshdr)));
      //填充增长区段
      memset(s + curlen, 0, (len - curlen + 1)); /* also set trailing \0 byte */
      totlen = sh->len + sh->free;
      sh->len = len;
      sh->free = totlen - sh->len;
      return s;
  }

  //尾部追加数据
  /* Append the specified binary-safe string pointed by 't' of 'len' bytes to the
  * end of the specified sds string 's'.
  *
  * After the call, the passed sds string is no longer valid and all the
  * references must be substituted with the new pointer returned by the call. */
  sds sdscatlen(sds s, const void *t, size_t len)
  {
      struct sdshdr *sh;
      //获取当前已用长度
      size_t curlen = sdslen(s);

      //确保有足够空闲缓冲区
      s = sdsMakeRoomFor(s, len);
      if (s == NULL)
          return NULL;
      //获取字符串头结构
      sh = (void *)(s - (sizeof(struct sdshdr)));
      //尾部追加数据
      memcpy(s + curlen, t, len);
      sh->len = curlen + len;
      sh->free = sh->free - len;
      s[curlen + len] = '\0';
      return s;
  }

  //尾部追加字符串
  /* Append the specified null termianted C string to the sds string 's'.
  *
  * After the call, the passed sds string is no longer valid and all the
  * references must be substituted with the new pointer returned by the call. */
  sds sdscat(sds s, const char *t)
  {
      return sdscatlen(s, t, strlen(t));
  }

  //尾部追加简单动态字符串
  /* Append the specified sds 't' to the existing sds 's'.
  *
  * After the call, the modified sds string is no longer valid and all the
  * references must be substituted with the new pointer returned by the call. */
  sds sdscatsds(sds s, const sds t)
  {
      return sdscatlen(s, t, sdslen(t));
  }

  //拷贝字符串
  /* Destructively modify the sds string 's' to hold the specified binary
  * safe string pointed by 't' of length 'len' bytes. */
  sds sdscpylen(sds s, const char *t, size_t len)
  {
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      size_t totlen = sh->free + sh->len;

      //确保缓冲区足够大
      if (totlen < len)
      {
          s = sdsMakeRoomFor(s, len - sh->len);
          if (s == NULL)
              return NULL;
          sh = (void *)(s - (sizeof(struct sdshdr)));
          totlen = sh->free + sh->len;
      }
      //拷贝数据
      memcpy(s, t, len);
      s[len] = '\0';
      sh->len = len;
      sh->free = totlen - len;
      return s;
  }

  //拷贝字符串
  /* Like sdscpylen() but 't' must be a null-termined string so that the length
  * of the string is obtained with strlen(). */
  sds sdscpy(sds s, const char *t)
  {
      return sdscpylen(s, t, strlen(t));
  }

  // long long to string
  /* Helper for sdscatlonglong() doing the actual number -> string
  * conversion. 's' must point to a string with room for at least
  * SDS_LLSTR_SIZE bytes.
  *
  * The function returns the length of the null-terminated string
  * representation stored at 's'. */
  #define SDS_LLSTR_SIZE 21
  int sdsll2str(char *s, long long value)
  {
      char *p, aux;
      unsigned long long v;
      size_t l;

      //取绝对值
      /* Generate the string representation, this method produces
      * an reversed string. */
      v = (value < 0) ? -value : value;
      p = s;
      //循环处理每个十进制位
      do
      {
          //取模
          *p++ = '0' + (v % 10);
          //右移一个十进制位
          v /= 10;
      } while (v);
      //处理负数
      if (value < 0)
          *p++ = '-';

      /* Compute length and add null term. */
      l = p - s;
      *p = '\0';

      //反转
      /* Reverse the string. */
      p--;
      while (s < p)
      {
          aux = *s;
          *s = *p;
          *p = aux;
          s++;
          p--;
      }
      return l;
  }

  // unsigned long long to str
  /* Identical sdsll2str(), but for unsigned long long type. */
  int sdsull2str(char *s, unsigned long long v)
  {
      char *p, aux;
      size_t l;

      /* Generate the string representation, this method produces
      * an reversed string. */
      p = s;
      //循环处理每个十进制位
      do
      {
          //取模
          *p++ = '0' + (v % 10);
          //右移一个十进制位
          v /= 10;
      } while (v);

      /* Compute length and add null term. */
      l = p - s;
      *p = '\0';

      //反转
      /* Reverse the string. */
      p--;
      while (s < p)
      {
          aux = *s;
          *s = *p;
          *p = aux;
          s++;
          p--;
      }
      return l;
  }

  // long long to sds
  /* Create an sds string from a long long value. It is much faster than:
  *
  * sdscatprintf(sdsempty(),"%lld\n", value);
  */
  sds sdsfromlonglong(long long value)
  {
      char buf[SDS_LLSTR_SIZE];
      int len = sdsll2str(buf, value);

      return sdsnewlen(buf, len);
  }

  //将字符串格式化后拼接到sds后
  /* Like sdscatprintf() but gets va_list instead of being variadic. */
  sds sdscatvprintf(sds s, const char *fmt, va_list ap)
  {
      va_list cpy;
      char staticbuf[1024], *buf = staticbuf, *t;
      //接收输出的缓冲区
      size_t buflen = strlen(fmt) * 2;

      /* We try to start using a static buffer for speed.
      * If not possible we revert to heap allocation. */
      if (buflen > sizeof(staticbuf))
      {
          buf = zmalloc(buflen);
          if (buf == NULL)
              return NULL;
      }
      else
      {
          // buflen太小则使用静态缓冲区
          buflen = sizeof(staticbuf);
      }

      /* Try with buffers two times bigger every time we fail to
      * fit the string in the current buffer size. */
      while (1)
      {
          // buf末尾置为'\0'，后面用来判断是否溢出
          buf[buflen - 2] = '\0';
          //拷贝可变参数
          va_copy(cpy, ap);
          //格式化输出
          vsnprintf(buf, buflen, fmt, cpy);
          //清理可变参数
          va_end(cpy);
          //判断溢出
          if (buf[buflen - 2] != '\0')
          {
              if (buf != staticbuf)
                  zfree(buf);
              //翻倍机会
              //「翻倍机会」是《游戏王》中的一张速攻魔法卡，在怪兽攻击被无效时让那只怪兽攻击力翻倍再作出攻击的效果。
              // 经典组合是：「No.39 希望皇霍普」无效自身攻击再发动「翻倍机会」时 "霍普" 获得5000的攻击力再次攻击。
              buflen *= 2;
              buf = zmalloc(buflen);
              if (buf == NULL)
                  return NULL;
              continue;
          }
          break;
      }

      //拼接格式化结果
      /* Finally concat the obtained string to the SDS string and return it. */
      t = sdscat(s, buf);
      if (buf != staticbuf)
          zfree(buf);
      return t;
  }

  //格式化字符串后拼接到sds后
  /* Append to the sds string 's' a string obtained using printf-alike format
  * specifier.
  *
  * After the call, the modified sds string is no longer valid and all the
  * references must be substituted with the new pointer returned by the call.
  *
  * Example:
  *
  * s = sdsnew("Sum is: ");
  * s = sdscatprintf(s,"%d+%d = %d",a,b,a+b).
  *
  * Often you need to create a string from scratch with the printf-alike
  * format. When this is the need, just use sdsempty() as the target string:
  *
  * s = sdscatprintf(sdsempty(), "... your format ...", args);
  */
  sds sdscatprintf(sds s, const char *fmt, ...)
  {
      va_list ap;
      char *t;
      //开始使用可变参数
      va_start(ap, fmt);
      t = sdscatvprintf(s, fmt, ap);
      //结束使用可变参数
      va_end(ap);
      return t;
  }

  //格式化字符串并拼接到sds后，不依赖libc
  /* This function is similar to sdscatprintf, but much faster as it does
  * not rely on sprintf() family functions implemented by the libc that
  * are often very slow. Moreover directly handling the sds string as
  * new data is concatenated provides a performance improvement.
  *
  * However this function only handles an incompatible subset of printf-alike
  * format specifiers:
  *
  * %s - C String
  * %S - SDS string
  * %i - signed int
  * %I - 64 bit signed integer (long long, int64_t)
  * %u - unsigned int
  * %U - 64 bit unsigned integer (unsigned long long, uint64_t)
  * %% - Verbatim "%" character.
  */
  sds sdscatfmt(sds s, char const *fmt, ...)
  {
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      size_t initlen = sdslen(s);
      const char *f = fmt;
      int i;
      va_list ap;

      va_start(ap, fmt);
      //当前操作字符地址
      f = fmt; /* Next format specifier byte to process. */
      //输出缓冲区索引
      i = initlen; /* Position of the next byte to write to dest str. */
      while (*f)
      {
          char next, *str;
          unsigned int l;
          long long num;
          unsigned long long unum;

          //确保空间
          /* Make sure there is always space for at least 1 char. */
          if (sh->free == 0)
          {
              s = sdsMakeRoomFor(s, 1);
              sh = (void *)(s - (sizeof(struct sdshdr)));
          }

          switch (*f)
          {
          //格式化标记
          case '%':
              next = *(f + 1);
              f++;
              //判断%后面的字符
              switch (next)
              {
              case 's':
              case 'S':
                  //解析可变参数
                  str = va_arg(ap, char *);
                  l = (next == 's') ? strlen(str) : sdslen(str);
                  if (sh->free < l)
                  {
                      s = sdsMakeRoomFor(s, l);
                      sh = (void *)(s - (sizeof(struct sdshdr)));
                  }
                  //拷贝数据
                  memcpy(s + i, str, l);
                  sh->len += l;
                  sh->free -= l;
                  i += l;
                  break;
              case 'i':
              case 'I':
                  if (next == 'i')
                      num = va_arg(ap, int);
                  else
                      num = va_arg(ap, long long);
                  {
                      char buf[SDS_LLSTR_SIZE];
                      // long long to sds
                      l = sdsll2str(buf, num);
                      if (sh->free < l)
                      {
                          s = sdsMakeRoomFor(s, l);
                          sh = (void *)(s - (sizeof(struct sdshdr)));
                      }
                      //拷贝数据
                      memcpy(s + i, buf, l);
                      sh->len += l;
                      sh->free -= l;
                      i += l;
                  }
                  break;
              case 'u':
              case 'U':
                  if (next == 'u')
                      unum = va_arg(ap, unsigned int);
                  else
                      unum = va_arg(ap, unsigned long long);
                  {
                      char buf[SDS_LLSTR_SIZE];
                      // unsigned long long to sds
                      l = sdsull2str(buf, unum);
                      if (sh->free < l)
                      {
                          s = sdsMakeRoomFor(s, l);
                          sh = (void *)(s - (sizeof(struct sdshdr)));
                      }
                      //拷贝数据
                      memcpy(s + i, buf, l);
                      sh->len += l;
                      sh->free -= l;
                      i += l;
                  }
                  break;
              default: /* Handle %% and generally %<unknown>. */
                  //跳过不识别选项
                  s[i++] = next;
                  sh->len += 1;
                  sh->free -= 1;
                  break;
              }
              break;
          default:
              //跳过非格式化字符
              s[i++] = *f;
              sh->len += 1;
              sh->free -= 1;
              break;
          }
          f++;
      }
      va_end(ap);

      /* Add null-term */
      s[i] = '\0';
      return s;
  }

  //去除缓冲区前后的匹配字符
  /* Remove the part of the string from left and from right composed just of
  * contiguous characters found in 'cset', that is a null terminted C string.
  *
  * After the call, the modified sds string is no longer valid and all the
  * references must be substituted with the new pointer returned by the call.
  *
  * Example:
  *
  * s = sdsnew("AA...AA.a.aa.aHelloWorld     :::");
  * s = sdstrim(s,"A. :");
  * printf("%s\n", s);
  *
  * Output will be just "Hello World".
  */
  sds sdstrim(sds s, const char *cset)
  {
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      char *start, *end, *sp, *ep;
      size_t len;

      //缓冲区开始
      sp = start = s;
      //缓冲区结束
      ep = end = s + sdslen(s) - 1;
      //从开始处一直右移，直到到达结束或者找到第一个不匹配字符
      while (sp <= end && strchr(cset, *sp))
          sp++;
      //从结束处一直左移，直到到达开始或者找到第一个不匹配字符
      while (ep > start && strchr(cset, *ep))
          ep--;
      //切割后的长度
      len = (sp > ep) ? 0 : ((ep - sp) + 1);
      if (sh->buf != sp) //开始处需要切割
          memmove(sh->buf, sp, len);
      sh->buf[len] = '\0';
      sh->free = sh->free + (sh->len - len);
      sh->len = len;
      return s;
  }

  //切割字符串
  /* Turn the string into a smaller (or equal) string containing only the
  * substring specified by the 'start' and 'end' indexes.
  *
  * start and end can be negative, where -1 means the last character of the
  * string, -2 the penultimate character, and so forth.
  *
  * The interval is inclusive, so the start and end characters will be part
  * of the resulting string.
  *
  * The string is modified in-place.
  *
  * Example:
  *
  * s = sdsnew("Hello World");
  * sdsrange(s,1,-1); => "ello World"
  */
  void sdsrange(sds s, int start, int end)
  {
      struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
      size_t newlen, len = sdslen(s);

      if (len == 0)
          return;
      if (start < 0)
      {
          start = len + start;
          if (start < 0)
              start = 0;
      }
      if (end < 0)
      {
          end = len + end;
          if (end < 0)
              end = 0;
      }
      //计算新长度
      newlen = (start > end) ? 0 : (end - start) + 1;
      if (newlen != 0)
      {
          if (start >= (signed)len)
          {
              newlen = 0;
          }
          else if (end >= (signed)len)
          {
              end = len - 1;
              newlen = (start > end) ? 0 : (end - start) + 1;
          }
      }
      else
      {
          start = 0;
      }
      if (start && newlen)
          memmove(sh->buf, sh->buf + start, newlen); //拷贝数据
      sh->buf[newlen] = 0;
      sh->free = sh->free + (sh->len - newlen);
      sh->len = newlen;
  }

  // tolower
  /* Apply tolower() to every character of the sds string 's'. */
  void sdstolower(sds s)
  {
      int len = sdslen(s), j;

      for (j = 0; j < len; j++)
          s[j] = tolower(s[j]);
  }

  // toupper
  /* Apply toupper() to every character of the sds string 's'. */
  void sdstoupper(sds s)
  {
      int len = sdslen(s), j;

      for (j = 0; j < len; j++)
          s[j] = toupper(s[j]);
  }

  //比较
  /* Compare two sds strings s1 and s2 with memcmp().
  *
  * Return value:
  *
  *     positive if s1 > s2.
  *     negative if s1 < s2.
  *     0 if s1 and s2 are exactly the same binary string.
  *
  * If two strings share exactly the same prefix, but one of the two has
  * additional characters, the longer string is considered to be greater than
  * the smaller one. */
  int sdscmp(const sds s1, const sds s2)
  {
      size_t l1, l2, minlen;
      int cmp;

      l1 = sdslen(s1);
      l2 = sdslen(s2);
      minlen = (l1 < l2) ? l1 : l2;
      //比较
      cmp = memcmp(s1, s2, minlen);
      if (cmp == 0)
          return l1 - l2;
      return cmp;
  }

  /* Split 's' with separator in 'sep'. An array
  * of sds strings is returned. *count will be set
  * by reference to the number of tokens returned.
  *
  * On out of memory, zero length string, zero length
  * separator, NULL is returned.
  *
  * Note that 'sep' is able to split a string using
  * a multi-character separator. For example
  * sdssplit("foo_-_bar","_-_"); will return two
  * elements "foo" and "bar".
  *
  * This version of the function is binary-safe but
  * requires length arguments. sdssplit() is just the
  * same function but for zero-terminated strings.
  */
  sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count)
  {
      int elements = 0, slots = 5, start = 0, j;
      sds *tokens;

      if (seplen < 1 || len < 0)
          return NULL;

      tokens = zmalloc(sizeof(sds) * slots);
      if (tokens == NULL)
          return NULL;

      if (len == 0)
      {
          *count = 0;
          return tokens;
      }
      for (j = 0; j < (len - (seplen - 1)); j++)
      {
          //槽数量判断
          // https://github.com/antirez/redis/pull/6464 提出把槽数量判断应该放到下面查找匹配下面，因为有可能出现这里将槽数量翻倍后再查找匹配却没有新匹配的情况，导致多分配了一倍不需要的内存！
          /* make sure there is room for the next element and the final one */
          if (slots < elements + 2)
          {
              sds *newtokens;

              //翻倍机会！
              slots *= 2;
              newtokens = zrealloc(tokens, sizeof(sds) * slots);
              if (newtokens == NULL)
                  goto cleanup;
              tokens = newtokens;
          }
          //查找匹配
          /* search the separator */
          if ((seplen == 1 && *(s + j) == sep[0]) || (memcmp(s + j, sep, seplen) == 0))
          {
              tokens[elements] = sdsnewlen(s + start, j - start);
              if (tokens[elements] == NULL)
                  goto cleanup;
              //更新索引
              elements++;
              start = j + seplen;
              j = j + seplen - 1; /* skip the separator */
          }
      }
      //收尾工作
      /* Add the final element. We are sure there is room in the tokens array. */
      tokens[elements] = sdsnewlen(s + start, len - start);
      if (tokens[elements] == NULL)
          goto cleanup;
      elements++;
      *count = elements;
      return tokens;

  cleanup:
  {
      //这里冗余了，可以用sdsfreesplitres代替
      int i;
      for (i = 0; i < elements; i++)
          sdsfree(tokens[i]);
      zfree(tokens);
      *count = 0;
      return NULL;
  }
  }

  //释放sdssplitlen分配的内存
  /* Free the result returned by sdssplitlen(), or do nothing if 'tokens' is NULL. */
  void sdsfreesplitres(sds *tokens, int count)
  {
      if (!tokens)
          return;
      while (count--)
          sdsfree(tokens[count]);
      zfree(tokens);
  }

  //拼接转义后的字符串
  /* Append to the sds string "s" an escaped string representation where
  * all the non-printable characters (tested with isprint()) are turned into
  * escapes in the form "\n\r\a...." or "\x<hex-number>".
  *
  * After the call, the modified sds string is no longer valid and all the
  * references must be substituted with the new pointer returned by the call. */
  sds sdscatrepr(sds s, const char *p, size_t len)
  {
      //+ "
      s = sdscatlen(s, "\"", 1);
      while (len--)
      {
          switch (*p)
          {
          case '\\': /* + \\ */
          case '"':  /* + \" */
              s = sdscatprintf(s, "\\%c", *p);
              break;
          case '\n':
              s = sdscatlen(s, "\\n", 2);
              break;
          case '\r':
              s = sdscatlen(s, "\\r", 2);
              break;
          case '\t':
              s = sdscatlen(s, "\\t", 2);
              break;
          case '\a':
              s = sdscatlen(s, "\\a", 2);
              break;
          case '\b':
              s = sdscatlen(s, "\\b", 2);
              break;
          default:
              if (isprint(*p))
                  s = sdscatprintf(s, "%c", *p);
              else
                  s = sdscatprintf(s, "\\x%02x", (unsigned char)*p);
              break;
          }
          p++;
      }
      //+ "
      return sdscatlen(s, "\"", 1);
  }

  //判断是否十六进制
  /* Helper function for sdssplitargs() that returns non zero if 'c'
  * is a valid hex digit. */
  int is_hex_digit(char c)
  {
      return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F');
  }

  // hex to int
  /* Helper function for sdssplitargs() that converts a hex digit into an
  * integer from 0 to 15 */
  int hex_digit_to_int(char c)
  {
      switch (c)
      {
      case '0':
          return 0;
      case '1':
          return 1;
      case '2':
          return 2;
      case '3':
          return 3;
      case '4':
          return 4;
      case '5':
          return 5;
      case '6':
          return 6;
      case '7':
          return 7;
      case '8':
          return 8;
      case '9':
          return 9;
      case 'a':
      case 'A':
          return 10;
      case 'b':
      case 'B':
          return 11;
      case 'c':
      case 'C':
          return 12;
      case 'd':
      case 'D':
          return 13;
      case 'e':
      case 'E':
          return 14;
      case 'f':
      case 'F':
          return 15;
      default:
          return 0;
      }
  }

  /* Split a line into arguments, where every argument can be in the
  * following programming-language REPL-alike form:
  *
  * foo bar "newline are supported\n" and "\xff\x00otherstuff"
  *
  * The number of arguments is stored into *argc, and an array
  * of sds is returned.
  *
  * The caller should free the resulting array of sds strings with
  * sdsfreesplitres().
  *
  * Note that sdscatrepr() is able to convert back a string into
  * a quoted string in the same format sdssplitargs() is able to parse.
  *
  * The function returns the allocated tokens on success, even when the
  * input string is empty, or NULL if the input contains unbalanced
  * quotes or closed quotes followed by non space characters
  * as in: "foo"bar or "foo'
  */
  sds *sdssplitargs(const char *line, int *argc)
  {
      const char *p = line;
      char *current = NULL;
      char **vector = NULL;

      *argc = 0;
      while (1)
      {
          //跳过空白字符
          /* skip blanks */
          while (*p && isspace(*p))
              p++;
          if (*p)
          {
              /* get a token */
              //是否在引号中
              int inq = 0; /* set to 1 if we are in "quotes" */
              //是否在单引号中
              int insq = 0; /* set to 1 if we are in 'single quotes' */
              int done = 0;

              if (current == NULL)
                  current = sdsempty();
              while (!done)
              {
                  if (inq)
                  {
                      //\xFF
                      if (*p == '\\' && *(p + 1) == 'x' &&
                          is_hex_digit(*(p + 2)) &&
                          is_hex_digit(*(p + 3)))
                      {
                          unsigned char byte;
                          //十六进制转换成十进制整数后拼接到sds后
                          byte = (hex_digit_to_int(*(p + 2)) * 16) +
                                hex_digit_to_int(*(p + 3));
                          current = sdscatlen(current, (char *)&byte, 1);
                          p += 3;
                      }
                      else if (*p == '\\' && *(p + 1)) //处理转义
                      {
                          char c;

                          p++;
                          switch (*p)
                          {
                          case 'n':
                              c = '\n';
                              break;
                          case 'r':
                              c = '\r';
                              break;
                          case 't':
                              c = '\t';
                              break;
                          case 'b':
                              c = '\b';
                              break;
                          case 'a':
                              c = '\a';
                              break;
                          default:
                              c = *p;
                              break;
                          }
                          current = sdscatlen(current, &c, 1);
                      }
                      else if (*p == '"') //退出引号
                      {
                          /* closing quote must be followed by a space or
                          * nothing at all. */
                          if (*(p + 1) && !isspace(*(p + 1)))
                              goto err;
                          done = 1;
                      }
                      else if (!*p) //'\0'
                      {
                          /* unterminated quotes */
                          goto err;
                      }
                      else
                      {
                          current = sdscatlen(current, p, 1);
                      }
                  }
                  else if (insq)
                  {
                      /* \' */
                      if (*p == '\\' && *(p + 1) == '\'')
                      {
                          p++;
                          current = sdscatlen(current, "'", 1);
                      }
                      else if (*p == '\'') //退出单引号
                      {
                          /* closing quote must be followed by a space or
                          * nothing at all. */
                          if (*(p + 1) && !isspace(*(p + 1)))
                              goto err;
                          done = 1;
                      }
                      else if (!*p) //'\0'
                      {
                          /* unterminated quotes */
                          goto err;
                      }
                      else
                      {
                          current = sdscatlen(current, p, 1);
                      }
                  }
                  else
                  {
                      switch (*p)
                      {
                      case ' ':
                      case '\n':
                      case '\r':
                      case '\t':
                      case '\0':
                          done = 1;
                          break;
                      case '"':
                          inq = 1;
                          break;
                      case '\'':
                          insq = 1;
                          break;
                      default:
                          current = sdscatlen(current, p, 1);
                          break;
                      }
                  }
                  if (*p)
                      p++;
              }
              //添加一条结果
              /* add the token to the vector */
              vector = zrealloc(vector, ((*argc) + 1) * sizeof(char *));
              vector[*argc] = current;
              (*argc)++;
              current = NULL;
          }
          else
          {
              /* Even on empty input string return something not NULL. */
              if (vector == NULL)
                  vector = zmalloc(sizeof(void *));
              return vector;
          }
      }

  err:
      //清空
      while ((*argc)--)
          sdsfree(vector[*argc]);
      zfree(vector);
      if (current)
          sdsfree(current);
      *argc = 0;
      return NULL;
  }

  //将sds中的from[i]替换成to[i]
  /* Modify the string substituting all the occurrences of the set of
  * characters specified in the 'from' string to the corresponding character
  * in the 'to' array.
  *
  * For instance: sdsmapchars(mystring, "ho", "01", 2)
  * will have the effect of turning the string "hello" into "0ell1".
  *
  * The function returns the sds string pointer, that is always the same
  * as the input pointer since no resize is needed. */
  sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen)
  {
      size_t j, i, l = sdslen(s);

      //遍历sds
      for (j = 0; j < l; j++)
      {
          //遍历from中是否有匹配
          for (i = 0; i < setlen; i++)
          {
              if (s[j] == from[i])
              {
                  //替换
                  s[j] = to[i];
                  break;
              }
          }
      }
      return s;
  }

  //连接字符串
  /* Join an array of C strings using the specified separator (also a C string).
  * Returns the result as an sds string. */
  sds sdsjoin(char **argv, int argc, char *sep)
  {
      sds join = sdsempty();
      int j;

      //遍历参数数组
      for (j = 0; j < argc; j++)
      {
          //拼接
          join = sdscat(join, argv[j]);
          if (j != argc - 1)
              join = sdscat(join, sep); //连接符
      }
      return join;
  }
  ```
  </details>

### 链表

  ![链表](https://github.com/gongluck/images/blob/main/redis/链表.png)

  - ```Redis```链表是双向链表，每个节点都有指向前置节点和后置节点的指针
  - 每个链表用一个```list```结构表示，结构中有表头节点指针、表尾节点指针，以及链表长度和类型特定函数指针等信息
  - ```Redis```链表是无环链表，表头节点的前置指针和表尾节点的后置指针都指向```NULL```
  - 通过设置不同的类型特定函数，```Redis```链表可用保存不同类型的值

  [adlist.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/adlist.h)

  <details>
  <summary>链表</summary>

  ```c++
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

  [adlist.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/adlist.c)

  <details>
  <summary>链表</summary>

  ```c++
  //创建链表
  /* Create a new list. The created list can be freed with
  * AlFreeList(), but private value of every node need to be freed
  * by the user before to call AlFreeList().
  *
  * On error, NULL is returned. Otherwise the pointer to the new list. */
  list *listCreate(void)
  {
      struct list *list;

      if ((list = zmalloc(sizeof(*list))) == NULL)
          return NULL;
      list->head = list->tail = NULL;
      list->len = 0;
      list->dup = NULL;
      list->free = NULL;
      list->match = NULL;
      return list;
  }

  //释放整个链表
  /* Free the whole list.
  *
  * This function can't fail. */
  void listRelease(list *list)
  {
      unsigned long len;
      listNode *current, *next;

      //从头开始遍历
      current = list->head;
      len = list->len;
      while (len--)
      {
          next = current->next;
          //释放节点
          if (list->free)
              list->free(current->value);
          zfree(current);
          //next
          current = next;
      }
      //释放链表
      zfree(list);
  }

  //头插法插入头
  /* Add a new node to the list, to head, containing the specified 'value'
  * pointer as value.
  *
  * On error, NULL is returned and no operation is performed (i.e. the
  * list remains unaltered).
  * On success the 'list' pointer you pass to the function is returned. */
  list *listAddNodeHead(list *list, void *value)
  {
      listNode *node;

      //构造节点
      if ((node = zmalloc(sizeof(*node))) == NULL)
          return NULL;
      node->value = value;
      if (list->len == 0)
      {
          list->head = list->tail = node;
          node->prev = node->next = NULL;
      }
      else
      {
          //头插法
          node->prev = NULL;
          node->next = list->head;
          list->head->prev = node;
          list->head = node;
      }
      list->len++;
      return list;
  }

  //头插法插入尾
  /* Add a new node to the list, to tail, containing the specified 'value'
  * pointer as value.
  *
  * On error, NULL is returned and no operation is performed (i.e. the
  * list remains unaltered).
  * On success the 'list' pointer you pass to the function is returned. */
  list *listAddNodeTail(list *list, void *value)
  {
      listNode *node;

      //构造节点
      if ((node = zmalloc(sizeof(*node))) == NULL)
          return NULL;
      node->value = value;
      if (list->len == 0)
      {
          list->head = list->tail = node;
          node->prev = node->next = NULL;
      }
      else
      {
          //头插法
          node->prev = list->tail;
          node->next = NULL;
          list->tail->next = node;
          list->tail = node;
      }
      list->len++;
      return list;
  }

  //插入节点
  list *listInsertNode(list *list, listNode *old_node, void *value, int after)
  {
      listNode *node;

      //构造节点
      if ((node = zmalloc(sizeof(*node))) == NULL)
          return NULL;
      node->value = value;
      //更新新节点的前驱和后继
      if (after)
      {
          node->prev = old_node;
          node->next = old_node->next;
          if (list->tail == old_node)
          {
              list->tail = node;
          }
      }
      else
      {
          node->next = old_node;
          node->prev = old_node->prev;
          if (list->head == old_node)
          {
              list->head = node;
          }
      }
      if (node->prev != NULL)
      {
          node->prev->next = node;
      }
      if (node->next != NULL)
      {
          node->next->prev = node;
      }
      list->len++;
      return list;
  }

  //删除节点
  /* Remove the specified node from the specified list.
  * It's up to the caller to free the private value of the node.
  *
  * This function can't fail. */
  void listDelNode(list *list, listNode *node)
  {
      //更新待删除节点的前后节点
      if (node->prev)
          node->prev->next = node->next;
      else
          list->head = node->next;
      if (node->next)
          node->next->prev = node->prev;
      else
          list->tail = node->prev;
      //释放节点
      if (list->free)
          list->free(node->value);
      zfree(node);
      list->len--;
  }

  //获取链表迭代器
  /* Returns a list iterator 'iter'. After the initialization every
  * call to listNext() will return the next element of the list.
  *
  * This function can't fail. */
  listIter *listGetIterator(list *list, int direction)
  {
      listIter *iter;

      //构造迭代器
      if ((iter = zmalloc(sizeof(*iter))) == NULL)
          return NULL;
      if (direction == AL_START_HEAD)
          iter->next = list->head;
      else
          iter->next = list->tail;
      iter->direction = direction;
      return iter;
  }

  //销毁迭代器
  /* Release the iterator memory */
  void listReleaseIterator(listIter *iter)
  {
      zfree(iter);
  }

  //复位迭代器到头节点
  /* Create an iterator in the list private iterator structure */
  void listRewind(list *list, listIter *li)
  {
      li->next = list->head;
      li->direction = AL_START_HEAD;
  }

  //复位迭代器到尾节点
  void listRewindTail(list *list, listIter *li)
  {
      li->next = list->tail;
      li->direction = AL_START_TAIL;
  }

  //移动迭代器到下一个节点，返回移动前到节点
  /* Return the next element of an iterator.
  * It's valid to remove the currently returned element using
  * listDelNode(), but not to remove other elements.
  *
  * The function returns a pointer to the next element of the list,
  * or NULL if there are no more elements, so the classical usage patter
  * is:
  *
  * iter = listGetIterator(list,<direction>);
  * while ((node = listNext(iter)) != NULL) {
  *     doSomethingWith(listNodeValue(node));
  * }
  *
  * */
  listNode *listNext(listIter *iter)
  {
      listNode *current = iter->next;

      if (current != NULL)
      {
          if (iter->direction == AL_START_HEAD)
              iter->next = current->next;
          else
              iter->next = current->prev;
      }
      return current;
  }

  //复制链表
  /* Duplicate the whole list. On out of memory NULL is returned.
  * On success a copy of the original list is returned.
  *
  * The 'Dup' method set with listSetDupMethod() function is used
  * to copy the node value. Otherwise the same pointer value of
  * the original node is used as value of the copied node.
  *
  * The original list both on success or error is never modified. */
  list *listDup(list *orig)
  {
      list *copy;
      listIter *iter;
      listNode *node;

      //构造新链表
      if ((copy = listCreate()) == NULL)
          return NULL;
      copy->dup = orig->dup;
      copy->free = orig->free;
      copy->match = orig->match;
      //遍历原链表
      iter = listGetIterator(orig, AL_START_HEAD);
      while ((node = listNext(iter)) != NULL)
      {
          void *value;

          //复制节点
          if (copy->dup)
          {
              value = copy->dup(node->value);
              if (value == NULL)
              {
                  listRelease(copy);
                  listReleaseIterator(iter);
                  return NULL;
              }
          }
          else
              value = node->value;
          //插入新节点
          if (listAddNodeTail(copy, value) == NULL)
          {
              listRelease(copy);
              listReleaseIterator(iter);
              return NULL;
          }
      }
      listReleaseIterator(iter);
      return copy;
  }

  //查找节点
  /* Search the list for a node matching a given key.
  * The match is performed using the 'match' method
  * set with listSetMatchMethod(). If no 'match' method
  * is set, the 'value' pointer of every node is directly
  * compared with the 'key' pointer.
  *
  * On success the first matching node pointer is returned
  * (search starts from head). If no matching node exists
  * NULL is returned. */
  listNode *listSearchKey(list *list, void *key)
  {
      listIter *iter;
      listNode *node;

      //遍历节点
      iter = listGetIterator(list, AL_START_HEAD);
      while ((node = listNext(iter)) != NULL)
      {
          //比较
          if (list->match)
          {
              if (list->match(node->value, key))
              {
                  listReleaseIterator(iter);
                  return node;
              }
          }
          else
          {
              if (key == node->value)
              {
                  listReleaseIterator(iter);
                  return node;
              }
          }
      }
      listReleaseIterator(iter);
      return NULL;
  }

  //获取索引处节点
  /* Return the element at the specified zero-based index
  * where 0 is the head, 1 is the element next to head
  * and so on. Negative integers are used in order to count
  * from the tail, -1 is the last element, -2 the penultimate
  * and so on. If the index is out of range NULL is returned. */
  listNode *listIndex(list *list, long index)
  {
      listNode *n;

      if (index < 0)
      {
          //尾部开始查找
          index = (-index) - 1;
          n = list->tail;
          while (index-- && n)
              n = n->prev;
      }
      else
      {
          //头部开始查找
          n = list->head;
          while (index-- && n)
              n = n->next;
      }
      return n;
  }

  //尾节点移动到头部
  /* Rotate the list removing the tail node and inserting it to the head. */
  void listRotate(list *list)
  {
      listNode *tail = list->tail;

      if (listLength(list) <= 1)
          return;

      //移出尾节点
      /* Detach current tail */
      list->tail = tail->prev;
      list->tail->next = NULL;

      //将上面移出的tail节点插入到头部
      /* Move it as head */
      list->head->prev = tail;
      tail->prev = NULL;
      tail->next = list->head;
      list->head = tail;
  }
  ```
  </details>

### 字典

  ![字典](https://github.com/gongluck/images/blob/main/redis/字典.png)

  - 字典中有两个哈希表，一个平时使用，一个在```rehash```时使用
  - 哈希表使用链地址法解决键冲突
  - 对哈希表进行扩展或者收缩操作时，进行```rehash```将现有哈希表的所有键值对重新计算到新哈希表中，整个过程是渐进式的

  [dict.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/dict.h)

  <details>
  <summary>字典</summary>

  ```c++
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

  [dict.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/dict.c)

  <details>
  <summary>字典</summary>

  ```c++
  //是否启用resize
	static int dict_can_resize = 1;

	//强制rehash比率
	static unsigned int dict_force_resize_ratio = 5;

	/* -------------------------- private prototypes ---------------------------- */

	static int _dictExpandIfNeeded(dict *ht);
	static unsigned long _dictNextPower(unsigned long size);
	static int _dictKeyIndex(dict *ht, const void *key);
	static int _dictInit(dict *ht, dictType *type, void *privDataPtr);

	/* -------------------------- hash functions -------------------------------- */

	// 32位整数key哈希计算，不明觉厉
	/* Thomas Wang's 32 bit Mix Function */
	unsigned int dictIntHashFunction(unsigned int key)
	{
			key += ~(key << 15);
			key ^= (key >> 10);
			key += (key << 3);
			key ^= (key >> 6);
			key += ~(key << 11);
			key ^= (key >> 16);
			return key;
	}

	//哈希种子
	static uint32_t dict_hash_function_seed = 5381;

	void dictSetHashFunctionSeed(uint32_t seed)
	{
			dict_hash_function_seed = seed;
	}

	uint32_t dictGetHashFunctionSeed(void)
	{
			return dict_hash_function_seed;
	}

	/* MurmurHash2, by Austin Appleby
	* Note - This code makes a few assumptions about how your machine behaves -
	* 1. We can read a 4-byte value from any address without crashing
	* 2. sizeof(int) == 4
	*
	* And it has a few limitations -
	*
	* 1. It will not work incrementally.
	* 2. It will not produce the same results on little-endian and big-endian
	*    machines.
	*/
	unsigned int dictGenHashFunction(const void *key, int len)
	{
			/* 'm' and 'r' are mixing constants generated offline.
			They're not really 'magic', they just happen to work well.  */
			uint32_t seed = dict_hash_function_seed;
			const uint32_t m = 0x5bd1e995;
			const int r = 24;

			/* Initialize the hash to a 'random' value */
			uint32_t h = seed ^ len;

			/* Mix 4 bytes at a time into the hash */
			const unsigned char *data = (const unsigned char *)key;

			while (len >= 4)
			{
					uint32_t k = *(uint32_t *)data;

					k *= m;
					k ^= k >> r;
					k *= m;

					h *= m;
					h ^= k;

					data += 4;
					len -= 4;
			}

			/* Handle the last few bytes of the input array  */
			switch (len)
			{
			case 3:
					h ^= data[2] << 16;
			case 2:
					h ^= data[1] << 8;
			case 1:
					h ^= data[0];
					h *= m;
			};

			/* Do a few final mixes of the hash to ensure the last few
			* bytes are well-incorporated. */
			h ^= h >> 13;
			h *= m;
			h ^= h >> 15;

			return (unsigned int)h;
	}

	/* And a case insensitive hash function (based on djb hash) */
	unsigned int dictGenCaseHashFunction(const unsigned char *buf, int len)
	{
			unsigned int hash = (unsigned int)dict_hash_function_seed;

			while (len--)
					hash = ((hash << 5) + hash) + (tolower(*buf++)); /* hash * 33 + c */
			return hash;
	}

	/* ----------------------------- API implementation ------------------------- */

	//重置哈希表
	/* Reset a hash table already initialized with ht_init().
	* NOTE: This function should only be called by ht_destroy(). */
	static void _dictReset(dictht *ht)
	{
			ht->table = NULL;
			ht->size = 0;
			ht->sizemask = 0;
			ht->used = 0;
	}

	//创建字典
	/* Create a new hash table */
	dict *dictCreate(dictType *type,
									void *privDataPtr)
	{
			dict *d = zmalloc(sizeof(*d));

			//初始化
			_dictInit(d, type, privDataPtr);
			return d;
	}

	//初始化字典
	/* Initialize the hash table */
	int _dictInit(dict *d, dictType *type,
								void *privDataPtr)
	{
			//重置哈希表
			_dictReset(&d->ht[0]);
			_dictReset(&d->ht[1]);
			d->type = type;
			d->privdata = privDataPtr;
			d->rehashidx = -1;
			d->iterators = 0;
			return DICT_OK;
	}

	// fix字典大小
	/* Resize the table to the minimal size that contains all the elements,
	* but with the invariant of a USED/BUCKETS ratio near to <= 1 */
	int dictResize(dict *d)
	{
			int minimal;

			//判断是否启用resize或者是否正在rehash
			if (!dict_can_resize || dictIsRehashing(d))
					return DICT_ERR;
			//到这一步，不会在rehash
			minimal = d->ht[0].used;
			if (minimal < DICT_HT_INITIAL_SIZE)
					minimal = DICT_HT_INITIAL_SIZE;
			return dictExpand(d, minimal);
	}

	/* Expand or create the hash table */
	int dictExpand(dict *d, unsigned long size)
	{
			dictht n; /* the new hash table */
			//调整size
			unsigned long realsize = _dictNextPower(size);

			/* the size is invalid if it is smaller than the number of
			* elements already inside the hash table */
			if (dictIsRehashing(d) || d->ht[0].used > size)
					return DICT_ERR;

			/* Rehashing to the same table size is not useful. */
			if (realsize == d->ht[0].size)
					return DICT_ERR;

			/* Allocate the new hash table and initialize all pointers to NULL */
			n.size = realsize;
			n.sizemask = realsize - 1;
			//创建节点数组并初始化为0
			n.table = zcalloc(realsize * sizeof(dictEntry *));
			n.used = 0;

			/* Is this the first initialization? If so it's not really a rehashing
			* we just set the first hash table so that it can accept keys. */
			if (d->ht[0].table == NULL)
			{
					d->ht[0] = n;
					return DICT_OK;
			}

			//准备rehash
			/* Prepare a second hash table for incremental rehashing */
			d->ht[1] = n;
			d->rehashidx = 0;
			return DICT_OK;
	}

	//渐进式rehash
	/* Performs N steps of incremental rehashing. Returns 1 if there are still
	* keys to move from the old to the new hash table, otherwise 0 is returned.
	*
	* Note that a rehashing step consists in moving a bucket (that may have more
	* than one key as we use chaining) from the old to the new hash table, however
	* since part of the hash table may be composed of empty spaces, it is not
	* guaranteed that this function will rehash even a single bucket, since it
	* will visit at max N*10 empty buckets in total, otherwise the amount of
	* work it does would be unbound and the function may block for a long time. */
	int dictRehash(dict *d, int n)
	{
			//最大访问空桶数
			int empty_visits = n * 10; /* Max number of empty buckets to visit. */
			if (!dictIsRehashing(d))
					return 0;

			while (n-- && d->ht[0].used != 0)
			{
					dictEntry *de, *nextde;

					/* Note that rehashidx can't overflow as we are sure there are more
					* elements because ht[0].used != 0 */
					assert(d->ht[0].size > (unsigned long)d->rehashidx);
					while (d->ht[0].table[d->rehashidx] == NULL) //空桶
					{
							d->rehashidx++;
							if (--empty_visits == 0) //到达最大访问空桶数
									return 1;            //因为外部循环条件d->ht[0].used != 0，所以可以判断还有未处理的桶
					}
					//处理这个桶的元素
					de = d->ht[0].table[d->rehashidx];
					/* Move all the keys in this bucket from the old to the new hash HT */
					while (de)
					{
							unsigned int h;
							//保存下一个节点
							nextde = de->next;
							//重新计算哈希
							/* Get the index in the new hash table */
							h = dictHashKey(d, de->key) & d->ht[1].sizemask;
							//头插法
							de->next = d->ht[1].table[h];
							d->ht[1].table[h] = de;
							d->ht[0].used--;
							d->ht[1].used++;
							//准备处理下一个节点
							de = nextde;
					}
					// 0号哈希表的rehashidx桶处理完复位
					d->ht[0].table[d->rehashidx] = NULL;
					d->rehashidx++;
			}

			//检查是否处理完整个哈希表
			/* Check if we already rehashed the whole table... */
			if (d->ht[0].used == 0)
			{
					//销毁0号哈希表
					zfree(d->ht[0].table);
					//使1号哈希表成为0号
					d->ht[0] = d->ht[1];
					//复位1号哈希表
					_dictReset(&d->ht[1]);
					//复位rehash索引
					d->rehashidx = -1;
					return 0;
			}

			/* More to rehash... */
			return 1;
	}

	//获取毫秒时间
	long long timeInMilliseconds(void)
	{
			struct timeval tv;

			gettimeofday(&tv, NULL);
			return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
	}

	//在时间段内rehash
	/* Rehash for an amount of time between ms milliseconds and ms+1 milliseconds */
	int dictRehashMilliseconds(dict *d, int ms)
	{
			long long start = timeInMilliseconds();
			int rehashes = 0;

			while (dictRehash(d, 100))
			{
					rehashes += 100;
					if (timeInMilliseconds() - start > ms)
							break;
			}
			return rehashes;
	}

	//单步哈希
	/* This function performs just a step of rehashing, and only if there are
	* no safe iterators bound to our hash table. When we have iterators in the
	* middle of a rehashing we can't mess with the two hash tables otherwise
	* some element can be missed or duplicated.
	*
	* This function is called by common lookup or update operations in the
	* dictionary so that the hash table automatically migrates from H1 to H2
	* while it is actively used. */
	static void _dictRehashStep(dict *d)
	{
			if (d->iterators == 0)
					dictRehash(d, 1);
	}

	//往字典中添加元素
	/* Add an element to the target hash table */
	int dictAdd(dict *d, void *key, void *val)
	{
			//插入元素
			dictEntry *entry = dictAddRaw(d, key);

			if (!entry)
					return DICT_ERR;
			//设置元素值
			dictSetVal(d, entry, val);
			return DICT_OK;
	}

	//往字典中添加元素
	/* Low level add. This function adds the entry but instead of setting
	* a value returns the dictEntry structure to the user, that will make
	* sure to fill the value field as he wishes.
	*
	* This function is also directly exposed to the user API to be called
	* mainly in order to store non-pointers inside the hash value, example:
	*
	* entry = dictAddRaw(dict,mykey);
	* if (entry != NULL) dictSetSignedIntegerVal(entry,1000);
	*
	* Return values:
	*
	* If key already exists NULL is returned.
	* If key was added, the hash entry is returned to be manipulated by the caller.
	*/
	dictEntry *dictAddRaw(dict *d, void *key)
	{
			int index;
			dictEntry *entry;
			dictht *ht;

			//渐进rehash
			if (dictIsRehashing(d))
					_dictRehashStep(d);

			//计算元素在哈希桶中的索引
			/* Get the index of the new element, or -1 if
			* the element already exists. */
			if ((index = _dictKeyIndex(d, key)) == -1)
					return NULL;

			/* Allocate the memory and store the new entry */
			ht = dictIsRehashing(d) ? &d->ht[1] : &d->ht[0]; // rehash中则往ht[1]插入
			entry = zmalloc(sizeof(*entry));
			//头插法插入元素
			entry->next = ht->table[index];
			ht->table[index] = entry;
			ht->used++;

			//设置元素值
			/* Set the hash entry fields. */
			dictSetKey(d, entry, key);
			return entry;
	}

	//插入或修改键值对
	/* Add an element, discarding the old if the key already exists.
	* Return 1 if the key was added from scratch, 0 if there was already an
	* element with such key and dictReplace() just performed a value update
	* operation. */
	int dictReplace(dict *d, void *key, void *val)
	{
			dictEntry *entry, auxentry;

			//元素不存在，新插入成功
			/* Try to add the element. If the key
			* does not exists dictAdd will suceed. */
			if (dictAdd(d, key, val) == DICT_OK)
					return 1;

			//元素存在，查找它
			/* It already exists, get the entry */
			entry = dictFind(d, key);
			// dictSetVal和dictFreeVal可能会有内存申请释放操作，所以下面会有设置新值后要删除旧值的操作
			/* Set the new value and free the old one. Note that it is important
			* to do that in this order, as the value may just be exactly the same
			* as the previous one. In this context, think to reference counting,
			* you want to increment (set), and then decrement (free), and not the
			* reverse. */
			auxentry = *entry;
			//设置新值
			dictSetVal(d, entry, val);
			//释放旧值
			dictFreeVal(d, &auxentry);
			return 0;
	}

	//插入键
	/* dictReplaceRaw() is simply a version of dictAddRaw() that always
	* returns the hash entry of the specified key, even if the key already
	* exists and can't be added (in that case the entry of the already
	* existing key is returned.)
	*
	* See dictAddRaw() for more information. */
	dictEntry *dictReplaceRaw(dict *d, void *key)
	{
			dictEntry *entry = dictFind(d, key);
			//键已存在则返回第一个元素，否则插入
			return entry ? entry : dictAddRaw(d, key);
	}

	//删除匹配元素
	/* Search and remove an element */
	static int dictGenericDelete(dict *d, const void *key, int nofree)
	{
			unsigned int h, idx;
			dictEntry *he, *prevHe;
			int table;

			//哈希表为空，返回error
			if (d->ht[0].size == 0)
					return DICT_ERR; /* d->ht[0].table is NULL */
			//渐进哈希
			if (dictIsRehashing(d))
					_dictRehashStep(d);
			//计算哈希桶索引
			h = dictHashKey(d, key);

			//遍历两个哈希表
			for (table = 0; table <= 1; table++)
			{
					//计算哈希桶索引
					idx = h & d->ht[table].sizemask;
					//遍历哈希桶
					he = d->ht[table].table[idx];
					prevHe = NULL;
					while (he)
					{
							//比较键
							if (dictCompareKeys(d, key, he->key))
							{
									//调整链表
									/* Unlink the element from the list */
									if (prevHe)
											prevHe->next = he->next;
									else
											d->ht[table].table[idx] = he->next;
									if (!nofree)
									{
											//释放键和值
											dictFreeKey(d, he);
											dictFreeVal(d, he);
									}
									//释放节点
									zfree(he);
									d->ht[table].used--;
									return DICT_OK;
							}
							prevHe = he;
							he = he->next;
					}
					//非rehash，跳过另一个哈希表
					if (!dictIsRehashing(d))
							break;
			}
			return DICT_ERR; /* not found */
	}

	//删除元素
	int dictDelete(dict *ht, const void *key)
	{
			return dictGenericDelete(ht, key, 0);
	}
	int dictDeleteNoFree(dict *ht, const void *key)
	{
			return dictGenericDelete(ht, key, 1);
	}

	//销毁哈希表
	/* Destroy an entire dictionary */
	int _dictClear(dict *d, dictht *ht, void(callback)(void *))
	{
			unsigned long i;

			//销毁所有元素
			/* Free all the elements */
			for (i = 0; i < ht->size && ht->used > 0; i++)
			{
					dictEntry *he, *nextHe;

					if (callback && (i & 65535) == 0)
							callback(d->privdata);

					//跳过空桶
					if ((he = ht->table[i]) == NULL)
							continue;
					//遍历整个桶
					while (he)
					{
							nextHe = he->next;
							//销毁键和值
							dictFreeKey(d, he);
							dictFreeVal(d, he);
							//销毁元素
							zfree(he);
							ht->used--;
							he = nextHe;
					}
			}
			//销毁元素链表
			/* Free the table and the allocated cache structure */
			zfree(ht->table);
			//复位哈希表
			/* Re-initialize the table */
			_dictReset(ht);
			return DICT_OK; /* never fails */
	}

	//销毁字典
	/* Clear & Release the hash table */
	void dictRelease(dict *d)
	{
			_dictClear(d, &d->ht[0], NULL);
			_dictClear(d, &d->ht[1], NULL);
			zfree(d);
	}

	//查找
	dictEntry *dictFind(dict *d, const void *key)
	{
			dictEntry *he;
			unsigned int h, idx, table;

			//没有元素
			if (d->ht[0].size == 0)
					return NULL; /* We don't have a table at all */
			//渐进哈希
			if (dictIsRehashing(d))
					_dictRehashStep(d);
			//计算桶索引
			h = dictHashKey(d, key);
			//遍历两个哈希表
			for (table = 0; table <= 1; table++)
			{
					//计算元素索引
					idx = h & d->ht[table].sizemask;
					//遍历元素链表
					he = d->ht[table].table[idx];
					while (he)
					{
							//比较
							if (dictCompareKeys(d, key, he->key))
									return he;
							he = he->next;
					}
					//非rehash，跳过另外一个哈希表
					if (!dictIsRehashing(d))
							return NULL;
			}
			return NULL;
	}

	//获取元素value
	void *dictFetchValue(dict *d, const void *key)
	{
			dictEntry *he;

			he = dictFind(d, key);
			return he ? dictGetVal(he) : NULL;
	}

	/* A fingerprint is a 64 bit number that represents the state of the dictionary
	* at a given time, it's just a few dict properties xored together.
	* When an unsafe iterator is initialized, we get the dict fingerprint, and check
	* the fingerprint again when the iterator is released.
	* If the two fingerprints are different it means that the user of the iterator
	* performed forbidden operations against the dictionary while iterating. */
	long long dictFingerprint(dict *d)
	{
			long long integers[6], hash = 0;
			int j;

			integers[0] = (long)d->ht[0].table;
			integers[1] = d->ht[0].size;
			integers[2] = d->ht[0].used;
			integers[3] = (long)d->ht[1].table;
			integers[4] = d->ht[1].size;
			integers[5] = d->ht[1].used;

			/* We hash N integers by summing every successive integer with the integer
			* hashing of the previous sum. Basically:
			*
			* Result = hash(hash(hash(int1)+int2)+int3) ...
			*
			* This way the same set of integers in a different order will (likely) hash
			* to a different number. */
			for (j = 0; j < 6; j++)
			{
					hash += integers[j];
					/* For the hashing step we use Tomas Wang's 64 bit integer hash. */
					hash = (~hash) + (hash << 21); // hash = (hash << 21) - hash - 1;
					hash = hash ^ (hash >> 24);
					hash = (hash + (hash << 3)) + (hash << 8); // hash * 265
					hash = hash ^ (hash >> 14);
					hash = (hash + (hash << 2)) + (hash << 4); // hash * 21
					hash = hash ^ (hash >> 28);
					hash = hash + (hash << 31);
			}
			return hash;
	}

	//获取字典不安全迭代器
	dictIterator *dictGetIterator(dict *d)
	{
			dictIterator *iter = zmalloc(sizeof(*iter));

			iter->d = d;
			iter->table = 0;
			iter->index = -1;
			iter->safe = 0;
			iter->entry = NULL;
			iter->nextEntry = NULL;
			return iter;
	}

	//获取字典安全迭代器
	dictIterator *dictGetSafeIterator(dict *d)
	{
			dictIterator *i = dictGetIterator(d);

			i->safe = 1;
			return i;
	}

	//遍历迭代器
	dictEntry *dictNext(dictIterator *iter)
	{
			while (1)
			{
					//第一次遍历或者当前哈希表遍历到结束
					if (iter->entry == NULL)
					{
							//获取指向的哈希表
							dictht *ht = &iter->d->ht[iter->table];
							//第一次遍历
							if (iter->index == -1 && iter->table == 0)
							{
									if (iter->safe)
											iter->d->iterators++; //安全迭代器，更新迭代器计数
									else
											iter->fingerprint = dictFingerprint(iter->d); //计算指纹
							}
							//更新索引
							iter->index++;
							//索引溢出，当前哈希表遍历完
							if (iter->index >= (long)ht->size)
							{
									// rehash中代表应该遍历ht[1]
									if (dictIsRehashing(iter->d) && iter->table == 0)
									{
											//准备遍历另外一个哈希表
											iter->table++;
											iter->index = 0;
											ht = &iter->d->ht[1];
									}
									else
									{
											break;
									}
							}
							//指向节点
							iter->entry = ht->table[iter->index];
					}
					else //非第一次遍历或者当前哈希表遍历到结束
					{
							//指向下一个节点
							iter->entry = iter->nextEntry;
					}
					if (iter->entry) //当前节点有效
					{
							/* We need to save the 'next' here, the iterator user
							* may delete the entry we are returning. */
							iter->nextEntry = iter->entry->next;
							return iter->entry;
					}
			}
			return NULL;
	}

	//释放迭代器
	void dictReleaseIterator(dictIterator *iter)
	{
			if (!(iter->index == -1 && iter->table == 0))
			{
					if (iter->safe)
							iter->d->iterators--; //递减安全迭代器的计数
					else
							assert(iter->fingerprint == dictFingerprint(iter->d)); //验证指纹
			}
			zfree(iter);
	}

	//随机返回节点
	/* Return a random entry from the hash table. Useful to
	* implement randomized algorithms */
	dictEntry *dictGetRandomKey(dict *d)
	{
			dictEntry *he, *orighe;
			unsigned int h;
			int listlen, listele;

			if (dictSize(d) == 0)
					return NULL;
			//渐进哈希
			if (dictIsRehashing(d))
					_dictRehashStep(d);
			// rehash中
			if (dictIsRehashing(d))
			{
					do
					{
							// ht[0]中的[0, rehashidx]是没有元素的
							/* We are sure there are no elements in indexes from 0
							* to rehashidx-1 */
							h = d->rehashidx + (random() % (d->ht[0].size +
																							d->ht[1].size -
																							d->rehashidx));
							he = (h >= d->ht[0].size) ? d->ht[1].table[h - d->ht[0].size] : d->ht[0].table[h];
					} while (he == NULL);
			}
			else
			{
					do
					{
							//计算随机索引
							h = random() & d->ht[0].sizemask;
							he = d->ht[0].table[h];
					} while (he == NULL);
			}

			/* Now we found a non empty bucket, but it is a linked
			* list and we need to get a random element from the list.
			* The only sane way to do so is counting the elements and
			* select a random index. */
			listlen = 0;
			orighe = he;
			//遍历当前桶的链表长度
			while (he)
			{
					he = he->next;
					listlen++;
			}
			//随机一个链表索引
			listele = random() % listlen;
			he = orighe;
			while (listele--)
					he = he->next;
			return he;
	}

	//随机返回一些节点
	/* This function samples the dictionary to return a few keys from random
	* locations.
	*
	* It does not guarantee to return all the keys specified in 'count', nor
	* it does guarantee to return non-duplicated elements, however it will make
	* some effort to do both things.
	*
	* Returned pointers to hash table entries are stored into 'des' that
	* points to an array of dictEntry pointers. The array must have room for
	* at least 'count' elements, that is the argument we pass to the function
	* to tell how many random elements we need.
	*
	* The function returns the number of items stored into 'des', that may
	* be less than 'count' if the hash table has less than 'count' elements
	* inside, or if not enough elements were found in a reasonable amount of
	* steps.
	*
	* Note that this function is not suitable when you need a good distribution
	* of the returned items, but only when you need to "sample" a given number
	* of continuous elements to run some kind of algorithm or to produce
	* statistics. However the function is much faster than dictGetRandomKey()
	* at producing N elements. */
	unsigned int dictGetSomeKeys(dict *d, dictEntry **des, unsigned int count)
	{
			unsigned int j;      /* internal hash table id, 0 or 1. */
			unsigned int tables; /* 1 or 2 tables? */
			unsigned int stored = 0, maxsizemask;
			unsigned int maxsteps;

			if (dictSize(d) < count)
					count = dictSize(d);
			maxsteps = count * 10;

			//尝试进行一些rehash
			/* Try to do a rehashing work proportional to 'count'. */
			for (j = 0; j < count; j++)
			{
					if (dictIsRehashing(d))
							_dictRehashStep(d);
					else
							break;
			}

			tables = dictIsRehashing(d) ? 2 : 1;
			maxsizemask = d->ht[0].sizemask;
			if (tables > 1 && maxsizemask < d->ht[1].sizemask)
					maxsizemask = d->ht[1].sizemask;

			//计算随机索引
			/* Pick a random point inside the larger table. */
			unsigned int i = random() & maxsizemask;
			unsigned int emptylen = 0; /* Continuous empty entries so far. */
			while (stored < count && maxsteps--)
			{
					for (j = 0; j < tables; j++)
					{
							/* Invariant of the dict.c rehashing: up to the indexes already
							* visited in ht[0] during the rehashing, there are no populated
							* buckets, so we can skip ht[0] for indexes between 0 and idx-1. */
							if (tables == 2 && j == 0 && i < (unsigned int)d->rehashidx) //取ht[1]的情况
							{
									/* Moreover, if we are currently out of range in the second
									* table, there will be no elements in both tables up to
									* the current rehashing index, so we jump if possible.
									* (this happens when going from big to small table). */
									if (i >= d->ht[1].size) //防止溢出
											i = d->rehashidx;
									continue;
							}
							if (i >= d->ht[j].size) //溢出
									continue;           /* Out of range for this table. */
							//获取随机索引指向的桶
							dictEntry *he = d->ht[j].table[i];

							/* Count contiguous empty buckets, and jump to other
							* locations if they reach 'count' (with a minimum of 5). */
							if (he == NULL)
							{
									emptylen++;
									if (emptylen >= 5 && emptylen > count)
									{
											//重新计算一次随机索引
											i = random() & maxsizemask;
											emptylen = 0;
									}
							}
							else
							{
									emptylen = 0;
									while (he)
									{
											/* Collect all the elements of the buckets found non
											* empty while iterating. */
											*des = he; //添加到结果数组
											des++;
											he = he->next;
											stored++;
											if (stored == count)
													return stored;
									}
							}
					}
					//遍历下一个桶
					i = (i + 1) & maxsizemask;
			}
			return stored;
	}

	//按位反转
	/* Function to reverse bits. Algorithm from:
	* http://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel */
	static unsigned long rev(unsigned long v)
	{
			unsigned long s = 8 * sizeof(v); // bit size; must be power of 2
			unsigned long mask = ~0;
			while ((s >>= 1) > 0)
			{
					mask ^= (mask << s);
					v = ((v >> s) & mask) | ((v << s) & ~mask);
			}
			return v;
	}

	//遍历
	/* dictScan() is used to iterate over the elements of a dictionary.
	*
	* Iterating works the following way:
	*
	* 1) Initially you call the function using a cursor (v) value of 0.
	* 2) The function performs one step of the iteration, and returns the
	*    new cursor value you must use in the next call.
	* 3) When the returned cursor is 0, the iteration is complete.
	*
	* The function guarantees all elements present in the
	* dictionary get returned between the start and end of the iteration.
	* However it is possible some elements get returned multiple times.
	*
	* For every element returned, the callback argument 'fn' is
	* called with 'privdata' as first argument and the dictionary entry
	* 'de' as second argument.
	*
	* HOW IT WORKS.
	*
	* The iteration algorithm was designed by Pieter Noordhuis.
	* The main idea is to increment a cursor starting from the higher order
	* bits. That is, instead of incrementing the cursor normally, the bits
	* of the cursor are reversed, then the cursor is incremented, and finally
	* the bits are reversed again.
	*
	* This strategy is needed because the hash table may be resized between
	* iteration calls.
	*
	* dict.c hash tables are always power of two in size, and they
	* use chaining, so the position of an element in a given table is given
	* by computing the bitwise AND between Hash(key) and SIZE-1
	* (where SIZE-1 is always the mask that is equivalent to taking the rest
	*  of the division between the Hash of the key and SIZE).
	*
	* For example if the current hash table size is 16, the mask is
	* (in binary) 1111. The position of a key in the hash table will always be
	* the last four bits of the hash output, and so forth.
	*
	* WHAT HAPPENS IF THE TABLE CHANGES IN SIZE?
	*
	* If the hash table grows, elements can go anywhere in one multiple of
	* the old bucket: for example let's say we already iterated with
	* a 4 bit cursor 1100 (the mask is 1111 because hash table size = 16).
	*
	* If the hash table will be resized to 64 elements, then the new mask will
	* be 111111. The new buckets you obtain by substituting in ??1100
	* with either 0 or 1 can be targeted only by keys we already visited
	* when scanning the bucket 1100 in the smaller hash table.
	*
	* By iterating the higher bits first, because of the inverted counter, the
	* cursor does not need to restart if the table size gets bigger. It will
	* continue iterating using cursors without '1100' at the end, and also
	* without any other combination of the final 4 bits already explored.
	*
	* Similarly when the table size shrinks over time, for example going from
	* 16 to 8, if a combination of the lower three bits (the mask for size 8
	* is 111) were already completely explored, it would not be visited again
	* because we are sure we tried, for example, both 0111 and 1111 (all the
	* variations of the higher bit) so we don't need to test it again.
	*
	* WAIT... YOU HAVE *TWO* TABLES DURING REHASHING!
	*
	* Yes, this is true, but we always iterate the smaller table first, then
	* we test all the expansions of the current cursor into the larger
	* table. For example if the current cursor is 101 and we also have a
	* larger table of size 16, we also test (0)101 and (1)101 inside the larger
	* table. This reduces the problem back to having only one table, where
	* the larger one, if it exists, is just an expansion of the smaller one.
	*
	* LIMITATIONS
	*
	* This iterator is completely stateless, and this is a huge advantage,
	* including no additional memory used.
	*
	* The disadvantages resulting from this design are:
	*
	* 1) It is possible we return elements more than once. However this is usually
	*    easy to deal with in the application level.
	* 2) The iterator must return multiple elements per call, as it needs to always
	*    return all the keys chained in a given bucket, and all the expansions, so
	*    we are sure we don't miss keys moving during rehashing.
	* 3) The reverse cursor is somewhat hard to understand at first, but this
	*    comment is supposed to help.
	*/
	unsigned long dictScan(dict *d,
												unsigned long v,
												dictScanFunction *fn,
												void *privdata)
	{
			dictht *t0, *t1;
			const dictEntry *de;
			unsigned long m0, m1;

			if (dictSize(d) == 0)
					return 0;

			if (!dictIsRehashing(d))
			{
					t0 = &(d->ht[0]);
					m0 = t0->sizemask;

					//遍历该索引的桶
					/* Emit entries at cursor */
					de = t0->table[v & m0];
					while (de)
					{
							fn(privdata, de);
							de = de->next;
					}
			}
			else
			{
					t0 = &d->ht[0];
					t1 = &d->ht[1];

					/* Make sure t0 is the smaller and t1 is the bigger table */
					if (t0->size > t1->size)
					{
							t0 = &d->ht[1];
							t1 = &d->ht[0];
					}

					m0 = t0->sizemask;
					m1 = t1->sizemask;

					//遍历该索引的桶
					/* Emit entries at cursor */
					de = t0->table[v & m0];
					while (de)
					{
							fn(privdata, de);
							de = de->next;
					}

					/* Iterate over indices in larger table that are the expansion
					* of the index pointed to by the cursor in the smaller table */
					do
					{
							//遍历另外一个哈希表中的对应桶
							/* Emit entries at cursor */
							de = t1->table[v & m1];
							while (de)
							{
									fn(privdata, de);
									de = de->next;
							}

							//考虑由ht[0]的v桶在rehash过程中移动到ht[1]的节点
							/* Increment bits not covered by the smaller mask */
							v = (((v | m0) + 1) & ~m0) | (v & m0);

							/* Continue while bits covered by mask difference is non-zero */
					} while (v & (m0 ^ m1)); // v溢出
			}

			/* Set unmasked bits so incrementing the reversed cursor
			* operates on the masked bits of the smaller table */
			v |= ~m0; //去除溢出的高位

			/* Increment the reverse cursor */
			v = rev(v);
			v++;
			v = rev(v);

			return v;
	}

	/* ------------------------- private functions ------------------------------ */

	//如果需要，扩展字典
	/* Expand the hash table if needed */
	static int _dictExpandIfNeeded(dict *d)
	{
			// rehash中，返回成功
			/* Incremental rehashing already in progress. Return. */
			if (dictIsRehashing(d))
					return DICT_OK;

			//哈希表为空，扩展到初始大小
			/* If the hash table is empty expand it to the initial size. */
			if (d->ht[0].size == 0)
					return dictExpand(d, DICT_HT_INITIAL_SIZE);

			//节点数过多，翻倍空间
			/* If we reached the 1:1 ratio, and we are allowed to resize the hash
			* table (global setting) or we should avoid it but the ratio between
			* elements/buckets is over the "safe" threshold, we resize doubling
			* the number of buckets. */
			if (d->ht[0].used >= d->ht[0].size &&
					(dict_can_resize ||
					d->ht[0].used / d->ht[0].size > dict_force_resize_ratio))
			{
					return dictExpand(d, d->ht[0].used * 2);
			}
			return DICT_OK;
	}

	//计算最小的大于等于size的2的n次幂
	/* Our hash table capability is a power of two */
	static unsigned long _dictNextPower(unsigned long size)
	{
			unsigned long i = DICT_HT_INITIAL_SIZE;

			if (size >= LONG_MAX)
					return LONG_MAX;
			while (1)
			{
					if (i >= size)
							return i;
					i *= 2;
			}
	}

	//返回键元素在桶中的索引，如果元素已经存在则返回-1
	/* Returns the index of a free slot that can be populated with
	* a hash entry for the given 'key'.
	* If the key already exists, -1 is returned.
	*
	* Note that if we are in the process of rehashing the hash table, the
	* index is always returned in the context of the second (new) hash table. */
	static int _dictKeyIndex(dict *d, const void *key)
	{
			unsigned int h, idx, table;
			dictEntry *he;

			/* Expand the hash table if needed */
			if (_dictExpandIfNeeded(d) == DICT_ERR)
					return -1;
			//计算哈希
			/* Compute the key hash value */
			h = dictHashKey(d, key);
			//考虑rehash情况，遍历两个哈希表
			for (table = 0; table <= 1; table++)
			{
					//计算索引
					idx = h & d->ht[table].sizemask;
					//查找元素是否已存在
					/* Search if this slot does not already contain the given key */
					he = d->ht[table].table[idx];
					while (he)
					{
							if (dictCompareKeys(d, key, he->key)) //元素已存在
									return -1;
							he = he->next;
					}
					//不是rehash中，不用考虑ht[1]了
					if (!dictIsRehashing(d))
							break;
			}
			return idx;
	}

	void dictEmpty(dict *d, void(callback)(void *))
	{
			_dictClear(d, &d->ht[0], callback);
			_dictClear(d, &d->ht[1], callback);
			d->rehashidx = -1;
			d->iterators = 0;
	}

	void dictEnableResize(void)
	{
			dict_can_resize = 1;
	}

	void dictDisableResize(void)
	{
			dict_can_resize = 0;
	}
  ```
  </details>

### 跳表

  ![跳表](https://github.com/gongluck/images/blob/main/redis/跳表.png)

  - 跳表节点的层高是```1~32```之间的随机数
  - 多个节点可以包含相同的分值，但节点的成员对象必须是唯一的
  - 节点按照分值排序，分值相同时按照对象的大小进行排序

  [redis.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/redis.h)

  <details>
  <summary>跳表</summary>

  ```c++
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

  [t_zset.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/t_zset.c)

  <details>
  <summary>跳表</summary>

  ```c++
  //创建跳表节点
  zskiplistNode *zslCreateNode(int level, double score, robj *obj)
  {
    zskiplistNode *zn = zmalloc(sizeof(*zn) + level * sizeof(struct zskiplistLevel));
    zn->score = score;
    zn->obj = obj;
    return zn;
  }

  //创建跳表
  zskiplist *zslCreate(void)
  {
    int j;
    zskiplist *zsl;

    zsl = zmalloc(sizeof(*zsl));
    //空表层数为1
    zsl->level = 1;
    //空表长度0
    zsl->length = 0;
    //头节点不计入长度，层数为最大值32
    zsl->header = zslCreateNode(ZSKIPLIST_MAXLEVEL, 0, NULL);
    for (j = 0; j < ZSKIPLIST_MAXLEVEL; j++)
    {
      zsl->header->level[j].forward = NULL;
      zsl->header->level[j].span = 0;
    }
    zsl->header->backward = NULL;
    zsl->tail = NULL;
    return zsl;
  }

  //释放跳表节点
  void zslFreeNode(zskiplistNode *node)
  {
    //减少对象引用计数
    decrRefCount(node->obj);
    zfree(node);
  }

  //释放跳表
  void zslFree(zskiplist *zsl)
  {
    zskiplistNode *node = zsl->header->level[0].forward /*第一个有效节点*/, *next;

    //循环释放所有节点
    zfree(zsl->header);
    while (node)
    {
      //只需遍历第0层节点
      next = node->level[0].forward;
      //释放节点
      zslFreeNode(node);
      node = next;
    }
    //释放跳表结构本身
    zfree(zsl);
  }

  //计算一个随机层数
  /* Returns a random level for the new skiplist node we are going to create.
  * The return value of this function is between 1 and ZSKIPLIST_MAXLEVEL
  * (both inclusive), with a powerlaw-alike distribution where higher
  * levels are less likely to be returned. */
  int zslRandomLevel(void)
  {
    int level = 1;
    while ((random() & 0xFFFF) < (ZSKIPLIST_P * 0xFFFF))
      level += 1;
    return (level < ZSKIPLIST_MAXLEVEL) ? level : ZSKIPLIST_MAXLEVEL;
  }

  //插入节点
  zskiplistNode *zslInsert(zskiplist *zsl, double score, robj *obj)
  {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    unsigned int rank[ZSKIPLIST_MAXLEVEL];
    int i, level;

    redisAssert(!isnan(score));
    //从头节点最高层出发，查找每一层最靠近新节点的前继节点
    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      // rank[i]为每层最靠近新节点的前继节点的距离
      /* store rank that is crossed to reach the insert position */
      rank[i] = i == (zsl->level - 1) ? 0 : rank[i + 1]; //动态规划！
      while (x->level[i].forward &&                      //如果后继节点有效
            (x->level[i].forward->score < score ||      //并且后继节点的比值小于等于新节点的比值
              (x->level[i].forward->score == score &&
              compareStringObjects(x->level[i].forward->obj, obj) < 0)))
      {
        rank[i] += x->level[i].span; //累加距离
        x = x->level[i].forward;     //继续下一个后继节点
      }
      // x所指向的节点为i层最靠近新节点的前继节点，记录前继节点
      update[i] = x;
    }
    /* we assume the key is not already inside, since we allow duplicated
    * scores, and the re-insertion of score and redis object should never
    * happen since the caller of zslInsert() should test in the hash table
    * if the element is already inside or not. */
    level = zslRandomLevel(); //随机层数
    if (level > zsl->level)   //最大层数变化
    {
      //新增update[zsl->level]~update[level]
      for (i = zsl->level; i < level; i++)
      {
        rank[i] = 0;
        update[i] = zsl->header;                //新增层从头节点开始
        update[i]->level[i].span = zsl->length; //新增层只加这个新节点
      }
      zsl->level = level;
    }
    //创建节点
    x = zslCreateNode(level, score, obj);
    //更新所有旧层
    for (i = 0; i < level; i++)
    {
      //头插入法插入到后继节点前
      x->level[i].forward = update[i]->level[i].forward;
      update[i]->level[i].forward = x;

      //更新span
      /* update span covered by update[i] as x is inserted here */
      x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
      update[i]->level[i].span = (rank[0] - rank[i]) + 1;
    }

    //如果没有新增层，更新update[zsl->level]~update[level]
    /* increment span for untouched levels */
    for (i = level; i < zsl->level; i++)
    {
      //对[level,zsl->level)的后继节点中的跨度+1，因为新插入一个节点
      update[i]->level[i].span++;
    }

    x->backward = (update[0] == zsl->header) ? NULL : update[0]; // 0层相当于普通链表
    if (x->level[0].forward)
      x->level[0].forward->backward = x;
    else
      zsl->tail = x;
    zsl->length++;
    return x;
  }

  //删除跳表节点
  /* Internal function used by zslDelete, zslDeleteByScore and zslDeleteByRank */
  void zslDeleteNode(zskiplist *zsl, zskiplistNode *x, zskiplistNode **update)
  {
    int i;
    //遍历每一层前继节点
    for (i = 0; i < zsl->level; i++)
    {
      if (update[i]->level[i].forward == x) //下一节点就是删除节点
      {
        update[i]->level[i].span += x->level[i].span - 1;  //增加距离
        update[i]->level[i].forward = x->level[i].forward; //移除x节点
      }
      else
      {
        update[i]->level[i].span -= 1;
      }
    }
    //待删除节点有后继
    if (x->level[0].forward)
    {
      x->level[0].forward->backward = x->backward;
    }
    else
    {
      zsl->tail = x->backward;
    }
    //更新最大层数
    while (zsl->level > 1 && zsl->header->level[zsl->level - 1].forward == NULL)
      zsl->level--;
    zsl->length--;
  }

  //删除跳表节点
  /* Delete an element with matching score/object from the skiplist. */
  int zslDelete(zskiplist *zsl, double score, robj *obj)
  {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    int i;

    //从头节点最高层出发，查找每一层最靠近查询节点的前继节点
    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      while (x->level[i].forward &&                 //如果后继节点有效
            (x->level[i].forward->score < score || //并且后继节点的比值小于等于查询节点的比值
              (x->level[i].forward->score == score &&
              compareStringObjects(x->level[i].forward->obj, obj) < 0)))
      {
        x = x->level[i].forward; //继续下一个后继节点
      }
      update[i] = x; //每一层最靠近查询节点的前继节点
    }
    //找出后置节点中比值符合并且对象相等的节点
    /* We may have multiple elements with the same score, what we need
    * is to find the element with both the right score and object. */
    x = x->level[0].forward;
    if (x && score == x->score && equalStringObjects(x->obj, obj))
    {
      //删除节点
      zslDeleteNode(zsl, x, update);
      zslFreeNode(x);
      return 1;
    }
    return 0; /* not found */
  }

  static int zslValueGteMin(double value, zrangespec *spec)
  {
    return spec->minex ? (value > spec->min) : (value >= spec->min);
  }

  static int zslValueLteMax(double value, zrangespec *spec)
  {
    return spec->maxex ? (value < spec->max) : (value <= spec->max);
  }

  //判断跳表节点是否在范围中
  /* Returns if there is a part of the zset is in range. */
  int zslIsInRange(zskiplist *zsl, zrangespec *range)
  {
    zskiplistNode *x;

    //排除空集
    /* Test for ranges that will always be empty. */
    if (range->min > range->max ||
        (range->min == range->max && (range->minex || range->maxex)))
      return 0;
    //尾节点
    x = zsl->tail;
    //判断尾节点是否在超出范围
    if (x == NULL || !zslValueGteMin(x->score, range))
      return 0;
    //第一个有效节点
    x = zsl->header->level[0].forward;
    //判断第一个有效节点是否在超出范围
    if (x == NULL || !zslValueLteMax(x->score, range))
      return 0;
    return 1;
  }

  //获取第一个在范围中的节点
  /* Find the first node that is contained in the specified range.
  * Returns NULL when no element is contained in the range. */
  zskiplistNode *zslFirstInRange(zskiplist *zsl, zrangespec *range)
  {
    zskiplistNode *x;
    int i;

    //判断跳表是否在范围中
    /* If everything is out of range, return early. */
    if (!zslIsInRange(zsl, range))
      return NULL;

    //从头节点最高层出发，查找每一层最靠近的前置节点
    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      /* Go forward while *OUT* of range. */
      while (x->level[i].forward &&                              //后继节点有效
            !zslValueGteMin(x->level[i].forward->score, range)) //并且后继节点在范围中
        x = x->level[i].forward;
    }

    //后继节点就是结果
    /* This is an inner range, so the next node cannot be NULL. */
    x = x->level[0].forward;
    redisAssert(x != NULL);

    //判断结果是否在范围中
    /* Check if score <= max. */
    if (!zslValueLteMax(x->score, range))
      return NULL;
    return x;
  }

  //获取最后一个在范围中的节点
  /* Find the last node that is contained in the specified range.
  * Returns NULL when no element is contained in the range. */
  zskiplistNode *zslLastInRange(zskiplist *zsl, zrangespec *range)
  {
    zskiplistNode *x;
    int i;

    //判断跳表是否在范围中
    /* If everything is out of range, return early. */
    if (!zslIsInRange(zsl, range))
      return NULL;

    //从头节点最高层出发，查找每一层最靠近的前置节点
    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      /* Go forward while *IN* range. */
      while (x->level[i].forward &&                             //如果后继节点有效
            zslValueLteMax(x->level[i].forward->score, range)) //并且后继节点在范围中
        x = x->level[i].forward;
    }

    /* This is an inner range, so this node cannot be NULL. */
    redisAssert(x != NULL);

    //判断结果是否在范围中
    /* Check if score >= min. */
    if (!zslValueGteMin(x->score, range))
      return NULL;
    return x;
  }

  //删除范围中的节点
  /* Delete all the elements with score between min and max from the skiplist.
  * Min and max are inclusive, so a score >= min || score <= max is deleted.
  * Note that this function takes the reference to the hash table view of the
  * sorted set, in order to remove the elements from the hash table too. */
  unsigned long zslDeleteRangeByScore(zskiplist *zsl, zrangespec *range, dict *dict)
  {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    unsigned long removed = 0;
    int i;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      while (x->level[i].forward && (range->minex ? x->level[i].forward->score <= range->min : x->level[i].forward->score < range->min))
        x = x->level[i].forward;
      update[i] = x; //记录最近的前继节点
    }

    //第一个范围中的节点
    /* Current node is the last with score < or <= min. */
    x = x->level[0].forward;

    //删除范围中的节点
    /* Delete nodes while in range. */
    while (x &&
          (range->maxex ? x->score < range->max : x->score <= range->max))
    {
      zskiplistNode *next = x->level[0].forward;
      zslDeleteNode(zsl, x, update);
      dictDelete(dict, x->obj);
      zslFreeNode(x);
      removed++;
      x = next;
    }
    return removed;
  }

  unsigned long zslDeleteRangeByLex(zskiplist *zsl, zlexrangespec *range, dict *dict)
  {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    unsigned long removed = 0;
    int i;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      while (x->level[i].forward &&
            !zslLexValueGteMin(x->level[i].forward->obj, range))
        x = x->level[i].forward;
      update[i] = x;
    }

    /* Current node is the last with score < or <= min. */
    x = x->level[0].forward;

    /* Delete nodes while in range. */
    while (x && zslLexValueLteMax(x->obj, range))
    {
      zskiplistNode *next = x->level[0].forward;
      zslDeleteNode(zsl, x, update);
      dictDelete(dict, x->obj);
      zslFreeNode(x);
      removed++;
      x = next;
    }
    return removed;
  }

  /* Delete all the elements with rank between start and end from the skiplist.
  * Start and end are inclusive. Note that start and end need to be 1-based */
  unsigned long zslDeleteRangeByRank(zskiplist *zsl, unsigned int start, unsigned int end, dict *dict)
  {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    unsigned long traversed = 0, removed = 0;
    int i;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      while (x->level[i].forward && (traversed + x->level[i].span) < start)
      {
        traversed += x->level[i].span;
        x = x->level[i].forward;
      }
      update[i] = x;
    }

    traversed++;
    x = x->level[0].forward;
    while (x && traversed <= end)
    {
      zskiplistNode *next = x->level[0].forward;
      zslDeleteNode(zsl, x, update);
      dictDelete(dict, x->obj);
      zslFreeNode(x);
      removed++;
      traversed++;
      x = next;
    }
    return removed;
  }

  //获取符合条件的节点的排位
  /* Find the rank for an element by both score and key.
  * Returns 0 when the element cannot be found, rank otherwise.
  * Note that the rank is 1-based due to the span of zsl->header to the
  * first element. */
  unsigned long zslGetRank(zskiplist *zsl, double score, robj *o)
  {
    zskiplistNode *x;
    unsigned long rank = 0;
    int i;

    //遍历
    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
              (x->level[i].forward->score == score &&
              compareStringObjects(x->level[i].forward->obj, o) <= 0)))
      {
        rank += x->level[i].span; //累加排位
        x = x->level[i].forward;  //下一个后继节点
      }

      /* x might be equal to zsl->header, so test if obj is non-NULL */
      if (x->obj && equalStringObjects(x->obj, o))
      {
        return rank;
      }
    }
    return 0;
  }

  //查找符合排位的节点
  /* Finds an element by its rank. The rank argument needs to be 1-based. */
  zskiplistNode *zslGetElementByRank(zskiplist *zsl, unsigned long rank)
  {
    zskiplistNode *x;
    unsigned long traversed = 0;
    int i;

    //遍历
    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      while (x->level[i].forward && (traversed + x->level[i].span) <= rank) //小于查找排位
      {
        //累加排位
        traversed += x->level[i].span;
        //下一个后继节点
        x = x->level[i].forward;
      }
      //找到结果
      if (traversed == rank)
      {
        return x;
      }
    }
    return NULL;
  }

  /* Populate the rangespec according to the objects min and max. */
  static int zslParseRange(robj *min, robj *max, zrangespec *spec)
  {
    char *eptr;
    spec->minex = spec->maxex = 0;

    /* Parse the min-max interval. If one of the values is prefixed
    * by the "(" character, it's considered "open". For instance
    * ZRANGEBYSCORE zset (1.5 (2.5 will match min < x < max
    * ZRANGEBYSCORE zset 1.5 2.5 will instead match min <= x <= max */
    if (min->encoding == REDIS_ENCODING_INT)
    {
      spec->min = (long)min->ptr;
    }
    else
    {
      if (((char *)min->ptr)[0] == '(')
      {
        spec->min = strtod((char *)min->ptr + 1, &eptr);
        if (eptr[0] != '\0' || isnan(spec->min))
          return REDIS_ERR;
        spec->minex = 1;
      }
      else
      {
        spec->min = strtod((char *)min->ptr, &eptr);
        if (eptr[0] != '\0' || isnan(spec->min))
          return REDIS_ERR;
      }
    }
    if (max->encoding == REDIS_ENCODING_INT)
    {
      spec->max = (long)max->ptr;
    }
    else
    {
      if (((char *)max->ptr)[0] == '(')
      {
        spec->max = strtod((char *)max->ptr + 1, &eptr);
        if (eptr[0] != '\0' || isnan(spec->max))
          return REDIS_ERR;
        spec->maxex = 1;
      }
      else
      {
        spec->max = strtod((char *)max->ptr, &eptr);
        if (eptr[0] != '\0' || isnan(spec->max))
          return REDIS_ERR;
      }
    }

    return REDIS_OK;
  }

  /* ------------------------ Lexicographic ranges ---------------------------- */

  /* Parse max or min argument of ZRANGEBYLEX.
  * (foo means foo (open interval)
  * [foo means foo (closed interval)
  * - means the min string possible
  * + means the max string possible
  *
  * If the string is valid the *dest pointer is set to the redis object
  * that will be used for the comparision, and ex will be set to 0 or 1
  * respectively if the item is exclusive or inclusive. REDIS_OK will be
  * returned.
  *
  * If the string is not a valid range REDIS_ERR is returned, and the value
  * of *dest and *ex is undefined. */
  int zslParseLexRangeItem(robj *item, robj **dest, int *ex)
  {
    char *c = item->ptr;

    switch (c[0])
    {
    case '+':
      if (c[1] != '\0')
        return REDIS_ERR;
      *ex = 0;
      *dest = shared.maxstring;
      incrRefCount(shared.maxstring);
      return REDIS_OK;
    case '-':
      if (c[1] != '\0')
        return REDIS_ERR;
      *ex = 0;
      *dest = shared.minstring;
      incrRefCount(shared.minstring);
      return REDIS_OK;
    case '(':
      *ex = 1;
      *dest = createStringObject(c + 1, sdslen(c) - 1);
      return REDIS_OK;
    case '[':
      *ex = 0;
      *dest = createStringObject(c + 1, sdslen(c) - 1);
      return REDIS_OK;
    default:
      return REDIS_ERR;
    }
  }

  /* Populate the rangespec according to the objects min and max.
  *
  * Return REDIS_OK on success. On error REDIS_ERR is returned.
  * When OK is returned the structure must be freed with zslFreeLexRange(),
  * otherwise no release is needed. */
  static int zslParseLexRange(robj *min, robj *max, zlexrangespec *spec)
  {
    /* The range can't be valid if objects are integer encoded.
    * Every item must start with ( or [. */
    if (min->encoding == REDIS_ENCODING_INT ||
        max->encoding == REDIS_ENCODING_INT)
      return REDIS_ERR;

    spec->min = spec->max = NULL;
    if (zslParseLexRangeItem(min, &spec->min, &spec->minex) == REDIS_ERR ||
        zslParseLexRangeItem(max, &spec->max, &spec->maxex) == REDIS_ERR)
    {
      if (spec->min)
        decrRefCount(spec->min);
      if (spec->max)
        decrRefCount(spec->max);
      return REDIS_ERR;
    }
    else
    {
      return REDIS_OK;
    }
  }

  /* Free a lex range structure, must be called only after zelParseLexRange()
  * populated the structure with success (REDIS_OK returned). */
  void zslFreeLexRange(zlexrangespec *spec)
  {
    decrRefCount(spec->min);
    decrRefCount(spec->max);
  }

  /* This is just a wrapper to compareStringObjects() that is able to
  * handle shared.minstring and shared.maxstring as the equivalent of
  * -inf and +inf for strings */
  int compareStringObjectsForLexRange(robj *a, robj *b)
  {
    if (a == b)
      return 0; /* This makes sure that we handle inf,inf and
                          -inf,-inf ASAP. One special case less. */
    if (a == shared.minstring || b == shared.maxstring)
      return -1;
    if (a == shared.maxstring || b == shared.minstring)
      return 1;
    return compareStringObjects(a, b);
  }

  static int zslLexValueGteMin(robj *value, zlexrangespec *spec)
  {
    return spec->minex ? (compareStringObjectsForLexRange(value, spec->min) > 0) : (compareStringObjectsForLexRange(value, spec->min) >= 0);
  }

  static int zslLexValueLteMax(robj *value, zlexrangespec *spec)
  {
    return spec->maxex ? (compareStringObjectsForLexRange(value, spec->max) < 0) : (compareStringObjectsForLexRange(value, spec->max) <= 0);
  }

  /* Returns if there is a part of the zset is in the lex range. */
  int zslIsInLexRange(zskiplist *zsl, zlexrangespec *range)
  {
    zskiplistNode *x;

    /* Test for ranges that will always be empty. */
    if (compareStringObjectsForLexRange(range->min, range->max) > 1 ||
        (compareStringObjects(range->min, range->max) == 0 &&
        (range->minex || range->maxex)))
      return 0;
    x = zsl->tail;
    if (x == NULL || !zslLexValueGteMin(x->obj, range))
      return 0;
    x = zsl->header->level[0].forward;
    if (x == NULL || !zslLexValueLteMax(x->obj, range))
      return 0;
    return 1;
  }

  /* Find the first node that is contained in the specified lex range.
  * Returns NULL when no element is contained in the range. */
  zskiplistNode *zslFirstInLexRange(zskiplist *zsl, zlexrangespec *range)
  {
    zskiplistNode *x;
    int i;

    /* If everything is out of range, return early. */
    if (!zslIsInLexRange(zsl, range))
      return NULL;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      /* Go forward while *OUT* of range. */
      while (x->level[i].forward &&
            !zslLexValueGteMin(x->level[i].forward->obj, range))
        x = x->level[i].forward;
    }

    /* This is an inner range, so the next node cannot be NULL. */
    x = x->level[0].forward;
    redisAssert(x != NULL);

    /* Check if score <= max. */
    if (!zslLexValueLteMax(x->obj, range))
      return NULL;
    return x;
  }

  /* Find the last node that is contained in the specified range.
  * Returns NULL when no element is contained in the range. */
  zskiplistNode *zslLastInLexRange(zskiplist *zsl, zlexrangespec *range)
  {
    zskiplistNode *x;
    int i;

    /* If everything is out of range, return early. */
    if (!zslIsInLexRange(zsl, range))
      return NULL;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
      /* Go forward while *IN* range. */
      while (x->level[i].forward &&
            zslLexValueLteMax(x->level[i].forward->obj, range))
        x = x->level[i].forward;
    }

    /* This is an inner range, so this node cannot be NULL. */
    redisAssert(x != NULL);

    /* Check if score >= min. */
    if (!zslLexValueGteMin(x->obj, range))
      return NULL;
    return x;
  }
  ```
  </details>

### 整数集合

  ![整数集合](https://github.com/gongluck/images/blob/main/redis/整数集合.png)

  - 整数集合底层是数组，这个数组以有序、无重复的方式保存集合元素
  - 在插入元素时，如果新插入元素的类型比原集合中的元素都大，会对整数集合进行升级
  - 整数集合只会进行升级，不会进行降级

  [intset.h](https://github.com/gongluck/sourcecode/blob/main/redis/src/intset.h)

  <details>
  <summary>整数集合</summary>

  ```c++
  //整数集合
  typedef struct intset
  {
    uint32_t encoding; //编码(小端字节序)
    uint32_t length;   //元素个数(小端字节序)
    int8_t contents[]; //元素数组
  } intset;
  ```
  </details>

  [intset.c](https://github.com/gongluck/sourcecode/blob/main/redis/src/intset.c)

  <details>
  <summary>整数集合</summary>

  ```c++
  /* Note that these encodings are ordered, so:
  * INTSET_ENC_INT16 < INTSET_ENC_INT32 < INTSET_ENC_INT64. */
  #define INTSET_ENC_INT16 (sizeof(int16_t))
  #define INTSET_ENC_INT32 (sizeof(int32_t))
  #define INTSET_ENC_INT64 (sizeof(int64_t))

  //返回该值应该使用的编码
  /* Return the required encoding for the provided value. */
  static uint8_t _intsetValueEncoding(int64_t v)
  {
    if (v < INT32_MIN || v > INT32_MAX)
      return INTSET_ENC_INT64;
    else if (v < INT16_MIN || v > INT16_MAX)
      return INTSET_ENC_INT32;
    else
      return INTSET_ENC_INT16;
  }

  //按编码参数返回索引处的元素
  /* Return the value at pos, given an encoding. */
  static int64_t _intsetGetEncoded(intset *is, int pos, uint8_t enc)
  {
    int64_t v64;
    int32_t v32;
    int16_t v16;

    if (enc == INTSET_ENC_INT64)
    {
      memcpy(&v64, ((int64_t *)is->contents) + pos, sizeof(v64));
      //对应set时做了一次转换，将数据转换回来。主要考虑分机情况，服务侧用小端字节序存储，客户侧按本机的字节序判断是否将小端字节序转换成大端字节序
      memrev64ifbe(&v64);
      return v64;
    }
    else if (enc == INTSET_ENC_INT32)
    {
      memcpy(&v32, ((int32_t *)is->contents) + pos, sizeof(v32));
      memrev32ifbe(&v32);
      return v32;
    }
    else
    {
      memcpy(&v16, ((int16_t *)is->contents) + pos, sizeof(v16));
      memrev16ifbe(&v16);
      return v16;
    }
  }

  //按设置的编码返回索引处的元素
  /* Return the value at pos, using the configured encoding. */
  static int64_t _intsetGet(intset *is, int pos)
  {
    return _intsetGetEncoded(is, pos, intrev32ifbe(is->encoding));
  }

  //插入值
  /* Set the value at pos, using the configured encoding. */
  static void _intsetSet(intset *is, int pos, int64_t value)
  {
    uint32_t encoding = intrev32ifbe(is->encoding);

    if (encoding == INTSET_ENC_INT64)
    {
      ((int64_t *)is->contents)[pos] = value;
      //按小端字节序存储
      memrev64ifbe(((int64_t *)is->contents) + pos);
    }
    else if (encoding == INTSET_ENC_INT32)
    {
      ((int32_t *)is->contents)[pos] = value;
      memrev32ifbe(((int32_t *)is->contents) + pos);
    }
    else
    {
      ((int16_t *)is->contents)[pos] = value;
      memrev16ifbe(((int16_t *)is->contents) + pos);
    }
  }

  //创建空整数集合
  /* Create an empty intset. */
  intset *intsetNew(void)
  {
    intset *is = zmalloc(sizeof(intset));
    //默认INTSET_ENC_INT16编码
    is->encoding = intrev32ifbe(INTSET_ENC_INT16);
    is->length = 0;
    return is;
  }

  //调整整数集合长度
  /* Resize the intset */
  static intset *intsetResize(intset *is, uint32_t len)
  {
    //计算长度
    uint32_t size = len * intrev32ifbe(is->encoding);
    //调整长度
    is = zrealloc(is, sizeof(intset) + size);
    return is;
  }

  //查找value的位置
  /* Search for the position of "value". Return 1 when the value was found and
  * sets "pos" to the position of the value within the intset. Return 0 when
  * the value is not present in the intset and sets "pos" to the position
  * where "value" can be inserted. */
  static uint8_t intsetSearch(intset *is, int64_t value, uint32_t *pos)
  {
    int min = 0, max = intrev32ifbe(is->length) - 1, mid = -1;
    int64_t cur = -1;

    /* The value can never be found when the set is empty */
    if (intrev32ifbe(is->length) == 0) //空集合
    {
      if (pos)
        *pos = 0;
      return 0;
    }
    else
    {
      /* Check for the case where we know we cannot find the value,
      * but do know the insert position. */
      if (value > _intsetGet(is, intrev32ifbe(is->length) - 1)) //超过最大值
      {
        if (pos)
          *pos = intrev32ifbe(is->length);
        return 0;
      }
      else if (value < _intsetGet(is, 0)) //小于最小值
      {
        if (pos)
          *pos = 0;
        return 0;
      }
    }

    //二分查找
    while (max >= min)
    {
      mid = ((unsigned int)min + (unsigned int)max) >> 1;
      cur = _intsetGet(is, mid);
      if (value > cur)
      {
        min = mid + 1;
      }
      else if (value < cur)
      {
        max = mid - 1;
      }
      else
      {
        break;
      }
    }

    if (value == cur) //找到了
    {
      if (pos)
        *pos = mid;
      return 1;
    }
    else //找不到
    {
      if (pos)
        *pos = min;
      return 0;
    }
  }

  //升级并插入
  /* Upgrades the intset to a larger encoding and inserts the given integer. */
  static intset *intsetUpgradeAndAdd(intset *is, int64_t value)
  {
    //旧编码
    uint8_t curenc = intrev32ifbe(is->encoding);
    //新编码
    uint8_t newenc = _intsetValueEncoding(value);
    //元素个数
    int length = intrev32ifbe(is->length);
    /*
        根据value的值，决定是将它添加到底层数组的最前端还是最后端
        因为value的编码比集合原有的其他元素的编码都要大
        所以value要么大于集合中的所有元素，要么小于集合中的所有元素
        因此，value只能添加到底层数组的最前端或最后端
    */
    int prepend = value < 0 ? 1 : 0;

    //调整缓冲区大小
    /* First set new encoding and resize */
    is->encoding = intrev32ifbe(newenc);
    //整数集合只有升级扩容
    is = intsetResize(is, intrev32ifbe(is->length) + 1); //+1是给新插入的元素

    //从后往前拷贝
    /* Upgrade back-to-front so we don't overwrite values.
    * Note that the "prepend" variable is used to make sure we have an empty
    * space at either the beginning or the end of the intset. */
    while (length--)
      _intsetSet(is, length + prepend, _intsetGetEncoded(is, length, curenc));

    //根据value的值，决定是将它添加到底层数组的最前端还是最后端
    /* Set the value at the beginning or the end. */
    if (prepend)
      _intsetSet(is, 0, value);
    else
      _intsetSet(is, intrev32ifbe(is->length), value);
    is->length = intrev32ifbe(intrev32ifbe(is->length) + 1);
    return is;
  }

  //移动指定索引范围内的数组元素
  static void intsetMoveTail(intset *is, uint32_t from, uint32_t to)
  {
    void *src, *dst;
    //移动长度
    uint32_t bytes = intrev32ifbe(is->length) - from;
    uint32_t encoding = intrev32ifbe(is->encoding);

    if (encoding == INTSET_ENC_INT64)
    {
      src = (int64_t *)is->contents + from;
      dst = (int64_t *)is->contents + to;
      bytes *= sizeof(int64_t);
    }
    else if (encoding == INTSET_ENC_INT32)
    {
      src = (int32_t *)is->contents + from;
      dst = (int32_t *)is->contents + to;
      bytes *= sizeof(int32_t);
    }
    else
    {
      src = (int16_t *)is->contents + from;
      dst = (int16_t *)is->contents + to;
      bytes *= sizeof(int16_t);
    }
    memmove(dst, src, bytes);
  }

  //插入一个值
  /* Insert an integer in the intset */
  intset *intsetAdd(intset *is, int64_t value, uint8_t *success)
  {
    //计算编码
    uint8_t valenc = _intsetValueEncoding(value);
    uint32_t pos;
    if (success)
      *success = 1;

    /* Upgrade encoding if necessary. If we need to upgrade, we know that
    * this value should be either appended (if > 0) or prepended (if < 0),
    * because it lies outside the range of existing values. */
    if (valenc > intrev32ifbe(is->encoding)) //需要升级插入
    {
      /* This always succeeds, so we don't need to curry *success. */
      return intsetUpgradeAndAdd(is, value);
    }
    else
    {
      //查找是否已经存在
      /* Abort if the value is already present in the set.
      * This call will populate "pos" with the right position to insert
      * the value when it cannot be found. */
      if (intsetSearch(is, value, &pos)) //已经存在，不插入
      {
        if (success)
          *success = 0;
        return is;
      }

      //调整大小+1
      is = intsetResize(is, intrev32ifbe(is->length) + 1);
      if (pos < intrev32ifbe(is->length)) //需要插入到最后一个元素前
        intsetMoveTail(is, pos, pos + 1); //后面部分元素后移
    }

    //插入
    _intsetSet(is, pos, value);
    is->length = intrev32ifbe(intrev32ifbe(is->length) + 1);
    return is;
  }

  //删除一个值
  /* Delete integer from intset */
  intset *intsetRemove(intset *is, int64_t value, int *success)
  {
    uint8_t valenc = _intsetValueEncoding(value);
    uint32_t pos;
    if (success)
      *success = 0;

    //编码没溢出并且查找到在集合中
    if (valenc <= intrev32ifbe(is->encoding) && intsetSearch(is, value, &pos))
    {
      uint32_t len = intrev32ifbe(is->length);

      /* We know we can delete */
      if (success)
        *success = 1;

      //如果不是最后一个
      /* Overwrite value with tail and update length */
      if (pos < (len - 1))
        intsetMoveTail(is, pos + 1, pos); //元素前移
      //调整大小
      is = intsetResize(is, len - 1);
      is->length = intrev32ifbe(len - 1);
    }
    return is;
  }

  //判断值是否存在
  /* Determine whether a value belongs to this set */
  uint8_t intsetFind(intset *is, int64_t value)
  {
    uint8_t valenc = _intsetValueEncoding(value);
    //编码不溢出并且查找到结果
    return valenc <= intrev32ifbe(is->encoding) && intsetSearch(is, value, NULL);
  }

  //随机获取一个值
  /* Return random member */
  int64_t intsetRandom(intset *is)
  {
    return _intsetGet(is, rand() % intrev32ifbe(is->length));
  }

  //获取指定位置的值
  /* Sets the value to the value at the given position. When this position is
  * out of range the function returns 0, when in range it returns 1. */
  uint8_t intsetGet(intset *is, uint32_t pos, int64_t *value)
  {
    if (pos < intrev32ifbe(is->length))
    {
      *value = _intsetGet(is, pos);
      return 1;
    }
    return 0;
  }

  //获取整数集合的长度
  /* Return intset length */
  uint32_t intsetLen(intset *is)
  {
    return intrev32ifbe(is->length);
  }

  //获取整个整数集合的内存大小
  /* Return intset blob size in bytes. */
  size_t intsetBlobLen(intset *is)
  {
    return sizeof(intset) + intrev32ifbe(is->length) * intrev32ifbe(is->encoding);
  }
  ```
  </details>