/*
 * @Author: gongluck
 * @Date: 2023-01-05 10:48:45
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-10 15:07:25
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

#define PRINT_SYM_STR(sym) \
  do                       \
  {                        \
    printf(#sym);          \
  } while (0)

#define PRINT_SYM_STREND(sym, end) \
  do                               \
  {                                \
    printf(#sym);                  \
    printf(end);                   \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

// e_ident[EI_CLASS] 4
#define PRINT_IDENT_CLASS(class)        \
  do                                    \
  {                                     \
    switch (class)                      \
    {                                   \
    case ELFCLASS32: /*32-bit objects*/ \
      PRINT_SYM_STR(ELFCLASS32);        \
      break;                            \
    case ELFCLASS64: /*64-bit objects*/ \
      PRINT_SYM_STR(ELFCLASS64);        \
      break;                            \
    default:                            \
      PRINT_VALUE_HEX(class);           \
      break;                            \
    }                                   \
  } while (0)

// e_ident[EI_DATA] 5
#define PRINT_IDENT_DATA(data)          \
  do                                    \
  {                                     \
    switch (data)                       \
    {                                   \
    case ELFDATA2LSB: /*little endian*/ \
      PRINT_SYM_STR(ELFDATA2LSB);       \
      break;                            \
    case ELFDATA2MSB: /*big endian*/    \
      PRINT_SYM_STR(ELFDATA2MSB);       \
      break;                            \
    default:                            \
      PRINT_VALUE_HEX(data);            \
      break;                            \
    }                                   \
  } while (0)

