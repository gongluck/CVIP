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

/// begin elf header

/// begin e_indent

// e_ident[EI_CLASS] 4
#define PRINT_IDENT_CLASS(class) \
  do                             \
  {                              \
    switch (class)               \
    {                            \
    case ELFCLASS32:             \
      PRINT_SYM_STR(ELFCLASS32); \
      break;                     \
    case ELFCLASS64:             \
      PRINT_SYM_STR(ELFCLASS64); \
      break;                     \
    default:                     \
      PRINT_VALUE_HEX(class);    \
      break;                     \
    }                            \
  } while (0)

// e_ident[EI_DATA] 5
#define PRINT_IDENT_DATA(data)    \
  do                              \
  {                               \
    switch (data)                 \
    {                             \
    case ELFDATA2LSB:             \
      PRINT_SYM_STR(ELFDATA2LSB); \
      break;                      \
    case ELFDATA2MSB:             \
      PRINT_SYM_STR(ELFDATA2MSB); \
      break;                      \
    default:                      \
      PRINT_VALUE_HEX(data);      \
      break;                      \
    }                             \
  } while (0)

// e_ident[EI_VERSION] 6
#define PRINT_IDENT_VERSION(version) \
  do                                 \
  {                                  \
    switch (version)                 \
    {                                \
    case EV_CURRENT:                 \
      PRINT_SYM_STR(EV_CURRENT);     \
      break;                         \
    default:                         \
      PRINT_VALUE_HEX(version);      \
      break;                         \
    }                                \
  } while (0)

// analysis e_ident
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

/// end e_indent

// e_type
#define PRINT_ELFHEADER_TYPE(type) \
  do                               \
  {                                \
    switch (type)                  \
    {                              \
    case ET_REL:                   \
      PRINT_SYM_STR(ET_REL);       \
      break;                       \
    case ET_EXEC:                  \
      PRINT_SYM_STR(ET_EXEC);      \
      break;                       \
    case ET_DYN:                   \
      PRINT_SYM_STR(ET_DYN);       \
      break;                       \
    case ET_CORE:                  \
      PRINT_SYM_STR(ET_CORE);      \
      break;                       \
    default:                       \
      PRINT_VALUE_HEX(type);       \
      break;                       \
    }                              \
  } while (0)

// e_machine
#define PRINT_ELFHEADER_MACHINE(machine) \
  do                                     \
  {                                      \
    switch (machine)                     \
    {                                    \
    case EM_NONE:                        \
      PRINT_SYM_STR(EM_NONE);            \
      break;                             \
    case EM_386:                         \
      PRINT_SYM_STR(EM_386);             \
      break;                             \
    case EM_X86_64:                      \
      PRINT_SYM_STR(EM_X86_64);          \
      break;                             \
    default:                             \
      PRINT_VALUE_HEX(machine);          \
      break;                             \
    }                                    \
  } while (0)

// elf header
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
    PRINT_SYM_VALUE(elfhdr.e_phoff);                             \
    printf("\nSection header offset: ");                         \
    PRINT_SYM_VALUE(elfhdr.e_shoff);                             \
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

/// end elf hdr

/// begin section header

// section header type
#define PRINT_SECTIONHEADER_TYPE(type) \
  do                                   \
  {                                    \
    switch (type)                      \
    {                                  \
    case SHT_NULL:                     \
      PRINT_SYM_STR(SHT_NULL);         \
      break;                           \
    case SHT_PROGBITS:                 \
      PRINT_SYM_STR(SHT_PROGBITS);     \
      break;                           \
    case SHT_SYMTAB:                   \
      PRINT_SYM_STR(SHT_SYMTAB);       \
      break;                           \
    case SHT_STRTAB:                   \
      PRINT_SYM_STR(SHT_STRTAB);       \
      break;                           \
    case SHT_RELA:                     \
      PRINT_SYM_STR(SHT_RELA);         \
      break;                           \
    case SHT_HASH:                     \
      PRINT_SYM_STR(SHT_HASH);         \
      break;                           \
    case SHT_DYNAMIC:                  \
      PRINT_SYM_STR(SHT_DYNAMIC);      \
      break;                           \
    case SHT_NOTE:                     \
      PRINT_SYM_STR(SHT_NOTE);         \
      break;                           \
    case SHT_NOBITS:                   \
      PRINT_SYM_STR(SHT_NOBITS);       \
      break;                           \
    case SHT_REL:                      \
      PRINT_SYM_STR(SHT_REL);          \
      break;                           \
    case SHT_SHLIB:                    \
      PRINT_SYM_STR(SHT_SHLIB);        \
      break;                           \
    case SHT_DYNSYM:                   \
      PRINT_SYM_STR(SHT_DYNSYM);       \
      break;                           \
    default:                           \
      PRINT_VALUE_HEX(type);           \
      break;                           \
    }                                  \
  } while (0)

