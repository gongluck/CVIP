/*
 * @Author: gongluck
 * @Date: 2023-01-05 10:48:45
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-06 17:25:13
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

#define PRINT_SYM_ADDR(sym)                                       \
  do                                                              \
  {                                                               \
    printf("%s : 0x%.2llx\n", #sym, (long long unsigned int)sym); \
  } while (0)

/// begin elf header

/// begin e_indent

// e_ident[EI_CLASS] 4
#define PRINT_IDENT_CLASS(class)          \
  do                                      \
  {                                       \
    switch (class)                        \
    {                                     \
    case ELFCLASS32:                      \
      printf("ELF32");                    \
      break;                              \
    case ELFCLASS64:                      \
      printf("ELF64");                    \
      break;                              \
    default:                              \
      printf("unknow class %.2x", class); \
      break;                              \
    }                                     \
  } while (0)

// e_ident[EI_DATA] 5
#define PRINT_IDENT_DATA(data)          \
  do                                    \
  {                                     \
    switch (data)                       \
    {                                   \
    case ELFDATA2LSB:                   \
      printf("LSB");                    \
      break;                            \
    case ELFDATA2MSB:                   \
      printf("MSB");                    \
      break;                            \
    default:                            \
      printf("unknow data %.2x", data); \
      break;                            \
    }                                   \
  } while (0)

// e_ident[EI_VERSION] 6
#define PRINT_IDENT_VERSION(version)          \
  do                                          \
  {                                           \
    switch (version)                          \
    {                                         \
    case EV_CURRENT:                          \
      printf("current");                      \
      break;                                  \
    default:                                  \
      printf("unknow version %.2x", version); \
      break;                                  \
    }                                         \
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
    printf("\n");                           \
                                            \
    printf("Class: ");                      \
    PRINT_IDENT_CLASS(ident[EI_CLASS]);     \
    printf("\n");                           \
                                            \
    printf("Data: ");                       \
    PRINT_IDENT_DATA(ident[EI_DATA]);       \
    printf("\n");                           \
                                            \
    printf("Version: ");                    \
    PRINT_IDENT_VERSION(ident[EI_VERSION]); \
    printf("\n");                           \
                                            \
  } while (0)

/// end e_indent

// e_type
#define PRINT_ELFHEADER_TYPE(type)      \
  do                                    \
  {                                     \
    switch (type)                       \
    {                                   \
    case ET_REL:                        \
      printf("REL");                    \
      break;                            \
    case ET_EXEC:                       \
      printf("EXEC");                   \
      break;                            \
    case ET_DYN:                        \
      printf("DYN");                    \
      break;                            \
    case ET_CORE:                       \
      printf("CORE");                   \
      break;                            \
    default:                            \
      printf("unknow type %.2x", type); \
      break;                            \
    }                                   \
  } while (0)

// e_machine
#define PRINT_ELFHEADER_MACHINE(machine)                                                                     \
  do                                                                                                         \
  {                                                                                                          \
    switch (machine)                                                                                         \
    {                                                                                                        \
    case EM_NONE:                                                                                            \
      printf("NONE");                                                                                        \
      break;                                                                                                 \
    case EM_M32:                                                                                             \
      printf("M32");                                                                                         \
      break;                                                                                                 \
    case EM_SPARC:                                                                                           \
      printf("SPARC");                                                                                       \
      break;                                                                                                 \
    case EM_386:                                                                                             \
      printf("386");                                                                                         \
      break;                                                                                                 \
    case EM_68K:                                                                                             \
      printf("68K");                                                                                         \
      break;                                                                                                 \
    case EM_88K:                                                                                             \
      printf("88K");                                                                                         \
      break;                                                                                                 \
    /*case EM_486:                                                                                           \
      printf("Perhaps disused");                                                                             \
      break;*/                                                                                               \
    case EM_860:                                                                                             \
      printf("860");                                                                                         \
      break;                                                                                                 \
    case EM_MIPS:                                                                                            \
      printf("MIPS R3000 (officially, big-endian only)");                                                    \
      break;                                                                                                 \
    case EM_PARISC:                                                                                          \
      printf("HPPA");                                                                                        \
      break;                                                                                                 \
    case EM_SPARC32PLUS:                                                                                     \
      printf("Sun's \"v8plus\"");                                                                            \
      break;                                                                                                 \
    case EM_PPC:                                                                                             \
      printf("PowerPC");                                                                                     \
      break;                                                                                                 \
    case EM_PPC64:                                                                                           \
      printf("PowerPC64");                                                                                   \
      break;                                                                                                 \
    case EM_SPU:                                                                                             \
      printf("Cell BE SPU");                                                                                 \
      break;                                                                                                 \
    case EM_SH:                                                                                              \
      printf("SuperH");                                                                                      \
      break;                                                                                                 \
    case EM_SPARCV9:                                                                                         \
      printf("SPARC v9 64-bit");                                                                             \
      break;                                                                                                 \
    case EM_IA_64:                                                                                           \
      printf("HP/Intel IA-64");                                                                              \
      break;                                                                                                 \
    case EM_X86_64:                                                                                          \
      printf("AMD x86-64");                                                                                  \
      break;                                                                                                 \
    case EM_S390:                                                                                            \
      printf("IBM S/390");                                                                                   \
      break;                                                                                                 \
    case EM_CRIS:                                                                                            \
      printf("Axis Communications 32-bit embedded processor");                                               \
      break;                                                                                                 \
    case EM_V850:                                                                                            \
      printf("NEC v850");                                                                                    \
      break;                                                                                                 \
    case EM_M32R:                                                                                            \
      printf("Renesas M32R");                                                                                \
      break;                                                                                                 \
    case EM_H8_300:                                                                                          \
      printf("Renesas H8/300,300H,H8S");                                                                     \
      break;                                                                                                 \
    case EM_MN10300:                                                                                         \
      printf("Panasonic/MEI MN10300, AM33");                                                                 \
      break;                                                                                                 \
    case EM_BLACKFIN:                                                                                        \
      printf("ADI Blackfin Processor");                                                                      \
      break;                                                                                                 \
    case EM_TI_C6000:                                                                                        \
      printf("TI C6X DSPs");                                                                                 \
      break;                                                                                                 \
    /*case EM_FRV:                                                                                           \
      printf("Fujitsu FR-V");                                                                                \
      break;*/                                                                                               \
    case EM_AVR32:                                                                                           \
      printf("Atmel AVR32");                                                                                 \
      break;                                                                                                 \
    case EM_ALPHA:                                                                                           \
      printf("This is an interim value that we will use until the committee comes up with a final number."); \
      break;                                                                                                 \
    /*case EM_CYGNUS_V850:                                                                                   \
      printf("Bogus old v850 magic number, used by old tools.");                                             \
      break;                                                                                                 \
    case EM_CYGNUS_M32R:                                                                                     \
      printf("Bogus old m32r magic number, used by old tools.");                                             \
      break;                                                                                                 \
    case EM_S390_OLD:                                                                                        \
      printf("This is the old interim value for S/390 architecture");                                        \
      break;                                                                                                 \
    case EM_CYGNUS_MN10300:                                                                                  \
      printf("Also Panasonic/MEI MN10300, AM33");                                                            \
      break;*/                                                                                               \
    default:                                                                                                 \
      printf("unknow machine %.2x", machine);                                                                \
      break;                                                                                                 \
    }                                                                                                        \
  } while (0)

