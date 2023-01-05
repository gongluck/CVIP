/*
 * @Author: gongluck
 * @Date: 2023-01-05 10:48:45
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-05 11:27:42
 */

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#define EXIT(code)                            \
  do                                          \
  {                                           \
    printf("exit int line : %d\n", __LINE__); \
    exit(code);                               \
  } while (0)

/// begin elf hdr

/// begin e_indent

// e_ident[EI_CLASS] 4
#define PRINTIDENTCLASS(class)            \
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
#define PRINTIDENTDATA(data)            \
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
#define PRINTIDENTVERSION(version)            \
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
#define PRINTIDENT(ident)                 \
  do                                      \
  {                                       \
    printf("Magic:");                     \
    for (int i = 0; i < EI_NIDENT; ++i)   \
    {                                     \
      printf(" %.2x", ident[i]);          \
    }                                     \
    printf("\n");                         \
                                          \
    printf("Class: ");                    \
    PRINTIDENTCLASS(ident[EI_CLASS]);     \
    printf("\n");                         \
                                          \
    printf("Data: ");                     \
    PRINTIDENTDATA(ident[EI_DATA]);       \
    printf("\n");                         \
                                          \
    printf("Version: ");                  \
    PRINTIDENTVERSION(ident[EI_VERSION]); \
    printf("\n");                         \
                                          \
  } while (0)

/// end e_indent

// e_type
#define PRINTELFHEADERTYPE(type)        \
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
#define PRINTELFHEADERMACHINE(machine)                                                                       \
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

#define PRINTELFHEADER(elfhdr)                                                                      \
  do                                                                                                \
  {                                                                                                 \
    PRINTIDENT(elfhdr.e_ident);                                                                     \
                                                                                                    \
    printf("Type: ");                                                                               \
    PRINTELFHEADERTYPE(elfhdr.e_type);                                                              \
    printf("\n");                                                                                   \
                                                                                                    \
    printf("Machine: ");                                                                            \
    PRINTELFHEADERMACHINE(elfhdr.e_machine);                                                        \
    printf("\n");                                                                                   \
                                                                                                    \
    printf("Version: 0x%x\n", elfhdr.e_version);                                                    \
    printf("Entry: 0x%llx\n", (long long unsigned int)elfhdr.e_entry);                              \
    printf("Program header offset: %lld\n", (long long unsigned int)elfhdr.e_phoff);                \
    printf("Section header offset: %lld\n", (long long unsigned int)elfhdr.e_shoff);                \
    printf("Flags: 0x%llx\n", (long long unsigned int)elfhdr.e_flags);                              \
    printf("Header size: %lld\n", (long long unsigned int)elfhdr.e_ehsize);                         \
    printf("Program header entry size: %lld\n", (long long unsigned int)elfhdr.e_phentsize);        \
    printf("Program header entry counts: %lld\n", (long long unsigned int)elfhdr.e_phnum);          \
    printf("Section header entry size: %lld\n", (long long unsigned int)elfhdr.e_shentsize);        \
    printf("Section header entry counts: %lld\n", (long long unsigned int)elfhdr.e_shnum);          \
    printf("Section header string table index: %lld\n", (long long unsigned int)elfhdr.e_shstrndx); \
  } while (0)

/// end elf hdr

/// begin section header

// section header type
#define PRINTSECTIONHEADERTYPE(type)    \
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
#define PRINTSECTIONHEADERFLAGS(flags) \
  do                                   \
  {                                    \
    if (flags & SHF_WRITE)             \
    {                                  \
      printf("WRITE ");                \
    }                                  \
    if (flags & SHF_ALLOC)             \
    {                                  \
      printf("ALLOC ");                \
    }                                  \
    if (flags & SHF_EXECINSTR)         \
    {                                  \
      printf("EXEC ");                 \
    }                                  \
  } while (0)

#define PRINTSECTIONHEADER(shdr)                                                   \
  do                                                                               \
  {                                                                                \
    printf("Name index: %lld\n", (unsigned long long)shdr.sh_name);                \
                                                                                   \
    printf("Type: ");                                                              \
    PRINTSECTIONHEADERTYPE(shdr.sh_type);                                          \
    printf("\n");                                                                  \
                                                                                   \
    printf("Flags: ");                                                             \
    PRINTSECTIONHEADERFLAGS(shdr.sh_flags);                                        \
    printf("\n");                                                                  \
                                                                                   \
    printf("Addr: 0x%llx\n", (long long unsigned int)shdr.sh_addr);                \
    printf("Section file offset: %lld\n", (long long unsigned int)shdr.sh_offset); \
    printf("Section size: %lld\n", (long long unsigned int)shdr.sh_size);          \
    printf("Link: %lld\n", (long long unsigned int)shdr.sh_link);                  \
    printf("Info: %lld\n", (long long unsigned int)shdr.sh_info);                  \
    printf("Align: %lld\n", (long long unsigned int)shdr.sh_addralign);            \
    printf("Entry size: %lld\n", (long long unsigned int)shdr.sh_entsize);         \
  } while (0)

/// end section header

int main(int argc, char *argv[])
{
  printf("Usage : thisfile elffile\n");
  if (argc < 2)
  {
    // EXIT(1);
  }

  FILE *felf = fopen("/mnt/e/code/CVIP/code/elf/SimpleSection.o", "rb");
  // FILE *felf = fopen(argv[1], "rb");
  if (felf == NULL)
  {
    EXIT(1);
  }
  printf("open elffile %s succeed\n", argv[1]);

  unsigned char e_ident[EI_NIDENT] = {0};
  fread(e_ident, EI_NIDENT, 1, felf);
  fseek(felf, 0, SEEK_SET);
  switch (e_ident[EI_CLASS])
  {
  case ELFCLASS32:
  {
    Elf32_Ehdr elfhdr = {0};
    fread(&elfhdr, sizeof(elfhdr), 1, felf);
    PRINTELFHEADER(elfhdr);
  }
  break;
  case ELFCLASS64:
  {
    Elf64_Ehdr elfhdr = {0};
    fread(&elfhdr, sizeof(elfhdr), 1, felf);
    PRINTELFHEADER(elfhdr);

    if (elfhdr.e_shoff != 0 && elfhdr.e_shnum != 0)
    {
      printf("\nSection headers:\n\n");
      int offset = elfhdr.e_shoff - sizeof(elfhdr);
      fseek(felf, offset, SEEK_CUR);
      int size = elfhdr.e_shentsize * elfhdr.e_shnum;
      Elf64_Shdr *elfshr = malloc(size);
      fread(elfshr, elfhdr.e_shentsize, elfhdr.e_shnum, felf);
      for (int i = 0; i < elfhdr.e_shnum; ++i)
      {
        PRINTSECTIONHEADER(elfshr[i]);
        printf("\n");
      }
    }
  }
  break;
  default:
    break;
  }

  fclose(felf);
  felf = NULL;
}