// e_ident[EI_VERSION] 6
#define PRINT_IDENT_VERSION(version)     \
  do                                     \
  {                                      \
    switch (version)                     \
    {                                    \
    case EV_CURRENT: /*Current version*/ \
      PRINT_SYM_STR(EV_CURRENT);         \
      break;                             \
    default:                             \
      PRINT_VALUE_HEX(version);          \
      break;                             \
    }                                    \
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
#define PRINT_ELFHEADER_TYPE(type)      \
  do                                    \
  {                                     \
    switch (type)                       \
    {                                   \
    case ET_REL: /*Relocatable file*/   \
      PRINT_SYM_STR(ET_REL);            \
      break;                            \
    case ET_EXEC: /*Executable file*/   \
      PRINT_SYM_STR(ET_EXEC);           \
      break;                            \
    case ET_DYN: /*Shared object file*/ \
      PRINT_SYM_STR(ET_DYN);            \
      break;                            \
    case ET_CORE: /*Core file*/         \
      PRINT_SYM_STR(ET_CORE);           \
      break;                            \
    default:                            \
      PRINT_VALUE_HEX(type);            \
      break;                            \
    }                                   \
  } while (0)

// e_machine
#define PRINT_ELFHEADER_MACHINE(machine)                    \
  do                                                        \
  {                                                         \
    switch (machine)                                        \
    {                                                       \
    case EM_386:                                            \
      PRINT_SYM_STR(EM_386); /*Intel 80386*/                \
      break;                                                \
    case EM_X86_64:                                         \
      PRINT_SYM_STR(EM_X86_64); /*AMD x86-64 architecture*/ \
      break;                                                \
    default:                                                \
      PRINT_VALUE_HEX(machine);                             \
      break;                                                \
    }                                                       \
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
#define PRINT_PROGRAMHEADER_TYPE(type)                   \
  do                                                     \
  {                                                      \
    switch (type)                                        \
    {                                                    \
    case PT_LOAD: /*Loadable program segment*/           \
      PRINT_SYM_STR(PT_LOAD);                            \
      break;                                             \
    case PT_DYNAMIC: /*Dynamic linking information*/     \
      PRINT_SYM_STR(PT_DYNAMIC);                         \
      break;                                             \
    case PT_INTERP: /*Program interpreter*/              \
      PRINT_SYM_STR(PT_INTERP);                          \
      break;                                             \
    case PT_NOTE: /*Auxiliary information*/              \
      PRINT_SYM_STR(PT_NOTE);                            \
      break;                                             \
    case PT_SHLIB: /*Reserved*/                          \
      PRINT_SYM_STR(PT_SHLIB);                           \
      break;                                             \
    case PT_PHDR: /*Entry for header table itself*/      \
      PRINT_SYM_STR(PT_PHDR);                            \
      break;                                             \
    case PT_TLS: /*Thread-local storage segment*/        \
      PRINT_SYM_STR(PT_TLS);                             \
      break;                                             \
    case PT_GNU_EH_FRAME: /*GCC .eh_frame_hdr segment*/  \
      PRINT_SYM_STR(PT_GNU_EH_FRAME);                    \
      break;                                             \
    case PT_GNU_STACK: /*Indicates stack executability*/ \
      PRINT_SYM_STR(PT_GNU_STACK);                       \
      break;                                             \
    case PT_GNU_RELRO: /*Read-only after relocation*/    \
      PRINT_SYM_STR(PT_GNU_RELRO);                       \
      break;                                             \
    case PT_GNU_PROPERTY: /*GNU property*/               \
      PRINT_SYM_STR(PT_GNU_PROPERTY);                    \
      break;                                             \
    default:                                             \
      PRINT_VALUE_HEX(type);                             \
      break;                                             \
    }                                                    \
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
#define PRINT_SECTIONHEADER_TYPE(type)                    \
  do                                                      \
  {                                                       \
    switch (type)                                         \
    {                                                     \
    case SHT_PROGBITS: /*Program data*/                   \
      PRINT_SYM_STR(SHT_PROGBITS);                        \
      break;                                              \
    case SHT_SYMTAB: /*Symbol table*/                     \
      PRINT_SYM_STR(SHT_SYMTAB);                          \
      break;                                              \
    case SHT_STRTAB: /*String table*/                     \
      PRINT_SYM_STR(SHT_STRTAB);                          \
      break;                                              \
    case SHT_RELA: /*Relocation entries with addends*/    \
      PRINT_SYM_STR(SHT_RELA);                            \
      break;                                              \
    case SHT_HASH: /*Symbol hash table*/                  \
      PRINT_SYM_STR(SHT_HASH);                            \
      break;                                              \
    case SHT_DYNAMIC: /*Dynamic linking information*/     \
      PRINT_SYM_STR(SHT_DYNAMIC);                         \
      break;                                              \
    case SHT_NOTE: /*Notes*/                              \
      PRINT_SYM_STR(SHT_NOTE);                            \
      break;                                              \
    case SHT_NOBITS: /*Program space with no data (bss)*/ \
      PRINT_SYM_STR(SHT_NOBITS);                          \
      break;                                              \
    case SHT_REL: /*Relocation entries, no addends*/      \
      PRINT_SYM_STR(SHT_REL);                             \
      break;                                              \
    case SHT_SHLIB: /*SHT_SHLIB*/                         \
      PRINT_SYM_STR(SHT_SHLIB);                           \
      break;                                              \
    case SHT_DYNSYM: /*Dynamic linker symbol table*/      \
      PRINT_SYM_STR(SHT_DYNSYM);                          \
      break;                                              \
    case SHT_GNU_HASH: /*GNU-style hash table.*/          \
      PRINT_SYM_STR(SHT_GNU_HASH);                        \
      break;                                              \
    default:                                              \
      PRINT_VALUE_HEX(type);                              \
      break;                                              \
    }                                                     \
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
      }                                                                                 \
      if (shdr.sh_flags & SHF_STRINGS)                                                  \
      {                                                                                 \
        PRINT_SECTION_STRINGS(felf, shdrarry, i, BITS);                                 \
      }                                                                                 \
    }                                                                                   \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

/// begin symbol table

// symbol info bind
#define PRINT_SYM_INFOBIND(bind) \
  do                             \
  {                              \
    switch (bind)                \
    {                            \
    case STB_LOCAL:              \
      PRINT_SYM_STR(STB_LOCAL);  \
      break;                     \
    case STB_GLOBAL:             \
      PRINT_SYM_STR(STB_GLOBAL); \
      break;                     \
    case STB_WEAK:               \
      PRINT_SYM_STR(STB_WEAK);   \
      break;                     \
    default:                     \
      PRINT_VALUE_HEX(bind);     \
      break;                     \
    }                            \
  } while (0)

// symbol info type
#define PRINT_SYM_INFOTYPE(type)  \
  do                              \
  {                               \
    switch (type)                 \
    {                             \
    case STT_NOTYPE:              \
      PRINT_SYM_STR(STT_NOTYPE);  \
      break;                      \
    case STT_OBJECT:              \
      PRINT_SYM_STR(STT_OBJECT);  \
      break;                      \
    case STT_FUNC:                \
      PRINT_SYM_STR(STT_FUNC);    \
      break;                      \
    case STT_SECTION:             \
      PRINT_SYM_STR(STT_SECTION); \
      break;                      \
    case STT_FILE:                \
      PRINT_SYM_STR(STT_FILE);    \
      break;                      \
    case STT_COMMON:              \
      PRINT_SYM_STR(STT_COMMON);  \
      break;                      \
    case STT_TLS:                 \
      PRINT_SYM_STR(STT_TLS);     \
      break;                      \
    default:                      \
      PRINT_VALUE_HEX(type);      \
      break;                      \
    }                             \
  } while (0)

// symbol section index
#define PRINT_SYM_INDEX(sindex)  \
  do                             \
  {                              \
    switch (sindex)              \
    {                            \
    case SHN_ABS:                \
      PRINT_SYM_STR(SHN_ABS);    \
      break;                     \
    case SHN_COMMON:             \
      PRINT_SYM_STR(SHN_COMMON); \
      break;                     \
    default:                     \
      PRINT_VALUE(sindex);       \
      break;                     \
    }                            \
  } while (0)

// symbol
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
    printf("\n");                                         \
  } while (0)

/// end symbol table

/// begin relocation addend

// relocation type in x86_64
#define PRINT_RELA_INFOTYPE(type)        \
  do                                     \
  {                                      \
    switch (type)                        \
    {                                    \
    case R_X86_64_NONE:                  \
      PRINT_SYM_STR(R_X86_64_NONE);      \
      break;                             \
    case R_X86_64_64:                    \
      PRINT_SYM_STR(R_X86_64_64);        \
      break;                             \
    case R_X86_64_PC32:                  \
      PRINT_SYM_STR(R_X86_64_PC32);      \
      break;                             \
    case R_X86_64_GOT32:                 \
      PRINT_SYM_STR(R_X86_64_GOT32);     \
      break;                             \
    case R_X86_64_PLT32:                 \
      PRINT_SYM_STR(R_X86_64_PLT32);     \
      break;                             \
    case R_X86_64_COPY:                  \
      PRINT_SYM_STR(R_X86_64_COPY);      \
      break;                             \
    case R_X86_64_GLOB_DAT:              \
      PRINT_SYM_STR(R_X86_64_GLOB_DAT);  \
      break;                             \
    case R_X86_64_JUMP_SLOT:             \
      PRINT_SYM_STR(R_X86_64_JUMP_SLOT); \
      break;                             \
    case R_X86_64_RELATIVE:              \
      PRINT_SYM_STR(R_X86_64_RELATIVE);  \
      break;                             \
    case R_X86_64_GOTPCREL:              \
      PRINT_SYM_STR(R_X86_64_GOTPCREL);  \
      break;                             \
    case R_X86_64_32:                    \
      PRINT_SYM_STR(R_X86_64_32);        \
      break;                             \
    case R_X86_64_32S:                   \
      PRINT_SYM_STR(R_X86_64_32S);       \
      break;                             \
    case R_X86_64_16:                    \
      PRINT_SYM_STR(R_X86_64_16);        \
      break;                             \
    case R_X86_64_PC16:                  \
      PRINT_SYM_STR(R_X86_64_PC16);      \
      break;                             \
    case R_X86_64_8:                     \
      PRINT_SYM_STR(R_X86_64_8);         \
      break;                             \
    case R_X86_64_PC8:                   \
      PRINT_SYM_STR(R_X86_64_PC8);       \
      break;                             \
    default:                             \
      PRINT_VALUE_HEX(type);             \
      break;                             \
    }                                    \
  } while (0)

// relocation addend
#define PRINT_RELA(felf, rela, linkstr, BITS, TYPE)      \
  do                                                     \
  {                                                      \
    printf("\tOffset: ");                                \
    PRINT_SYM_VALUE(rela.r_offset);                      \
    printf("\n\tInfo type: ");                           \
    PRINT_RELA_INFOTYPE(ELF##BITS##_R_SYM(rela.r_info)); \
    printf("\n\tInfo index: ");                          \
    PRINT_SYM_VALUE(ELF##BITS##_R_SYM(rela.r_info));     \
    printf("\n\tAddend: ");                              \
    PRINT_SYM_VALUE(rela.r_addend);                      \
    printf("\n");                                        \
  } while (0)

/// end relocation addend

/// begin dynmaic

// dynmaic type flag
#define PRINT_DYNMAIC_TYPE(type)         \
  do                                     \
  {                                      \
    switch (type)                        \
    {                                    \
    case DT_NULL:                        \
      PRINT_SYM_STR(DT_NULL);            \
      break;                             \
    case DT_NEEDED:                      \
      PRINT_SYM_STR(DT_NEEDED);          \
      break;                             \
    case DT_PLTRELSZ:                    \
      PRINT_SYM_STR(DT_PLTRELSZ);        \
      break;                             \
    case DT_PLTGOT:                      \
      PRINT_SYM_STR(DT_PLTGOT);          \
      break;                             \
    case DT_HASH:                        \
      PRINT_SYM_STR(DT_HASH);            \
      break;                             \
    case DT_STRTAB:                      \
      PRINT_SYM_STR(DT_STRTAB);          \
      break;                             \
    case DT_SYMTAB:                      \
      PRINT_SYM_STR(DT_SYMTAB);          \
      break;                             \
    case DT_RELA:                        \
      PRINT_SYM_STR(DT_RELA);            \
      break;                             \
    case DT_RELASZ:                      \
      PRINT_SYM_STR(DT_RELASZ);          \
      break;                             \
    case DT_RELAENT:                     \
      PRINT_SYM_STR(DT_RELAENT);         \
      break;                             \
    case DT_STRSZ:                       \
      PRINT_SYM_STR(DT_STRSZ);           \
      break;                             \
    case DT_SYMENT:                      \
      PRINT_SYM_STR(DT_SYMENT);          \
      break;                             \
    case DT_INIT:                        \
      PRINT_SYM_STR(DT_INIT);            \
      break;                             \
    case DT_FINI:                        \
      PRINT_SYM_STR(DT_FINI);            \
      break;                             \
    case DT_SONAME:                      \
      PRINT_SYM_STR(DT_SONAME);          \
      break;                             \
    case DT_RPATH:                       \
      PRINT_SYM_STR(DT_RPATH);           \
      break;                             \
    case DT_SYMBOLIC:                    \
      PRINT_SYM_STR(DT_SYMBOLIC);        \
      break;                             \
    case DT_REL:                         \
      PRINT_SYM_STR(DT_REL);             \
      break;                             \
    case DT_RELSZ:                       \
      PRINT_SYM_STR(DT_RELSZ);           \
      break;                             \
    case DT_RELENT:                      \
      PRINT_SYM_STR(DT_RELENT);          \
      break;                             \
    case DT_PLTREL:                      \
      PRINT_SYM_STR(DT_PLTREL);          \
      break;                             \
    case DT_DEBUG:                       \
      PRINT_SYM_STR(DT_DEBUG);           \
      break;                             \
    case DT_TEXTREL:                     \
      PRINT_SYM_STR(DT_TEXTREL);         \
      break;                             \
    case DT_JMPREL:                      \
      PRINT_SYM_STR(DT_JMPREL);          \
      break;                             \
    case DT_BIND_NOW:                    \
      PRINT_SYM_STR(DT_BIND_NOW);        \
      break;                             \
    case DT_INIT_ARRAY:                  \
      PRINT_SYM_STR(DT_INIT_ARRAY);      \
      break;                             \
    case DT_FINI_ARRAY:                  \
      PRINT_SYM_STR(DT_FINI_ARRAY);      \
      break;                             \
    case DT_INIT_ARRAYSZ:                \
      PRINT_SYM_STR(DT_INIT_ARRAYSZ);    \
      break;                             \
    case DT_FINI_ARRAYSZ:                \
      PRINT_SYM_STR(DT_FINI_ARRAYSZ);    \
      break;                             \
    case DT_RUNPATH:                     \
      PRINT_SYM_STR(DT_RUNPATH);         \
      break;                             \
    case DT_FLAGS:                       \
      PRINT_SYM_STR(DT_FLAGS);           \
      break;                             \
    /*case DT_ENCODING:                  \
      PRINT_SYM_STR(DT_ENCODING);        \
      break;*/                           \
    case DT_PREINIT_ARRAY:               \
      PRINT_SYM_STR(DT_PREINIT_ARRAY);   \
      break;                             \
    case DT_PREINIT_ARRAYSZ:             \
      PRINT_SYM_STR(DT_PREINIT_ARRAYSZ); \
      break;                             \
    case DT_SYMTAB_SHNDX:                \
      PRINT_SYM_STR(DT_SYMTAB_SHNDX);    \
      break;                             \
    default:                             \
      PRINT_VALUE_HEX(type);             \
      break;                             \
    }                                    \
  } while (0)

// dynmaic value
#define PRINT_DYNMAIC_VALUE(dyn)     \
  do                                 \
  {                                  \
    PRINT_VALUE(dyn.d_un.d_val);     \
    printf(" : ");                   \
    PRINT_VALUE_HEX(dyn.d_un.d_ptr); \
  } while (0)

// dynmaic entry
#define PRINT_DYNMAIC(felf, dyn, linkstr, BITS, TYPE) \
  do                                                  \
  {                                                   \
    printf("\tType: ");                               \
    PRINT_DYNMAIC_TYPE(dyn.d_tag);                    \
    printf("\n");                                     \
    printf("\tValue: ");                              \
    PRINT_DYNMAIC_VALUE(dyn);                         \
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

/// end dynmaic

/// begin section entries

// section entries
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

/// end section entries

#define PRINT_SECTION_STRINGS(felf, shdrarry, i, BITS) \
  do                                                   \
  {                                                    \
    Elf##BITS##_Shdr shdr = shdrarry[i];               \
    char *strs = malloc(shdr.sh_size);                 \
    fseek(felf, shdr.sh_offset, SEEK_SET);             \
    fread(strs, shdr.sh_size, 1, felf);                \
    printf("\tStrings: %s\n", strs);                   \
    free(strs);                                        \
    strs = NULL;                                       \
  } while (0)

///////////////////////////////////////////////////////////////////////////////////////////////////

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
