# ELF

- [ELF](#elf)
  - [编译链接过程](#编译链接过程)
  - [ELF文件格式](#elf文件格式)
  - [ELF文件类型](#elf文件类型)
  - [ELF文件头](#elf文件头)
  - [ELF程序头](#elf程序头)
  - [ELF节头](#elf节头)
  - [ELF节](#elf节)
    - [.text节](#text节)
    - [.rodata节](#rodata节)
    - [.data节](#data节)
    - [.bss节](#bss节)
    - [.got(全局偏移表) .plt节(过程链接表)](#got全局偏移表-plt节过程链接表)
    - [.shstrtab节头字符串表](#shstrtab节头字符串表)
    - [.symtab节(符号表)](#symtab节符号表)
    - [.strtab节(字符串表)](#strtab节字符串表)
    - [.dynsym节(动态链接符号表)](#dynsym节动态链接符号表)
    - [.dynstr节(动态链接字符串表)](#dynstr节动态链接字符串表)
    - [.rel.\*节(重定位表)](#rel节重定位表)
    - [.hash节](#hash节)
    - [.ctors节和.dtors节](#ctors节和dtors节)
  - [符号表节点](#符号表节点)
  - [ELF程序自修改](#elf程序自修改)
    - [修改全局/静态变量初始值](#修改全局静态变量初始值)

ELF文件(`Executable Linkable Format`)是一种文件存储格式。Linux下的目标文件和可执行文件都按照该格式进行存储。

## 编译链接过程

![编译链接过程](https://github.com/gongluck/images/blob/main/cpp/编译链接过程.png)

- 在经过汇编器后会输出一个.o文件，这个叫做可重定位的目标文件。
- 将main.o和sum.o输入链接器后，链接器输出的prog文件叫做可执行目标文件。

## ELF文件格式

![ELF文件格式](https://github.com/gongluck/images/blob/main/cpp/ELF文件格式.png)
![不同类型的ELF文件](https://github.com/gongluck/images/blob/main/cpp/不同类型的ELF文件.png)

- 段(segment)是程序执行的必要组成，当多个目标文件链接成一个可执行文件时，会将相同权限的节(section)合并到一个段中。
- 相比而言，节的粒度更小。
- 在全局变量或函数之前加上`__attribute__((section("name")))`属性就可以把相应的变量或函数放到以"name"作为段名的段中。
- 在全局变量或函数定义时加上`__attribute__("weak")`或声明时加上`__attribute__("weakref")`属性就可以把相应的变量或函数变成弱符号或弱引用。

## ELF文件类型

- 可重定位目标文件(`.o`或`.a`文件)
  - 包含二进制代码和数据，包含基础代码和数据，但它的代码及数据都没有指定绝对地址，因此它适合于与其他目标文件链接来创建可执行文件或者共享目标文件。
- 可执行目标文件(`a.out`文件)
  - 包含二进制代码和数据，其代码和数据都有固定的地址(或相对于基地址的偏移)，系统可根据这些地址信息把程序加载到内存执行。
- 共享对象文件(`.so`文件)
  - 包含二进制代码和数据，这些数据是在链接时被链接器(`ld`)和运行时动态链接器(`ld.so.l、libc.so.l、ld-linux.so.l`)使用的。
- 如果用于编译和链接(可重定位文件)，则编译器和链接器将把ELF文件看作是节头表描述的节的集合，程序头表可选。
- 如果用于加载执行(可执行文件)，则加载器则将把ELF文件看作是程序头表描述的段的集合，一个段可能包含多个节，节头表可选。

## ELF文件头

```Shell
readelf -h elf-file
```

<details>
<summary>ELF文件头</summary>

```C++
/* The ELF file header.  This appears at the start of every ELF file.  */

#define EI_NIDENT (16)

typedef struct
{
  unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */ //文件的标识以及标识描述了elf如何编码等信息
  Elf32_Half e_type; /* Object file type */                           //文件类型
  Elf32_Half e_machine; /* Architecture */                            //处理器架构
  Elf32_Word e_version; /* Object file version */                     //当前文件版本信息
  Elf32_Addr e_entry; /* Entry point virtual address */               //可执行程序的入口地址
  Elf32_Off e_phoff; /* Program header table file offset */           //程序表头偏移
  Elf32_Off e_shoff; /* Section header table file offset */           //节头表偏移
  Elf32_Word e_flags; /* Processor-specific flags */                  //特定处理器标识
  Elf32_Half e_ehsize; /* ELF header size in bytes */                 // elf文件头部大小
  Elf32_Half e_phentsize; /* Program header table entry size */       //程序头部表项大小
  Elf32_Half e_phnum; /* Program header table entry count */          //程序头表项个数
  Elf32_Half e_shentsize; /* Section header table entry size */       //节头表项大小
  Elf32_Half e_shnum; /* Section header table entry count */          //节头表项个数
  Elf32_Half e_shstrndx; /* Section header string table index */      //字符串节区的索引
} Elf32_Ehdr;
```
</details>

## ELF程序头

<details>
<summary>ELF程序头</summary>

```C++
/* Program segment header.  */

typedef struct
{
  Elf32_Word p_type; /* Segment type */              //段类型
  Elf32_Off p_offset; /* Segment file offset */      //本段的第一个字节从文件开始位置处的偏移量
  Elf32_Addr p_vaddr; /* Segment virtual address */  //本段的第一个字节在内存中的虚拟地址
  Elf32_Addr p_paddr; /* Segment physical address */ //在物理地址是相对寻址的系统上,这个成员保留用作段的物理地址
  Elf32_Word p_filesz; /* Segment size in file */    //本段在文件镜像中的字节大小
  Elf32_Word p_memsz; /* Segment size in memory */   //本段在内存镜像中的字节大小
  Elf32_Word p_flags; /* Segment flags */            //段相关的标记
  Elf32_Word p_align; /* Segment alignment */        //对齐方式
} Elf32_Phdr;
```
</details>

## ELF节头

```Shell
readelf -S elf-file
```

- 从`e_shoff`的地址开始偏移`e_shstrndx`+1个`e_shentsize`，得到字符串(`.strtab`)节区。

<details>
<summary>ELF节头</summary>

```C++
/* Section header.  */

typedef struct
{
  Elf32_Word sh_name; /* Section name (string tbl index) */      //节名字符串在.strtab节(字符串表)中的偏移
  Elf32_Word sh_type; /* Section type */                         //节类型
  Elf32_Word sh_flags; /* Section flags */                       //节标记
  Elf32_Addr sh_addr; /* Section virtual addr at execution */    //加载后程序段的虚拟地址
  Elf32_Off sh_offset; /* Section file offset */                 //节在文件中的偏移
  Elf32_Word sh_size; /* Section size in bytes */                //节长度
  Elf32_Word sh_link; /* Link to another section */              //链接相关标记
  Elf32_Word sh_info; /* Additional section information */       //其它标记
  Elf32_Word sh_addralign; /* Section alignment */               //节对齐
  Elf32_Word sh_entsize; /* Entry size if section holds table */ //每项固定的大小
} Elf32_Shdr;
```
</details>

## ELF节

### .text节

- `.text`节是保存了程序代码指令的代码节。
- 一段可执行程序，如果存在`Phdr`，则.text节就会存在于text段中。
- 由于.text节保存了程序代码，所以节类型为`SHT_PROGBITS`。

### .rodata节

- `.rodata`节保存了只读的数据，如一行C语言代码中的字符串。
- 由于.rodata节是只读的，所以只能存在于一个可执行文件的只读段中。只能在text段中找到.rodata节。
- 由于.rodata节是只读的，所以节类型为`SHT_PROGBITS`。

### .data节

- `.data`节存在于data段中，其保存了初始化的全局变量等数据。
- 由于.data节保存了程序的变量数据，所以节类型为`SHT_PROGBITS`。

### .bss节

- `.bss`节存在于data段中，占用空间不超过`4`字节，仅表示这个节本生的空间。
- .bss节保存了未进行初始化的全局数据。程序加载时数据被初始化为`0`，在程序执行期间可以进行赋值。
- 由于.bss节未保存实际的数据，所以节类型为`SHT_NOBITS`。

### .got(全局偏移表) .plt节(过程链接表)

- `.got`节和`.plt`节一起提供了对导入的共享库函数的访问入口，由动态链接器在运行时进行修改。
- 由于.got和.plt节与程序执行有关，所以节类型为`SHT_PROGBITS`。

### .shstrtab节头字符串表

- `.shstrtab`节头字符串表，用于保存节头表中用到的字符串，可通过`sh_name`进行索引。

### .symtab节(符号表)

- `.symtab`节是一个`ElfN_Sym`的数组，保存了符号信息。节类型为`SHT_SYMTAB`。
- .symtab中保存了可执行文件的本地符号和从共享库导入的动态符号。
- .symtab只是用来进行调试和链接的。

### .strtab节(字符串表)

- `.strtab`节保存的是符号字符串表，表中的内容会被.symtab的ElfN_Sym结构中的st_name引用。
- .strtab节类型为`SHT_STRTAB`。

### .dynsym节(动态链接符号表)

- `.dynsym`节保存在text段中。其保存了从共享库导入的动态符号表。节类型为`SHT_DYNSYM`。
- .dynsym保存了引用来自外部文件符号的全局符号。.dynsym保存的符号是.symtab所保存符合的子集。
- .dynsym保存的符号只能在运行时被解析，因此是运行时动态链接器所需的唯一符号。
- .dynsym对于动态链接可执行文件的执行是必需的。

### .dynstr节(动态链接字符串表)

- `.dynstr`保存了动态链接字符串表，表中存放了一系列字符串，这些字符串代表了符号名称，以空字符作为终止符。

### .rel.*节(重定位表)

- 重定位表保存了重定位相关的信息，这些信息描述了如何在链接或运行时，对ELF目标文件的某部分或者进程镜像进行补充或修改。
- 由于重定位表保存了重定位相关的数据，所以节类型为`SHT_REL`。

### .hash节

- `.hash`节也称为`.gnu.hash`，其保存了一个用于查找符号的散列表。

### .ctors节和.dtors节

- `.ctors`(构造器)节和`.dtors`(析构器)节分别保存了指向构造函数和析构函数的函数指针，构造函数是在main函数执行之前需要执行的代码；析构函数是在main函数之后需要执行的代码。

## 符号表节点

```Shell
readelf -s elf-file
```

<details>
<summary>符号表节点</summary>

```C++
/* Symbol table entry.  */

typedef struct
{
  Elf32_Word st_name; /* Symbol name (string tbl index) */ //符号名 该值为该符号名在字符串表中的偏移地址
  Elf32_Addr st_value; /* Symbol value */                  //符号对应的值 存放符号的值(可能是地址或位置偏移量)
  Elf32_Word st_size; /* Symbol size */                    //符号的大小
  unsigned char st_info; /* Symbol type and binding */     //符号类型及绑定属性
  unsigned char st_other; /* Symbol visibility */          //符号可见性
  Elf32_Section st_shndx; /* Section index */              //符号所在的节
} Elf32_Sym;
```
</details>

## ELF程序自修改

### [修改全局/静态变量初始值](../code/elf/global.cpp)

<details>
<summary>修改全局/静态变量初始值</summary>

```C++
/*
 * @Author: gongluck
 * @Date: 2022-04-14 10:49:56
 * @Last Modified by: gongluck
 * @Last Modified time: 2022-04-14 11:34:55
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