// elf header
#define PRINT_ELFHEADER(elfhdr)                                                                                 \
  do                                                                                                            \
  {                                                                                                             \
    PRINT_IDENT(elfhdr.e_ident);                                                                                \
                                                                                                                \
    printf("Type: ");                                                                                           \
    PRINT_ELFHEADER_TYPE(elfhdr.e_type);                                                                        \
    printf("\n");                                                                                               \
                                                                                                                \
    printf("Machine: ");                                                                                        \
    PRINT_ELFHEADER_MACHINE(elfhdr.e_machine);                                                                  \
    printf("\n");                                                                                               \
                                                                                                                \
    printf("Version: 0x%.2llx\n", (long long unsigned int)elfhdr.e_version);                                    \
    printf("Entry: 0x%.2llx\n", (long long unsigned int)elfhdr.e_entry);                                        \
    printf("Program header offset: %lld\n", (long long unsigned int)elfhdr.e_phoff);                            \
    printf("Section header offset: %lld\n", (long long unsigned int)elfhdr.e_shoff);                            \
    printf("Flags: 0x%.2llx\n", (long long unsigned int)elfhdr.e_flags);                                        \
    printf("Header size: %lld\n", (long long unsigned int)elfhdr.e_ehsize);                                     \
    printf("Program header entry size: %lld\n", (long long unsigned int)elfhdr.e_phentsize);                    \
    printf("Program header entry counts: %lld\n", (long long unsigned int)elfhdr.e_phnum);                      \
    printf("Section header entry size: %lld\n", (long long unsigned int)elfhdr.e_shentsize);                    \
    printf("Section header entry counts: %lld\n", (long long unsigned int)elfhdr.e_shnum);                      \
    printf("Section header string table (.shstrtab) index: %lld\n", (long long unsigned int)elfhdr.e_shstrndx); \
  } while (0)

