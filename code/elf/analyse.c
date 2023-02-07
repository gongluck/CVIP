/*
 * @Author: gongluck
 * @Date: 2023-01-05 10:48:45
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-02-07 23:21:12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

// helper
#define EXIT(code)                            \
  do                                          \
  {                                           \
    printf("exit int line : %d\n", __LINE__); \
    exit(code);                               \
  } while (0)

#define PRINT_SPLIT()            \
  do                             \
  {                              \
    for (int i = 0; i < 10; ++i) \
    {                            \
      printf("**********");      \
    }                            \
    printf("\n");                \
  } while (0)

#define PRINT_VALUE(value)                         \
  do                                               \
  {                                                \
    printf("%lld", (long long unsigned int)value); \
  } while (0)

#define PRINT_VALUE_HEX(value)                       \
  do                                                 \
  {                                                  \
    printf("0x%llx", (long long unsigned int)value); \
  } while (0)

#define PRINT_SYM_VALUE(sym)                     \
  do                                             \
  {                                              \
    printf("%lld", (long long unsigned int)sym); \
  } while (0)

#define PRINT_SYM_VALUE_HEX(sym) \
  do                             \
  {                              \
    PRINT_SYM_ADDR(sym);         \
  } while (0)

#define PRINT_SYM_ADDR(sym)                          \
  do                                                 \
  {                                                  \
    printf("0x%.2llx", (long long unsigned int)sym); \
  } while (0)

#define PRINT_STR(str) \
  do                   \
  {                    \
    printf(str);       \
  } while (0)

#define PRINT_SYM_STREND(sym, end) \
  do                               \
  {                                \
    printf(#sym);                  \
    printf(end);                   \
  } while (0)

#define PRINT_SYM_STR_BREAK(sym) \
  case sym:                      \
    PRINT_STR(#sym);             \
    break;

#define PRINT_SYM_STR_DEFAULT(sym) \
  default:                         \
    PRINT_VALUE_HEX(sym);          \
    break;

///////////////////////////////////////////////////////////////////////////////////////////////////

// e_ident[EI_CLASS] 4
#define PRINT_IDENT_CLASS(class)                          \
  do                                                      \
  {                                                       \
    switch (class)                                        \
    {                                                     \
      PRINT_SYM_STR_BREAK(ELFCLASS32); /*32-bit objects*/ \
      PRINT_SYM_STR_BREAK(ELFCLASS64); /*64-bit objects*/ \
      PRINT_SYM_STR_DEFAULT(class);                       \
    }                                                     \
  } while (0)

// e_ident[EI_DATA] 5
#define PRINT_IDENT_DATA(data)                            \
  do                                                      \
  {                                                       \
    switch (data)                                         \
    {                                                     \
      PRINT_SYM_STR_BREAK(ELFDATA2LSB); /*little endian*/ \
      PRINT_SYM_STR_BREAK(ELFDATA2MSB); /*big endian*/    \
      PRINT_SYM_STR_DEFAULT(data);                        \
    }                                                     \
  } while (0)

// e_ident[EI_VERSION] 6
#define PRINT_IDENT_VERSION(version)                       \
  do                                                       \
  {                                                        \
    switch (version)                                       \
    {                                                      \
      PRINT_SYM_STR_BREAK(EV_CURRENT); /*Current version*/ \
      PRINT_SYM_STR_DEFAULT(version);                      \
    }                                                      \
  } while (0)

// e_ident
#define PRINT_IDENT(ident)                  \
  do                                        \
  {                                         \
    printf("Magic:");                       \
    for (int i = 0; i < EI_NIDENT; ++i)     \
    {                                       \
      printf(" %.2x", ident[i]);            \
    }                                       \
    printf("\nClass: ");                    \
    PRINT_IDENT_CLASS(ident[EI_CLASS]);     \
    printf("\nData: ");                     \
    PRINT_IDENT_DATA(ident[EI_DATA]);       \
    printf("\nVersion: ");                  \
    PRINT_IDENT_VERSION(ident[EI_VERSION]); \
    printf("\n");                           \
  } while (0)

// e_type
#define PRINT_ELFHEADER_TYPE(type)                         \
  do                                                       \
  {                                                        \
    switch (type)                                          \
    {                                                      \
      PRINT_SYM_STR_BREAK(ET_REL);  /*Relocatable file*/   \
      PRINT_SYM_STR_BREAK(ET_EXEC); /*Executable file*/    \
      PRINT_SYM_STR_BREAK(ET_DYN);  /*Shared object file*/ \
      PRINT_SYM_STR_BREAK(ET_CORE); /*Core file*/          \
      PRINT_SYM_STR_DEFAULT(type);                         \
    }                                                      \
  } while (0)