// section header flags
#define PRINT_SECTIONHEADER_FLAGS(flags)           \
  do                                               \
  {                                                \
    if (flags & SHF_WRITE)                         \
    {                                              \
      PRINT_SYM_STREND(SHF_WRITE, " ");            \
    }                                              \
    if (flags & SHF_ALLOC)                         \
    {                                              \
      PRINT_SYM_STREND(SHF_ALLOC, " ");            \
    }                                              \
    if (flags & SHF_EXECINSTR)                     \
    {                                              \
      PRINT_SYM_STREND(SHF_EXECINSTR, " ");        \
    }                                              \
    if (flags & SHF_MERGE)                         \
    {                                              \
      PRINT_SYM_STREND(SHF_MERGE, " ");            \
    }                                              \
    if (flags & SHF_STRINGS)                       \
    {                                              \
      PRINT_SYM_STREND(SHF_STRINGS, " ");          \
    }                                              \
    if (flags & SHF_INFO_LINK)                     \
    {                                              \
      PRINT_SYM_STREND(SHF_INFO_LINK, " ");        \
    }                                              \
    if (flags & SHF_LINK_ORDER)                    \
    {                                              \
      PRINT_SYM_STREND(SHF_LINK_ORDER, " ");       \
    }                                              \
    if (flags & SHF_OS_NONCONFORMING)              \
    {                                              \
      PRINT_SYM_STREND(SHF_OS_NONCONFORMING, " "); \
    }                                              \
    if (flags & SHF_GROUP)                         \
    {                                              \
      PRINT_SYM_STREND(SHF_GROUP, " ");            \
    }                                              \
    if (flags & SHF_TLS)                           \
    {                                              \
      PRINT_SYM_STREND(SHF_TLS, " ");              \
    }                                              \
    if (flags & SHF_COMPRESSED)                    \
    {                                              \
      PRINT_SYM_STREND(SHF_COMPRESSED, " ");       \
    }                                              \
    if (flags & SHF_MASKOS)                        \
    {                                              \
      PRINT_SYM_STREND(SHF_MASKOS, " ");           \
    }                                              \
    if (flags & SHF_MASKPROC)                      \
    {                                              \
      PRINT_SYM_STREND(SHF_MASKPROC, " ");         \
    }                                              \
    if (flags & SHF_ORDERED)                       \
    {                                              \
      PRINT_SYM_STREND(SHF_ORDERED, " ");          \
    }                                              \
    if (flags & SHF_EXCLUDE)                       \
    {                                              \
      PRINT_SYM_STREND(SHF_EXCLUDE, " ");          \
    }                                              \
  } while (0)

// section header
#define PRINT_SECTIONHEADER(shdrarry, i, shstrtab, BITS)                            \
  do                                                                                \
  {                                                                                 \
    Elf##BITS##_Shdr shdr = shdrarry[i];                                            \
    printf("Name index: ");                                                         \
    PRINT_SYM_VALUE(shdr.sh_name);                                                  \
    if (shstrtab != NULL)                                                           \
    {                                                                               \
      printf("(%s)", shstrtab + shdr.sh_name);                                      \
    }                                                                               \
    printf("\nType: ");                                                             \
    PRINT_SECTIONHEADER_TYPE(shdr.sh_type);                                         \
    printf("\nFlags: ");                                                            \
    PRINT_SECTIONHEADER_FLAGS(shdr.sh_flags);                                       \
    printf("\nAddr: ");                                                             \
    PRINT_SYM_ADDR(shdr.sh_addr);                                                   \
    printf("\nSection file offset: ");                                              \
    PRINT_SYM_VALUE(shdr.sh_offset);                                                \
    printf("\nSection size: ");                                                     \
    PRINT_SYM_VALUE(shdr.sh_size);                                                  \
    printf("\nLink: ");                                                             \
    PRINT_SYM_VALUE(shdr.sh_link);                                                  \
    printf("\nInfo: ");                                                             \
    PRINT_SYM_VALUE(shdr.sh_info);                                                  \
    printf("\nAlign: ");                                                            \
    PRINT_SYM_VALUE(shdr.sh_addralign);                                             \
    printf("\nEntry size: ");                                                       \
    PRINT_SYM_VALUE(shdr.sh_entsize);                                               \
    printf("\n");                                                                   \
    if (shdr.sh_type == SHT_SYMTAB && shdr.sh_offset > 0 && shdr.sh_size > 0)       \
    {                                                                               \
      PRINT_SECTION_ENTRY(felf, shdrarry, i, shdr.sh_link, BITS, Sym, PRINT_SYM);   \
    }                                                                               \
    else if (shdr.sh_type == SHT_RELA && shdr.sh_offset > 0 && shdr.sh_size > 0)    \
    {                                                                               \
      PRINT_SECTION_ENTRY(felf, shdrarry, i, shdr.sh_link, BITS, Rela, PRINT_RELA); \
    }                                                                               \
  } while (0)

