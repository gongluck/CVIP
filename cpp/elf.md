# ELF

- [ELF](#elf)
  - [编译链接过程](#编译链接过程)
  - [ELF 段(segment)和节(section)](#elf-段segment和节section)
  - [ELF File Header](#elf-file-header)
  - [ELF Program Header](#elf-program-header)
  - [ELF Section Header](#elf-section-header)
  - [ELF Section](#elf-section)
    - [.interp](#interp)
    - [.text](#text)
    - [.rodata](#rodata)
    - [.data](#data)
    - [.bss](#bss)
    - [.got/.plt/.got.plt](#gotpltgotplt)
    - [.shstrtab](#shstrtab)
    - [.symtab](#symtab)
    - [.strtab](#strtab)
    - [.dynsym](#dynsym)
    - [.dynstr](#dynstr)
    - [.dynamic](#dynamic)
    - [.hash](#hash)
    - [.rel.\*](#rel)
    - [.ctors/.dtors](#ctorsdtors)
    - [.init/.finit](#initfinit)
  - [ELF 程序自修改](#elf-程序自修改)
    - [修改全局/静态变量初始值](#修改全局静态变量初始值)

ELF 文件(`Executable Linkable Format`)是一种文件存储格式。
Linux 下的目标文件和可执行文件都按照该格式进行存储。

## 编译链接过程

![编译链接过程](https://github.com/gongluck/images/blob/main/cpp/编译链接过程.png)

## ELF 段(segment)和节(section)

![ELF文件格式](https://github.com/gongluck/images/blob/main/cpp/ELF文件格式.png)
![不同类型的ELF文件](https://github.com/gongluck/images/blob/main/cpp/不同类型的ELF文件.png)

- 段(segment)是程序执行的必要组成，**当多个目标文件链接成一个可执行文件时，会将相同权限的节(section)合并到一个段中。**
- 在全局变量或函数之前加上`__attribute__((section("name")))`属性就可以把相应的变量或函数放到以"name"作为段名的段中。
- 在全局变量或函数定义时加上`__attribute__((weak))`或声明时加上`__attribute__((weakref))`属性就可以把相应的变量或函数变成弱符号或弱引用。
- **如果用于编译和链接(可重定位文件)，则编译器和链接器将把 ELF 文件看作是节头表描述的节的集合，程序头表可选。**
- **如果用于加载执行(可执行文件)，则加载器则将把 ELF 文件看作是程序头表描述的段的集合，一个段可能包含多个节，节头表可选。**

## [ELF File Header](https://github.com/gongluck/sourcecode/blob/main/linux-3.10/include/uapi/linux/elf.h#L210)

![ELF文件头](https://github.com/gongluck/images/blob/main/elf/elf_header.png)
![e_ident](https://github.com/gongluck/images/blob/main/elf/e_ident.png)

<details>
<summary>ELF File Header</summary>

```C++
#define EI_NIDENT 16

typedef struct elf32_hdr // elf文件头
{
  unsigned char e_ident[EI_NIDENT];     // elf文件标识
  Elf32_Half e_type;                    // elf文件类型
  Elf32_Half e_machine;                 // elf文件机器架构
  Elf32_Word e_version;                 // elf文件版本号
  Elf32_Addr e_entry; /* Entry point */ // elf执行入口点
  Elf32_Off e_phoff;                    // program header table的偏移
  Elf32_Off e_shoff;                    // section header table的偏移
  Elf32_Word e_flags;                   // 特定于处理器的标志
  Elf32_Half e_ehsize;                  // ELF文件头的大小，32位ELF是52字节，64位是64字节
  Elf32_Half e_phentsize;               // program header table中每个入口的大小
  Elf32_Half e_phnum;                   // program header table的入口个数
  Elf32_Half e_shentsize;               // section header table中每个入口的大小
  Elf32_Half e_shnum;                   // section header table的入口个数
  Elf32_Half e_shstrndx;                // section header table中字符串段(.shstrtab)的索引
} Elf32_Ehdr;

typedef struct elf64_hdr
{
  unsigned char e_ident[EI_NIDENT]; /* ELF "magic number" */
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry; /* Entry point virtual address */
  Elf64_Off e_phoff;  /* Program header table file offset */
  Elf64_Off e_shoff;  /* Section header table file offset */
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} Elf64_Ehdr;
```

</details>

## [ELF Program Header](https://github.com/gongluck/sourcecode/blob/main/linux-3.10/include/uapi/linux/elf.h#L247)

<details>
<summary>ELF Program Header</summary>

```C++
// 程序头权限属性
/* These constants define the permissions on sections in the program
   header, p_flags. */
#define PF_R 0x4 // 可读
#define PF_W 0x2 // 可写
#define PF_X 0x1 // 可运行

typedef struct elf32_phdr // elf程序头表 segment
{
  Elf32_Word p_type;   // 段类型 PT_XXX
  Elf32_Off p_offset;  // 段在内存映像(虚拟内存基址)的偏移
  Elf32_Addr p_vaddr;  // 虚拟地址空间
  Elf32_Addr p_paddr;  // 物理装载地址
  Elf32_Word p_filesz; // 段文件长度
  Elf32_Word p_memsz;  // 段虚拟空间长度
  Elf32_Word p_flags;  // 权限属性
  Elf32_Word p_align;  // 对齐幂数
} Elf32_Phdr;

typedef struct elf64_phdr
{
  Elf64_Word p_type;
  Elf64_Word p_flags;
  Elf64_Off p_offset;   /* Segment file offset */
  Elf64_Addr p_vaddr;   /* Segment virtual address */
  Elf64_Addr p_paddr;   /* Segment physical address */
  Elf64_Xword p_filesz; /* Segment size in file */
  Elf64_Xword p_memsz;  /* Segment size in memory */
  Elf64_Xword p_align;  /* Segment alignment, file & memory */
} Elf64_Phdr;
```

</details>

## [ELF Section Header](https://github.com/gongluck/sourcecode/blob/main/linux-3.10/include/uapi/linux/elf.h#L312)

<details>
<summary>ELF Section Header</summary>

```C++
typedef struct elf32_shdr // elf段表描述结构
{
  Elf32_Word sh_name;      //.shstrtab中的索引
  Elf32_Word sh_type;      // 段类型
  Elf32_Word sh_flags;     // 段标志
  Elf32_Addr sh_addr;      // 段虚拟地址
  Elf32_Off sh_offset;     // 段在文件中的偏移
  Elf32_Word sh_size;      // 段大小
  Elf32_Word sh_link;      // 段使用的字符串表或符号表在段表中的索引
  Elf32_Word sh_info;      // 重定位表所作用的段在段表中的索引
  Elf32_Word sh_addralign; // 段对齐 2的n次幂
  Elf32_Word sh_entsize;   // 段中每项大小(如果可用)
} Elf32_Shdr;

typedef struct elf64_shdr
{
  Elf64_Word sh_name;       /* Section name, index in string tbl */
  Elf64_Word sh_type;       /* Type of section */
  Elf64_Xword sh_flags;     /* Miscellaneous section attributes */
  Elf64_Addr sh_addr;       /* Section virtual addr at execution */
  Elf64_Off sh_offset;      /* Section file offset */
  Elf64_Xword sh_size;      /* Size of section in bytes */
  Elf64_Word sh_link;       /* Index of another section */
  Elf64_Word sh_info;       /* Additional section information */
  Elf64_Xword sh_addralign; /* Section alignment */
  Elf64_Xword sh_entsize;   /* Entry size if section holds table */
} Elf64_Shdr;
```

</details>

- 段名的字符串信息在`.strtab`段，需要从`e_shoff`的地址开始偏移`e_shstrndx`+1 个`e_shentsize`。

## ELF Section

### .interp

- 保存可执行文件需要的动态链接器的路径。

### .text

- 保存程序代码指令。

### .rodata

- 保存只读数据。

### .data

- 保存初始化的全局变量等数据。

### .bss

- 存在于`data`段中，占用空间不超过`4`字节，仅表示这个节本生的空间。
- 保存未进行初始化的全局数据。程序加载时数据被初始化为`0`，在程序执行期间可以进行赋值。

### .got/.plt/.got.plt

- GOT(Global Offset Table)全局偏移表。链接器为外部符号填充的实际偏移表。
- PLT(Procedure Linkage Table)程序链接表。可以在`.got.plt`节中拿到地址，并跳转。当`.got.plt`没有所需地址的时，触发链接器去找到所需地址。
- .got.plt 中的值是 GOT 的一部分。包含上述 PLT 表所需地址(已经找到的和需要去触发的)。

### .shstrtab

- 节头字符串表，用于保存节头表中用到的字符串，可通过`sh_name`进行索引。

### [.symtab](https://github.com/gongluck/sourcecode/blob/main/linux-3.10/include/uapi/linux/elf.h#L190)

<details>
<summary>符号表段</summary>

```C++
typedef struct elf32_sym // 符号表结构
{
  Elf32_Word st_name;     // 字符串表中的索引
  Elf32_Addr st_value;    // 符号值 绝对值或在段中偏移的地址值
  Elf32_Word st_size;     // 符号大小
  unsigned char st_info;  // 低4位标识符号类型 高4位标识绑定信息
  unsigned char st_other; // 0
  Elf32_Half st_shndx;    // 符号所在的段
} Elf32_Sym;

typedef struct elf64_sym
{
  Elf64_Word st_name;     /* Symbol name, index in string tbl */
  unsigned char st_info;  /* Type and binding attributes */
  unsigned char st_other; /* No defined meaning, 0 */
  Elf64_Half st_shndx;    /* Associated section index */
  Elf64_Addr st_value;    /* Value of the symbol */
  Elf64_Xword st_size;    /* Associated symbol size */
} Elf64_Sym;
```

</details>

- 保存了符号信息。
- 保存了可执行文件的本地符号和从共享库导入的动态符号。
- 只是用来进行调试和链接的。

### .strtab

- 保存符号字符串表，表中的内容会被`.symtab`的结构引用。

### .dynsym

- 保存了从共享库导入的动态符号表和全局符号，是`.symtab`所保存符合的子集。
- 保存的符号只能在运行时被解析，是运行时动态链接器所需的唯一符号。对于动态链接可执行文件的执行是必需的。

### .dynstr

- 保存动态链接字符串表，表中存放字符串代表符号名称，以空字符作为终止符。

### .dynamic

- 保存了动态链接所需要的基本信息，依赖于哪些共享对象、动态链接符号表的位置、动态链接重定位表的位置、共享对象初始化代码的地址等。

### .hash

- 也称为`.gnu.hash`，保存了一个用于查找符号的散列表。

### [.rel.\*](https://github.com/gongluck/sourcecode/blob/main/linux-3.10/include/uapi/linux/elf.h#L157)

<details>
<summary>重定位表</summary>

```C++
/* The following are used with relocations */ // 提取符号重定位信息
#define ELF32_R_SYM(x) ((x) >> 8)             // 提取符号重定位绑定信息
#define ELF32_R_TYPE(x) ((x)&0xff)            // 提取符号重定位类型

#define ELF64_R_SYM(i) ((i) >> 32)
#define ELF64_R_TYPE(i) ((i)&0xffffffff)

typedef struct elf32_rel // 重定位表入口结构
{
  Elf32_Addr r_offset; // 段偏移或虚拟地址
  Elf32_Word r_info;   // 低8位标识入口类型 高24位标识入口符号在符号表的下标
} Elf32_Rel;

typedef struct elf64_rel
{
  Elf64_Addr r_offset; /* Location at which to apply the action */
  Elf64_Xword r_info;  /* index and type of relocation */
} Elf64_Rel;

typedef struct elf32_rela // 重定位表入口结构
{
  Elf32_Addr r_offset;  // 段偏移或虚拟地址
  Elf32_Word r_info;    // 低8位标识入口类型 高24位标识入口符号在符号表的下标
  Elf32_Sword r_addend; // 辅助计算修订值 某些指令使用的是下一条指令的地址作为偏移寻址，则可以将这部分的偏移信息放在r_addend里面
} Elf32_Rela;

typedef struct elf64_rela
{
  Elf64_Addr r_offset;   /* Location at which to apply the action */
  Elf64_Xword r_info;    /* index and type of relocation */
  Elf64_Sxword r_addend; /* Constant addend used to compute value */
} Elf64_Rela;
```

</details>

- 保存了重定位相关的信息，描述了如何在链接或运行时，对 ELF 目标文件的某部分或者进程镜像进行补充或修改。

### .ctors/.dtors

- 构造器和析构器分别保存了指向构造函数和析构函数的函数指针，构造函数是在 main 函数执行之前需要执行的代码；析构函数是在 main 函数之后需要执行的代码。

### .init/.finit

- 支持全局和静态对象的构造和析构。
- 若共享对象有.init 段或.finit 段，那么动态链接器将会执行段中的代码，以实现共享对象特有的初始化和反初始化过程。

## ELF 程序自修改

### [修改全局/静态变量初始值](../code/elf/global.cpp)

<details>
<summary>修改全局/静态变量初始值</summary>

```C++
/*
 * @Author: gongluck
 * @Date: 2022-04-14 10:49:56
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-06 17:27:16
 */

// application rewrite it`s global variable via shell tools
#include <stdio.h>
#include <stdlib.h>
#define NAME2STR(name) (#name)
int GGG = 1;
int test()
{
  static int SSS = 100;
  printf("%d\n", SSS);
  return SSS;
}
int main(int argc, char *argv[])
{
  if (argc == 3)
  {
    int n = atoi(argv[2]);
    FILE *fp = fopen(argv[0], "r+b");
    fseek(fp, atoi(argv[1]), SEEK_SET);
    fwrite(&n, 4, 1, fp);
    fclose(fp);
  }
  else
  {
    printf("%s\n", argv[0]);
    printf("%d\n", GGG);
    test();
    srand(GGG);
    GGG = rand();
    char buf[1024] = {0};
    // readelf -s a.out | grep GGG
    // readelf -S a.out
    // hexdump a.out -C -s 0x3014 -n 4
    sprintf(buf, "%s $(expr `printf %%d 0x$(readelf -s %s | grep %s | awk '{print $2}')` - `printf %%d 0x$(readelf -S %s | grep \" .data \" | awk '{print $4}')` + `printf %%d 0x$(readelf -S %s | grep \" .data \" | awk '{print $5}')`) %d",
            argv[0], argv[0], NAME2STR(GGG), argv[0], argv[0], GGG);
    system(buf);
    srand(GGG);
    GGG = rand();
    // rewrite static variable
    sprintf(buf, "%s $(expr `printf %%d 0x$(readelf -s %s | grep %s | awk '{print $2}')` - `printf %%d 0x$(readelf -S %s | grep \" .data \" | awk '{print $4}')` + `printf %%d 0x$(readelf -S %s | grep \" .data \" | awk '{print $5}')`) %d",
            argv[0], argv[0], NAME2STR(SSS), argv[0], argv[0], GGG);
    system(buf);
  }
  return 0;
}
```

</details>