/// end elf hdr

/// begin section header

// section header type
#define PRINT_SECTIONHEADER_TYPE(type)  \
  do                                    \
  {                                     \
    switch (type)                       \
    {                                   \
    case SHT_NULL:                      \
      printf("NULL");                   \
      break;                            \
    case SHT_PROGBITS:                  \
      printf("PROGBITS");               \
      break;                            \
    case SHT_SYMTAB:                    \
      printf("SYMTAB");                 \
      break;                            \
    case SHT_STRTAB:                    \
      printf("STRTAB");                 \
      break;                            \
    case SHT_RELA:                      \
      printf("RELA");                   \
      break;                            \
    case SHT_HASH:                      \
      printf("HASH");                   \
      break;                            \
    case SHT_DYNAMIC:                   \
      printf("DYNAMIC");                \
      break;                            \
    case SHT_NOTE:                      \
      printf("NOTE");                   \
      break;                            \
    case SHT_NOBITS:                    \
      printf("NOBITS");                 \
      break;                            \
    case SHT_REL:                       \
      printf("REL");                    \
      break;                            \
    case SHT_SHLIB:                     \
      printf("SHLIB");                  \
      break;                            \
    case SHT_DYNSYM:                    \
      printf("DYNSYM");                 \
      break;                            \
    default:                            \
      printf("unknow type %.2x", type); \
      break;                            \
    }                                   \
  } while (0)

// section header flags
#define PRINT_SECTIONHEADER_FLAGS(flags) \
  do                                     \
  {                                      \
    if (flags & SHF_WRITE)               \
    {                                    \
      printf("WRITE ");                  \
    }                                    \
    if (flags & SHF_ALLOC)               \
    {                                    \
      printf("ALLOC ");                  \
    }                                    \
    if (flags & SHF_EXECINSTR)           \
    {                                    \
      printf("EXECINSTR ");              \
    }                                    \
    if (flags & SHF_MERGE)               \
    {                                    \
      printf("MERGE ");                  \
    }                                    \
    if (flags & SHF_STRINGS)             \
    {                                    \
      printf("STRINGS ");                \
    }                                    \
    if (flags & SHF_INFO_LINK)           \
    {                                    \
      printf("INFO_LINK ");              \
    }                                    \
    if (flags & SHF_LINK_ORDER)          \
    {                                    \
      printf("LINK_ORDER ");             \
    }                                    \
    if (flags & SHF_OS_NONCONFORMING)    \
    {                                    \
      printf("OS_NONCONFORMING ");       \
    }                                    \
    if (flags & SHF_GROUP)               \
    {                                    \
      printf("GROUP ");                  \
    }                                    \
    if (flags & SHF_TLS)                 \
    {                                    \
      printf("TLS ");                    \
    }                                    \
    if (flags & SHF_COMPRESSED)          \
    {                                    \
      printf("COMPRESSED ");             \
    }                                    \
    if (flags & SHF_MASKOS)              \
    {                                    \
      printf("MASKOS ");                 \
    }                                    \
    if (flags & SHF_MASKPROC)            \
    {                                    \
      printf("MASKPROC ");               \
    }                                    \
    if (flags & SHF_ORDERED)             \
    {                                    \
      printf("ORDERED ");                \
    }                                    \
    if (flags & SHF_EXCLUDE)             \
    {                                    \
      printf("EXCLUDE ");                \
    }                                    \
  } while (0)

