# ```Redis```数据结构

- [```Redis```数据结构](#redis数据结构)
  - [```Redis```基础数据结构](#redis基础数据结构)
    - [简单动态字符串](#简单动态字符串)
    - [链表](#链表)

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