/// end section header

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
#define PRINT_SYM(sym, linkstr, BITS, TYPE)               \
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
#define PRINT_RELA(rela, linkstr, BITS, TYPE)            \
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
      WORK(entries[i], linkstr, BITS, TYPE);                           \
    }                                                                  \
    free(linkstr);                                                     \
    linkstr = NULL;                                                    \
    free(entries);                                                     \
    entries = NULL;                                                    \
  } while (0)

/// end section entries

///////////////////////////////////////////////////////////////////////////////////////////////////

/// begin program header

// program header type
#define PRINT_PROGRAMHEADER_TYPE(type) \
  do                                   \
  {                                    \
    switch (type)                      \
    {                                  \
    case PT_NULL:                      \
      PRINT_SYM_STR(PT_NULL);          \
      break;                           \
    case PT_LOAD:                      \
      PRINT_SYM_STR(PT_LOAD);          \
      break;                           \
    case PT_DYNAMIC:                   \
      PRINT_SYM_STR(PT_DYNAMIC);       \
      break;                           \
    case PT_INTERP:                    \
      PRINT_SYM_STR(PT_INTERP);        \
      break;                           \
    case PT_NOTE:                      \
      PRINT_SYM_STR(PT_NOTE);          \
      break;                           \
    case PT_SHLIB:                     \
      PRINT_SYM_STR(PT_SHLIB);         \
      break;                           \
    case PT_PHDR:                      \
      PRINT_SYM_STR(PT_PHDR);          \
      break;                           \
    case PT_TLS:                       \
      PRINT_SYM_STR(PT_TLS);           \
      break;                           \
    case PT_LOOS:                      \
      PRINT_SYM_STR(PT_LOOS);          \
      break;                           \
    case PT_HIOS:                      \
      PRINT_SYM_STR(PT_HIOS);          \
      break;                           \
    case PT_LOPROC:                    \
      PRINT_SYM_STR(PT_LOPROC);        \
      break;                           \
    case PT_HIPROC:                    \
      PRINT_SYM_STR(PT_HIPROC);        \
      break;                           \
    case PT_GNU_EH_FRAME:              \
      PRINT_SYM_STR(PT_GNU_EH_FRAME);  \
      break;                           \
    case PT_GNU_STACK:                 \
      PRINT_SYM_STR(PT_GNU_STACK);     \
      break;                           \
    default:                           \
      PRINT_VALUE_HEX(type);           \
      break;                           \
    }                                  \
  } while (0)

// program header flags
#define PRINT_PROGRAMHEADER_FLAGS(flags) \
  do                                     \
  {                                      \
    if (flags & PF_X)                    \
    {                                    \
      PRINT_SYM_STREND(PF_X, " ");       \
    }                                    \
    if (flags & PF_W)                    \
    {                                    \
      PRINT_SYM_STREND(PF_W, " ");       \
    }                                    \
    if (flags & PF_R)                    \
    {                                    \
      PRINT_SYM_STREND(PF_R, " ");       \
    }                                    \
  } while (0)

// program header
#define PRINT_PROGRAMHEADER(elfphr)            \
  do                                           \
  {                                            \
    printf("\tType: ");                        \
    PRINT_PROGRAMHEADER_TYPE(elfphr.p_type);   \
    printf("\n\tOffset: ");                    \
    PRINT_SYM_VALUE(elfphr.p_offset);          \
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

/// end program header

///////////////////////////////////////////////////////////////////////////////////////////////////

#define PRINT_SECTIONHEADER_CHECK(elfhdr, felf, BITS)              \
  do                                                               \
  {                                                                \
    if (elfhdr.e_shoff > 0 && elfhdr.e_shnum > 0)                  \
    {                                                              \
      PRINT_SPLIT();                                               \
      printf("Section headers:\n\n");                              \
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
        PRINT_SECTIONHEADER(elfshr, i, shstrtab, BITS);            \
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