// section header
#define PRINT_SECTIONHEADER(shdr, BITS)                                            \
  do                                                                               \
  {                                                                                \
    printf("Name index: %lld\n", (unsigned long long)shdr.sh_name);                \
                                                                                   \
    printf("Type: ");                                                              \
    PRINT_SECTIONHEADER_TYPE(shdr.sh_type);                                        \
    printf("\n");                                                                  \
                                                                                   \
    printf("Flags: ");                                                             \
    PRINT_SECTIONHEADER_FLAGS(shdr.sh_flags);                                      \
    printf("\n");                                                                  \
                                                                                   \
    printf("Addr: 0x%.2llx\n", (long long unsigned int)shdr.sh_addr);              \
    printf("Section file offset: %lld\n", (long long unsigned int)shdr.sh_offset); \
    printf("Section size: %lld\n", (long long unsigned int)shdr.sh_size);          \
    printf("Link: %lld\n", (long long unsigned int)shdr.sh_link);                  \
    printf("Info: %lld\n", (long long unsigned int)shdr.sh_info);                  \
    printf("Align: %lld\n", (long long unsigned int)shdr.sh_addralign);            \
    printf("Entry size: %lld\n", (long long unsigned int)shdr.sh_entsize);         \
                                                                                   \
    if (shdr.sh_type == SHT_SYMTAB && shdr.sh_offset > 0 && shdr.sh_size > 0)      \
    {                                                                              \
      Elf##BITS##_Sym *symtbl = malloc(shdr.sh_size);                              \
      fseek(felf, shdr.sh_offset, SEEK_SET);                                       \
      fread(symtbl, shdr.sh_size, 1, felf);                                        \
      int symbolcounts = shdr.sh_size / shdr.sh_entsize;                           \
      printf("\n+symtbl counts: %d\n\n", symbolcounts);                            \
      for (int i = 0; i < symbolcounts; ++i)                                       \
      {                                                                            \
        printf("Symbol %2d:\n", i);                                                \
        PRINT_SYM(symtbl[i], BITS);                                                \
      }                                                                            \
                                                                                   \
      free(symtbl);                                                                \
      symtbl = NULL;                                                               \
    }                                                                              \
  } while (0)

/// end section header

/// begin symbol table

#define PRINT_SYM_INFOBIND(bind)        \
  do                                    \
  {                                     \
    switch (bind)                       \
    {                                   \
    case STB_LOCAL:                     \
      printf("LOCAL");                  \
      break;                            \
    case STB_GLOBAL:                    \
      printf("GLOBAL");                 \
      break;                            \
    case STB_WEAK:                      \
      printf("WEAK");                   \
      break;                            \
    default:                            \
      printf("unknow bind %.2x", bind); \
      break;                            \
    }                                   \
  } while (0)

