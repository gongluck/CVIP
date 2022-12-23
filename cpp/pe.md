# PE

- [PE](#pe)
  - [PE段属性](#pe段属性)
  - [PE段](#pe段)
    - [.drectve](#drectve)
    - [.debug$\*](#debug)

PE文件(`Portable Executable`)是一种文件存储格式。Windows下的目标文件和可执行文件都按照该格式进行存储。

## PE段属性

- `#pragma data_seg("name")`表示把语句之后的变量或函数放到指定的段。

## PE段

### .drectve

- 编译器传递给链接器的指令，即编译器希望告诉链接器应该怎样链接这个目标文件。

### .debug$*

- 调试信息段。