// e_machine
#define PRINT_ELFHEADER_MACHINE(machine)                          \
  do                                                              \
  {                                                               \
    switch (machine)                                              \
    {                                                             \
      PRINT_SYM_STR_BREAK(EM_386);    /*Intel 80386*/             \
      PRINT_SYM_STR_BREAK(EM_X86_64); /*AMD x86-64 architecture*/ \
      PRINT_SYM_STR_DEFAULT(machine);                             \
    }                                                             \
  } while (0)

// ~elf header https://github.com/gongluck/CVIP/blob/master/cpp/elf.md#elf-file-header
#define PRINT_ELFHEADER(elfhdr)                                  \
  do                                                             \
  {                                                              \
    PRINT_SPLIT();                                               \
    PRINT_IDENT(elfhdr.e_ident);                                 \
    printf("Type: ");                                            \
    PRINT_ELFHEADER_TYPE(elfhdr.e_type);                         \
    printf("\nMachine: ");                                       \
    PRINT_ELFHEADER_MACHINE(elfhdr.e_machine);                   \
    printf("\nVersion: ");                                       \
    PRINT_SYM_VALUE_HEX(elfhdr.e_version);                       \
    printf("\nEntry: ");                                         \
    PRINT_SYM_ADDR(elfhdr.e_entry);                              \
    printf("\nProgram header offset: ");                         \
    PRINT_SYM_VALUE_HEX(elfhdr.e_phoff);                         \
    printf("(");                                                 \
    PRINT_SYM_VALUE(elfhdr.e_phoff);                             \
    printf(")");                                                 \
    printf("\nSection header offset: ");                         \
    PRINT_SYM_VALUE_HEX(elfhdr.e_shoff);                         \
    printf("(");                                                 \
    PRINT_SYM_VALUE(elfhdr.e_shoff);                             \
    printf(")");                                                 \
    printf("\nFlags: ");                                         \
    PRINT_SYM_ADDR(elfhdr.e_flags);                              \
    printf("\nHeader size: ");                                   \
    PRINT_SYM_VALUE(elfhdr.e_ehsize);                            \
    printf("\nProgram header entry size: ");                     \
    PRINT_SYM_VALUE(elfhdr.e_phentsize);                         \
    printf("\nProgram header entry counts: ");                   \
    PRINT_SYM_VALUE(elfhdr.e_phnum);                             \
    printf("\nSection header entry size: ");                     \
    PRINT_SYM_VALUE(elfhdr.e_shentsize);                         \
    printf("\nSection header entry counts: ");                   \
    PRINT_SYM_VALUE(elfhdr.e_shnum);                             \
    printf("\nSection header string table (.shstrtab) index: "); \
    PRINT_SYM_VALUE(elfhdr.e_shstrndx);                          \
    printf("\n\n");                                              \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PT_GNU_PROPERTY
#define PT_GNU_PROPERTY (PT_LOOS + 0x474e553) /*GNU property*/
#endif

// program header type
#define PRINT_PROGRAMHEADER_TYPE(type)                                        \
  do                                                                          \
  {                                                                           \
    switch (type)                                                             \
    {                                                                         \
      PRINT_SYM_STR_BREAK(PT_LOAD);         /*Loadable program segment*/      \
      PRINT_SYM_STR_BREAK(PT_DYNAMIC);      /*Dynamic linking information*/   \
      PRINT_SYM_STR_BREAK(PT_INTERP);       /*Program interpreter*/           \
      PRINT_SYM_STR_BREAK(PT_NOTE);         /*Auxiliary information*/         \
      PRINT_SYM_STR_BREAK(PT_SHLIB);        /*Reserved*/                      \
      PRINT_SYM_STR_BREAK(PT_PHDR);         /*Entry for header table itself*/ \
      PRINT_SYM_STR_BREAK(PT_TLS);          /*Thread-local storage segment*/  \
      PRINT_SYM_STR_BREAK(PT_GNU_EH_FRAME); /*GCC .eh_frame_hdr segment*/     \
      PRINT_SYM_STR_BREAK(PT_GNU_STACK);    /*Indicates stack executability*/ \
      PRINT_SYM_STR_BREAK(PT_GNU_RELRO);    /*Read-only after relocation*/    \
      PRINT_SYM_STR_BREAK(PT_GNU_PROPERTY); /*GNU property*/                  \
      PRINT_SYM_STR_DEFAULT(type);                                            \
    }                                                                         \
  } while (0)

// program header flags
#define PRINT_PROGRAMHEADER_FLAGS(flags)        \
  do                                            \
  {                                             \
    if (flags & PF_X) /*Segment is executable*/ \
    {                                           \
      PRINT_SYM_STREND(PF_X, " ");              \
    }                                           \
    if (flags & PF_W) /*Segment is writable*/   \
    {                                           \
      PRINT_SYM_STREND(PF_W, " ");              \
    }                                           \
    if (flags & PF_R) /*Segment is readable*/   \
    {                                           \
      PRINT_SYM_STREND(PF_R, " ");              \
    }                                           \
  } while (0)

// ~program header https://github.com/gongluck/CVIP/blob/master/cpp/elf.md#elf-program-header
#define PRINT_PROGRAMHEADER(elfphr)            \
  do                                           \
  {                                            \
    printf("\tType: ");                        \
    PRINT_PROGRAMHEADER_TYPE(elfphr.p_type);   \
    printf("\n\tOffset: ");                    \
    PRINT_SYM_VALUE_HEX(elfphr.p_offset);      \
    printf("(");                               \
    PRINT_SYM_VALUE(elfphr.p_offset);          \
    printf(")");                               \
    printf("\n\tVirtual Addr: ");              \
    PRINT_SYM_VALUE_HEX(elfphr.p_vaddr);       \
    printf("\n\tPhysical Addr: ");             \
    PRINT_SYM_VALUE_HEX(elfphr.p_paddr);       \
    printf("\n\tFile Size: ");                 \
    PRINT_SYM_VALUE(elfphr.p_filesz);          \
    printf("\n\tMemory Size: ");               \
    PRINT_SYM_VALUE(elfphr.p_memsz);           \
    printf("\n\tFlags: ");                     \
    PRINT_PROGRAMHEADER_FLAGS(elfphr.p_flags); \
    printf("\n\tAlign: ");                     \
    PRINT_SYM_VALUE(elfphr.p_align);           \
    printf("\n");                              \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

// section header type
#define PRINT_SECTIONHEADER_TYPE(type)                                             \
  do                                                                               \
  {                                                                                \
    switch (type)                                                                  \
    {                                                                              \
      PRINT_SYM_STR_BREAK(SHT_PROGBITS);      /*Program data*/                     \
      PRINT_SYM_STR_BREAK(SHT_SYMTAB);        /*Symbol table*/                     \
      PRINT_SYM_STR_BREAK(SHT_STRTAB);        /*String table*/                     \
      PRINT_SYM_STR_BREAK(SHT_RELA);          /*Relocation entries with addends*/  \
      PRINT_SYM_STR_BREAK(SHT_HASH);          /*Symbol hash table*/                \
      PRINT_SYM_STR_BREAK(SHT_DYNAMIC);       /*Dynamic linking information*/      \
      PRINT_SYM_STR_BREAK(SHT_NOTE);          /*Notes*/                            \
      PRINT_SYM_STR_BREAK(SHT_NOBITS);        /*Program space with no data (bss)*/ \
      PRINT_SYM_STR_BREAK(SHT_REL);           /*Relocation entries, no addends*/   \
      PRINT_SYM_STR_BREAK(SHT_SHLIB);         /*SHT_SHLIB*/                        \
      PRINT_SYM_STR_BREAK(SHT_DYNSYM);        /*Dynamic linker symbol table*/      \
      PRINT_SYM_STR_BREAK(SHT_INIT_ARRAY);    /*Array of constructors*/            \
      PRINT_SYM_STR_BREAK(SHT_FINI_ARRAY);    /*Array of destructors*/             \
      PRINT_SYM_STR_BREAK(SHT_PREINIT_ARRAY); /*Array of pre-constructors*/        \
      PRINT_SYM_STR_BREAK(SHT_GNU_HASH);      /*GNU-style hash table.*/            \
      PRINT_SYM_STR_BREAK(SHT_GNU_versym);    /*SHT_GNU_versym*/                   \
      PRINT_SYM_STR_BREAK(SHT_GNU_verneed);   /*Version needs section.*/           \
      PRINT_SYM_STR_DEFAULT(type);                                                 \
    }                                                                              \
  } while (0)

// section header flags
#define PRINT_SECTIONHEADER_FLAGS(flags)                                             \
  do                                                                                 \
  {                                                                                  \
    if (flags & SHF_WRITE) /*Writable*/                                              \
    {                                                                                \
      PRINT_SYM_STREND(SHF_WRITE, " ");                                              \
    }                                                                                \
    if (flags & SHF_ALLOC) /*Occupies memory during execution*/                      \
    {                                                                                \
      PRINT_SYM_STREND(SHF_ALLOC, " ");                                              \
    }                                                                                \
    if (flags & SHF_EXECINSTR) /*Executable*/                                        \
    {                                                                                \
      PRINT_SYM_STREND(SHF_EXECINSTR, " ");                                          \
    }                                                                                \
    if (flags & SHF_MERGE) /*Might be merged*/                                       \
    {                                                                                \
      PRINT_SYM_STREND(SHF_MERGE, " ");                                              \
    }                                                                                \
    if (flags & SHF_STRINGS) /*Contains nul-terminated strings*/                     \
    {                                                                                \
      PRINT_SYM_STREND(SHF_STRINGS, " ");                                            \
    }                                                                                \
    if (flags & SHF_INFO_LINK) /*`sh_info' contains SHT index*/                      \
    {                                                                                \
      PRINT_SYM_STREND(SHF_INFO_LINK, " ");                                          \
    }                                                                                \
    if (flags & SHF_LINK_ORDER) /*Preserve order after combining*/                   \
    {                                                                                \
      PRINT_SYM_STREND(SHF_LINK_ORDER, " ");                                         \
    }                                                                                \
    if (flags & SHF_OS_NONCONFORMING) /*Non-standard OS specific handling required*/ \
    {                                                                                \
      PRINT_SYM_STREND(SHF_OS_NONCONFORMING, " ");                                   \
    }                                                                                \
    if (flags & SHF_GROUP) /*Section is member of a group.*/                         \
    {                                                                                \
      PRINT_SYM_STREND(SHF_GROUP, " ");                                              \
    }                                                                                \
    if (flags & SHF_TLS) /*Section hold thread-local data.*/                         \
    {                                                                                \
      PRINT_SYM_STREND(SHF_TLS, " ");                                                \
    }                                                                                \
    if (flags & SHF_COMPRESSED) /*Section with compressed data.*/                    \
    {                                                                                \
      PRINT_SYM_STREND(SHF_COMPRESSED, " ");                                         \
    }                                                                                \
  } while (0)

// ~section header https://github.com/gongluck/CVIP/blob/master/cpp/elf.md#elf-section-header
#define PRINT_SECTIONHEADER(felf, shdrarry, i, shstrtab, BITS)                          \
  do                                                                                    \
  {                                                                                     \
    Elf##BITS##_Shdr shdr = shdrarry[i];                                                \
    printf("Name index: ");                                                             \
    PRINT_SYM_VALUE(shdr.sh_name);                                                      \
    const char *shdname = "";                                                           \
    if (shstrtab != NULL)                                                               \
    {                                                                                   \
      shdname = shstrtab + shdr.sh_name;                                                \
      printf("(%s)", shdname);                                                          \
    }                                                                                   \
    printf("\nType: ");                                                                 \
    PRINT_SECTIONHEADER_TYPE(shdr.sh_type);                                             \
    printf("\nFlags: ");                                                                \
    PRINT_SECTIONHEADER_FLAGS(shdr.sh_flags);                                           \
    printf("\nAddr: ");                                                                 \
    PRINT_SYM_ADDR(shdr.sh_addr);                                                       \
    if (strcmp(shdname, ".interp") == 0)                                                \
    {                                                                                   \
      char *interpstr = malloc(shdr.sh_size);                                           \
      fseek(felf, shdr.sh_addr, SEEK_SET);                                              \
      fread(interpstr, shdr.sh_size, 1, felf);                                          \
      printf("(%s)", interpstr);                                                        \
      free(interpstr);                                                                  \
      interpstr = NULL;                                                                 \
    }                                                                                   \
    printf("\nSection file offset: ");                                                  \
    PRINT_SYM_VALUE_HEX(shdr.sh_offset);                                                \
    printf("(");                                                                        \
    PRINT_SYM_VALUE(shdr.sh_offset);                                                    \
    printf(")");                                                                        \
    printf("\nSection size: ");                                                         \
    PRINT_SYM_VALUE(shdr.sh_size);                                                      \
    printf("\nLink: ");                                                                 \
    PRINT_SYM_VALUE(shdr.sh_link);                                                      \
    printf("\nInfo: ");                                                                 \
    PRINT_SYM_VALUE(shdr.sh_info);                                                      \
    printf("\nAlign: ");                                                                \
    PRINT_SYM_VALUE(shdr.sh_addralign);                                                 \
    printf("\nEntry size: ");                                                           \
    PRINT_SYM_VALUE(shdr.sh_entsize);                                                   \
    printf("\n");                                                                       \
    if (shdr.sh_offset > 0 && shdr.sh_size > 0)                                         \
    {                                                                                   \
      switch (shdr.sh_type)                                                             \
      {                                                                                 \
      case SHT_SYMTAB:                                                                  \
      case SHT_DYNSYM:                                                                  \
        PRINT_SECTION_ENTRY(felf, shdrarry, i, shdr.sh_link, BITS, Sym, PRINT_SYM);     \
        break;                                                                          \
      case SHT_RELA:                                                                    \
        PRINT_SECTION_ENTRY(felf, shdrarry, i, shdr.sh_link, BITS, Rela, PRINT_RELA);   \
        break;                                                                          \
      case SHT_DYNAMIC:                                                                 \
        PRINT_SECTION_ENTRY(felf, shdrarry, i, shdr.sh_link, BITS, Dyn, PRINT_DYNMAIC); \
        break;                                                                          \
      case SHT_STRTAB:                                                                  \
        PRINT_SECTION_STRINGS(felf, shdrarry, i, BITS);                                 \
        break;                                                                          \
      }                                                                                 \
      if (shdr.sh_flags & SHF_STRINGS)                                                  \
      {                                                                                 \
        PRINT_SECTION_STRINGS(felf, shdrarry, i, BITS);                                 \
      }                                                                                 \
    }                                                                                   \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

// symbol info bind
#define PRINT_SYM_INFOBIND(bind)                         \
  do                                                     \
  {                                                      \
    switch (bind)                                        \
    {                                                    \
      PRINT_SYM_STR_BREAK(STB_LOCAL);  /*Local symbol*/  \
      PRINT_SYM_STR_BREAK(STB_GLOBAL); /*Global symbol*/ \
      PRINT_SYM_STR_BREAK(STB_WEAK);   /*STB_WEAK*/      \
      PRINT_SYM_STR_DEFAULT(bind);                       \
    }                                                    \
  } while (0)

// symbol info type
#define PRINT_SYM_INFOTYPE(type)                                               \
  do                                                                           \
  {                                                                            \
    switch (type)                                                              \
    {                                                                          \
      PRINT_SYM_STR_BREAK(STT_NOTYPE);  /*Symbol type is unspecified*/         \
      PRINT_SYM_STR_BREAK(STT_OBJECT);  /*Symbol is a data object*/            \
      PRINT_SYM_STR_BREAK(STT_FUNC);    /*Symbol is a code object*/            \
      PRINT_SYM_STR_BREAK(STT_SECTION); /*Symbol associated with a section*/   \
      PRINT_SYM_STR_BREAK(STT_FILE);    /*Symbol's name is file name*/         \
      PRINT_SYM_STR_BREAK(STT_COMMON);  /*Symbol is a common data object*/     \
      PRINT_SYM_STR_BREAK(STT_TLS);     /*Symbol is thread-local data object*/ \
      PRINT_SYM_STR_DEFAULT(type);                                             \
    }                                                                          \
  } while (0)

// symbol section index
#define PRINT_SYM_INDEX(sindex)                                          \
  do                                                                     \
  {                                                                      \
    switch (sindex)                                                      \
    {                                                                    \
      PRINT_SYM_STR_BREAK(SHN_ABS);    /*Associated symbol is absolute*/ \
      PRINT_SYM_STR_BREAK(SHN_COMMON); /*Associated symbol is common*/   \
      PRINT_SYM_STR_DEFAULT(sindex);                                     \
    }                                                                    \
  } while (0)

// ~symbol https://github.com/gongluck/CVIP/blob/master/cpp/elf.md#symtab
#define PRINT_SYM(felf, sym, linkstr, BITS, TYPE)         \
  do                                                      \
  {                                                       \
    printf("\tName index: ");                             \
    PRINT_SYM_VALUE(sym.st_name);                         \
    if (linkstr != NULL)                                  \
    {                                                     \
      printf("(%s)", linkstr + sym.st_name);              \
    }                                                     \
    printf("\n\tValue: ");                                \
    PRINT_SYM_VALUE_HEX(sym.st_value);                    \
    printf("\n\tSize: ");                                 \
    PRINT_SYM_VALUE(sym.st_size);                         \
    printf("\n\tInfo bind: ");                            \
    PRINT_SYM_INFOBIND(ELF##BITS##_ST_BIND(sym.st_info)); \
    printf("\n\tInfo type: ");                            \
    PRINT_SYM_INFOTYPE(ELF##BITS##_ST_TYPE(sym.st_info)); \
    printf("\n\tOthers: ");                               \
    PRINT_SYM_VALUE(sym.st_other);                        \
    printf("\n\tSection index: ");                        \
    PRINT_SYM_INDEX(sym.st_shndx);                        \
    printf("(");                                          \
    PRINT_VALUE(sym.st_shndx);                            \
    printf(")");                                          \
    printf("\n");                                         \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

// relocation type in x86_64
#define PRINT_RELA_INFOTYPE(type)                                         \
  do                                                                      \
  {                                                                       \
    switch (type)                                                         \
    {                                                                     \
      PRINT_SYM_STR_BREAK(R_X86_64_GLOB_DAT);  /*Create GOT entry*/       \
      PRINT_SYM_STR_BREAK(R_X86_64_JUMP_SLOT); /*Create PLT entry*/       \
      PRINT_SYM_STR_BREAK(R_X86_64_RELATIVE);  /*Adjust by program base*/ \
      PRINT_SYM_STR_DEFAULT(type);                                        \
    }                                                                     \
  } while (0)

// ~relocation addend https://github.com/gongluck/CVIP/blob/master/cpp/elf.md#rel
#define PRINT_RELA(felf, rela, linkstr, BITS, TYPE)       \
  do                                                      \
  {                                                       \
    printf("\tOffset: ");                                 \
    PRINT_SYM_VALUE_HEX(rela.r_offset);                   \
    printf("(");                                          \
    PRINT_SYM_VALUE(rela.r_offset);                       \
    printf(")");                                          \
    printf("\n\tInfo type: ");                            \
    PRINT_RELA_INFOTYPE(ELF##BITS##_R_TYPE(rela.r_info)); \
    printf("\n\tInfo index: ");                           \
    PRINT_SYM_VALUE(ELF##BITS##_R_SYM(rela.r_info));      \
    printf("\n\tAddend: ");                               \
    PRINT_SYM_VALUE_HEX(rela.r_addend);                   \
    printf("(");                                          \
    PRINT_SYM_VALUE(rela.r_addend);                       \
    printf(")");                                          \
    printf("\n");                                         \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

// dynmaic type flag
#define PRINT_DYNMAIC_TYPE(type)                                                                                                   \
  do                                                                                                                               \
  {                                                                                                                                \
    switch (type)                                                                                                                  \
    {                                                                                                                              \
      PRINT_SYM_STR_BREAK(DT_NULL);            /*Marks end of dynamic section*/                                                    \
      PRINT_SYM_STR_BREAK(DT_NEEDED);          /*Name of needed library*/                                                          \
      PRINT_SYM_STR_BREAK(DT_PLTRELSZ);        /*Size in bytes of PLT relocs*/                                                     \
      PRINT_SYM_STR_BREAK(DT_PLTGOT);          /*Processor defined value*/                                                         \
      PRINT_SYM_STR_BREAK(DT_HASH);            /*Address of symbol hash table*/                                                    \
      PRINT_SYM_STR_BREAK(DT_STRTAB);          /*Address of string table*/                                                         \
      PRINT_SYM_STR_BREAK(DT_SYMTAB);          /*Address of symbol table*/                                                         \
      PRINT_SYM_STR_BREAK(DT_RELA);            /*Address of Rela relocs*/                                                          \
      PRINT_SYM_STR_BREAK(DT_RELASZ);          /*Total size of Rela relocs*/                                                       \
      PRINT_SYM_STR_BREAK(DT_RELAENT);         /*Size of one Rela reloc*/                                                          \
      PRINT_SYM_STR_BREAK(DT_STRSZ);           /*Size of string table*/                                                            \
      PRINT_SYM_STR_BREAK(DT_SYMENT);          /*Size of one symbol table entry*/                                                  \
      PRINT_SYM_STR_BREAK(DT_INIT);            /*Address of init function*/                                                        \
      PRINT_SYM_STR_BREAK(DT_FINI);            /*Address of termination function*/                                                 \
      PRINT_SYM_STR_BREAK(DT_SONAME);          /*Name of shared object*/                                                           \
      PRINT_SYM_STR_BREAK(DT_RPATH);           /*Library search path (deprecated)*/                                                \
      PRINT_SYM_STR_BREAK(DT_SYMBOLIC);        /*Start symbol search here*/                                                        \
      PRINT_SYM_STR_BREAK(DT_REL);             /*Address of Rel relocs*/                                                           \
      PRINT_SYM_STR_BREAK(DT_RELSZ);           /*Total size of Rel relocs*/                                                        \
      PRINT_SYM_STR_BREAK(DT_RELENT);          /*Size of one Rel reloc*/                                                           \
      PRINT_SYM_STR_BREAK(DT_PLTREL);          /*Type of reloc in PLT*/                                                            \
      PRINT_SYM_STR_BREAK(DT_DEBUG);           /*For debugging; unspecified*/                                                      \
      PRINT_SYM_STR_BREAK(DT_TEXTREL);         /*Reloc might modify .text*/                                                        \
      PRINT_SYM_STR_BREAK(DT_JMPREL);          /*Address of PLT relocs*/                                                           \
      PRINT_SYM_STR_BREAK(DT_BIND_NOW);        /*Process relocations of object*/                                                   \
      PRINT_SYM_STR_BREAK(DT_INIT_ARRAY);      /*Array with addresses of init fct*/                                                \
      PRINT_SYM_STR_BREAK(DT_FINI_ARRAY);      /*Array with addresses of fini fct*/                                                \
      PRINT_SYM_STR_BREAK(DT_INIT_ARRAYSZ);    /*Size in bytes of DT_INIT_ARRAY*/                                                  \
      PRINT_SYM_STR_BREAK(DT_FINI_ARRAYSZ);    /*Size in bytes of DT_FINI_ARRAY*/                                                  \
      PRINT_SYM_STR_BREAK(DT_RUNPATH);         /*Library search path*/                                                             \
      PRINT_SYM_STR_BREAK(DT_FLAGS);           /*Flags for the object being loaded*/                                               \
      PRINT_SYM_STR_BREAK(DT_PREINIT_ARRAY);   /*Array with addresses of preinit fct*/                                             \
      PRINT_SYM_STR_BREAK(DT_PREINIT_ARRAYSZ); /*size in bytes of DT_PREINIT_ARRAY*/                                               \
      PRINT_SYM_STR_BREAK(DT_SYMTAB_SHNDX);    /*Address of SYMTAB_SHNDX section*/                                                 \
      PRINT_SYM_STR_BREAK(DT_VERSYM);          /*The versioning entry types.  The next are defined as part of the GNU extension.*/ \
      PRINT_SYM_STR_BREAK(DT_GNU_HASH);        /*GNU-style hash table.*/                                                           \
      PRINT_SYM_STR_BREAK(DT_RELACOUNT);                                                                                           \
      PRINT_SYM_STR_BREAK(SHT_SUNW_COMDAT);                                                                                        \
      PRINT_SYM_STR_BREAK(SHT_GNU_verneed); /*Version needs section.*/                                                             \
      PRINT_SYM_STR_BREAK(SHT_GNU_versym);  /*Version symbol table.*/                                                              \
      PRINT_SYM_STR_DEFAULT(type);                                                                                                 \
    }                                                                                                                              \
  } while (0)

// dynmaic value
#define PRINT_DYNMAIC_VALUE(dyn)     \
  do                                 \
  {                                  \
    PRINT_VALUE(dyn.d_un.d_val);     \
    printf(" : ");                   \
    PRINT_VALUE_HEX(dyn.d_un.d_ptr); \
  } while (0)

// dynmaic flag type
#define PRINT_FLAG_TYPE(type)                                                  \
  do                                                                           \
  {                                                                            \
    switch (type)                                                              \
    {                                                                          \
      PRINT_SYM_STR_BREAK(DF_ORIGIN);     /*Object may use DF_ORIGIN*/         \
      PRINT_SYM_STR_BREAK(DF_SYMBOLIC);   /*Symbol resolutions starts here*/   \
      PRINT_SYM_STR_BREAK(DF_TEXTREL);    /*Object contains text relocations*/ \
      PRINT_SYM_STR_BREAK(DF_BIND_NOW);   /*No lazy binding for this object*/  \
      PRINT_SYM_STR_BREAK(DF_STATIC_TLS); /*Module uses the static TLS model*/ \
      PRINT_SYM_STR_DEFAULT(type);                                             \
    }                                                                          \
  } while (0)

// ~dynmaic entry https://github.com/gongluck/CVIP/blob/master/cpp/elf.md#dynamic
#define PRINT_DYNMAIC(felf, dyn, linkstr, BITS, TYPE) \
  do                                                  \
  {                                                   \
    printf("\tType: ");                               \
    PRINT_DYNMAIC_TYPE(dyn.d_tag);                    \
    printf("\n");                                     \
    printf("\tValue: ");                              \
    PRINT_DYNMAIC_VALUE(dyn);                         \
    if (dyn.d_tag == DT_FLAGS)                        \
    {                                                 \
      printf("(");                                    \
      PRINT_FLAG_TYPE(dyn.d_un.d_val);                \
      printf(")");                                    \
    }                                                 \
    switch (dyn.d_tag)                                \
    {                                                 \
    case DT_NEEDED:                                   \
    case DT_SONAME:                                   \
    case DT_RPATH:                                    \
      printf("(%s)", linkstr + dyn.d_un.d_ptr);       \
      break;                                          \
    }                                                 \
    printf("\n");                                     \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

// ~section entries
#define PRINT_SECTION_ENTRY(felf, shdrarry, i, link, BITS, TYPE, WORK) \
  do                                                                   \
  {                                                                    \
    Elf##BITS##_Shdr shdr = shdrarry[i];                               \
    Elf##BITS##_##TYPE *entries = malloc(shdr.sh_size);                \
    fseek(felf, shdr.sh_offset, SEEK_SET);                             \
    fread(entries, shdr.sh_size, 1, felf);                             \
    int entrycounts = shdr.sh_size / shdr.sh_entsize;                  \
    printf("\nEntry counts: %d\n", entrycounts);                       \
    PRINT_SPLIT();                                                     \
    char *linkstr = NULL;                                              \
    printf("link is %d\n", (int)link);                                 \
    if (link != 0)                                                     \
    {                                                                  \
      shdr = shdrarry[link];                                           \
      linkstr = malloc(shdr.sh_size);                                  \
      printf("linksize is %d\n", (int)shdr.sh_size);                   \
      fseek(felf, shdr.sh_offset, SEEK_SET);                           \
      fread(linkstr, shdr.sh_size, 1, felf);                           \
    }                                                                  \
    for (int i = 0; i < entrycounts; ++i)                              \
    {                                                                  \
      printf("\nEntry %2d:\n", i);                                     \
      WORK(felf, entries[i], linkstr, BITS, TYPE);                     \
    }                                                                  \
    free(linkstr);                                                     \
    linkstr = NULL;                                                    \
    free(entries);                                                     \
    entries = NULL;                                                    \
  } while (0)

// string table
#define PRINT_SECTION_STRINGS(felf, shdrarry, i, BITS) \
  do                                                   \
  {                                                    \
    Elf##BITS##_Shdr shdr = shdrarry[i];               \
    char *strs = malloc(shdr.sh_size);                 \
    fseek(felf, shdr.sh_offset, SEEK_SET);             \
    fread(strs, shdr.sh_size, 1, felf);                \
    printf("\tStrings:\t\t");                          \
    for (int pos = 0; pos < shdr.sh_size; ++pos)       \
    {                                                  \
      if (strs[pos] == '\0')                           \
      {                                                \
        printf("\n\t\t");                              \
      }                                                \
      else                                             \
      {                                                \
        putchar(strs[pos]);                            \
      }                                                \
    }                                                  \
    free(strs);                                        \
    strs = NULL;                                       \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRINT_SECTIONHEADER_CHECK(elfhdr, felf, BITS)              \
  do                                                               \
  {                                                                \
    if (elfhdr.e_shoff > 0 && elfhdr.e_shnum > 0)                  \
    {                                                              \
      PRINT_SPLIT();                                               \
      printf("Section headers:\n");                                \
      int size = elfhdr.e_shentsize * elfhdr.e_shnum;              \
      Elf##BITS##_Shdr *elfshr = malloc(size);                     \
      fseek(felf, elfhdr.e_shoff, SEEK_SET);                       \
      fread(elfshr, elfhdr.e_shentsize, elfhdr.e_shnum, felf);     \
      char *shstrtab = malloc(elfshr[elfhdr.e_shstrndx].sh_size);  \
      fseek(felf, elfshr[elfhdr.e_shstrndx].sh_offset, SEEK_SET);  \
      fread(shstrtab, elfshr[elfhdr.e_shstrndx].sh_size, 1, felf); \
      for (int i = 0; i < elfhdr.e_shnum; ++i)                     \
      {                                                            \
        printf("\n");                                              \
        printf("Section %2d:\n", i);                               \
        PRINT_SECTIONHEADER(felf, elfshr, i, shstrtab, BITS);      \
      }                                                            \
      free(shstrtab);                                              \
      shstrtab = NULL;                                             \
      free(elfshr);                                                \
      elfshr = NULL;                                               \
    }                                                              \
  } while (0)

#define PRINT_PROGRAMHEADER_CHECK(elfhdr, felf, BITS)          \
  do                                                           \
  {                                                            \
    if (elfhdr.e_phoff > 0 && elfhdr.e_phnum > 0)              \
    {                                                          \
      PRINT_SPLIT();                                           \
      printf("Program headers:\n\n");                          \
      int size = elfhdr.e_phentsize * elfhdr.e_phnum;          \
      Elf##BITS##_Phdr *elfphr = malloc(size);                 \
      fseek(felf, elfhdr.e_phoff, SEEK_SET);                   \
      fread(elfphr, elfhdr.e_phentsize, elfhdr.e_phnum, felf); \
      for (int i = 0; i < elfhdr.e_phnum; ++i)                 \
      {                                                        \
        printf("Program %2d:\n", i);                           \
        PRINT_PROGRAMHEADER(elfphr[i]);                        \
        printf("\n");                                          \
      }                                                        \
      free(elfphr);                                            \
      elfphr = NULL;                                           \
    }                                                          \
  } while (0)

#define PRINT_ELFHEADER_CHECK(felf, BITS)          \
  do                                               \
  {                                                \
    Elf##BITS##_Ehdr elfhdr = {0};                 \
    fread(&elfhdr, sizeof(elfhdr), 1, felf);       \
    PRINT_ELFHEADER(elfhdr);                       \
    PRINT_PROGRAMHEADER_CHECK(elfhdr, felf, BITS); \
    PRINT_SECTIONHEADER_CHECK(elfhdr, felf, BITS); \
  } while (0)

int main(int argc, char *argv[])
{
  printf("Usage : thisfile elffile\n\n");
  if (argc < 2)
  {
    EXIT(1);
  }

  FILE *felf = fopen(argv[1], "rb");
  if (felf == NULL)
  {
    EXIT(1);
  }
  printf("open elffile %s succeed\n\n", argv[1]);

  // analyse first 16 bytes magic code
  unsigned char e_ident[EI_NIDENT] = {0};
  fread(e_ident, EI_NIDENT, 1, felf);
  // match first 4 code "\177ELF"
  if (memcmp(ELFMAG, e_ident, 4) != 0)
  {
    EXIT(1);
  }
  // reset file point to begin
  fseek(felf, 0, SEEK_SET);
  // judge class byte
  switch (e_ident[EI_CLASS])
  {
  case ELFCLASS32: // 32 bits
  {
    PRINT_ELFHEADER_CHECK(felf, 32);
  }
  break;
  case ELFCLASS64: // 64 bits
  {
    PRINT_ELFHEADER_CHECK(felf, 64);
  }
  break;
  default:
    break;
  }

  fclose(felf);
  felf = NULL;
}