#define PRINT_SYM_INFOTYPE(type)        \
  do                                    \
  {                                     \
    switch (type)                       \
    {                                   \
    case STT_NOTYPE:                    \
      printf("NOTYPE");                 \
      break;                            \
    case STT_OBJECT:                    \
      printf("OBJECT");                 \
      break;                            \
    case STT_FUNC:                      \
      printf("FUNC");                   \
      break;                            \
    case STT_SECTION:                   \
      printf("SECTION");                \
      break;                            \
    case STT_FILE:                      \
      printf("FILE");                   \
      break;                            \
    case STT_COMMON:                    \
      printf("COMMON");                 \
      break;                            \
    case STT_TLS:                       \
      printf("TLS");                    \
      break;                            \
    default:                            \
      printf("unknow type %.2x", type); \
      break;                            \
    }                                   \
  } while (0)

#define PRINT_SYM(sym, BITS)                                          \
  do                                                                  \
  {                                                                   \
    printf("\tName index: %lld\n", (unsigned long long)sym.st_name);  \
    printf("\tValue: 0x%.2llx\n", (unsigned long long)sym.st_value);  \
    printf("\tSize: %lld\n", (unsigned long long)sym.st_size);        \
                                                                      \
    printf("\tInfo bind: ");                                          \
    PRINT_SYM_INFOBIND(ELF##BITS##_ST_BIND(sym.st_info));             \
    printf("\n");                                                     \
                                                                      \
    printf("\tInfo type: ");                                          \
    PRINT_SYM_INFOTYPE(ELF##BITS##_ST_TYPE(sym.st_info));             \
    printf("\n");                                                     \
                                                                      \
    printf("\tOthers: %lld\n", (unsigned long long)sym.st_other);     \
    printf("\tSection index: ");                                      \
    if (sym.st_shndx == SHN_ABS)                                      \
    {                                                                 \
      printf("ABS(0x%.2llx)\n", (unsigned long long)sym.st_shndx);    \
    }                                                                 \
    else if (sym.st_shndx == SHN_COMMON)                              \
    {                                                                 \
      printf("COMMON(0x%.2llx)\n", (unsigned long long)sym.st_shndx); \
    }                                                                 \
    else                                                              \
    {                                                                 \
      printf("%lld\n", (unsigned long long)sym.st_shndx);             \
    }                                                                 \
    printf("\n");                                                     \
  } while (0)

/// end symbol table

#define PRINT_SECTIONHEADER_CHECK(elfhdr, felf, BITS)          \
  do                                                           \
  {                                                            \
    if (elfhdr.e_shoff > 0 && elfhdr.e_shnum > 0)              \
    {                                                          \
      printf("\nSection headers:\n\n");                        \
      int size = elfhdr.e_shentsize * elfhdr.e_shnum;          \
      Elf##BITS##_Shdr *elfshr = malloc(size);                 \
      fseek(felf, elfhdr.e_shoff, SEEK_SET);                   \
      fread(elfshr, elfhdr.e_shentsize, elfhdr.e_shnum, felf); \
      for (int i = 0; i < elfhdr.e_shnum; ++i)                 \
      {                                                        \
        printf("Section %2d:\n", i);                           \
        PRINT_SECTIONHEADER(elfshr[i], BITS);                  \
        printf("\n");                                          \
      }                                                        \
                                                               \
      free(elfshr);                                            \
      elfshr = NULL;                                           \
    }                                                          \
  } while (0)

#define PRINT_ELFHEADER_CHECK(felf, BITS)          \
  do                                               \
  {                                                \
    Elf##BITS##_Ehdr elfhdr = {0};                 \
    fread(&elfhdr, sizeof(elfhdr), 1, felf);       \
    PRINT_ELFHEADER(elfhdr);                       \
                                                   \
    PRINT_SECTIONHEADER_CHECK(elfhdr, felf, BITS); \
  } while (0)

extern char *__executable_start;

int main(int argc, char *argv[])
{
  PRINT_SYM_ADDR(__executable_start);

  printf("Usage : thisfile elffile\n");
  if (argc < 2)
  {
    EXIT(1);
  }

  FILE *felf = fopen(argv[1], "rb");
  if (felf == NULL)
  {
    EXIT(1);
  }
  printf("open elffile %s succeed\n", argv[1]);

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