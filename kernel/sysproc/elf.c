#include "elf.h"
#include <kernel/lib/string.h>
#include <kernel/lib/stdio.h>

typedef uint16_t Elf32_Half; // Unsigned half int
typedef uint32_t Elf32_Off;  // Unsigned offset
typedef uint32_t Elf32_Addr; // Unsigned address
typedef uint32_t Elf32_Word; // Unsigned int
typedef int32_t Elf32_Sword; // Signed int

#define EI_NIDENT 16

typedef struct
{
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Ehdr;

enum Elf_Ident
{
    EI_MAG0 = 0,       // 0x7F
    EI_MAG1 = 1,       // 'E'
    EI_MAG2 = 2,       // 'L'
    EI_MAG3 = 3,       // 'F'
    EI_CLASS = 4,      // Architecture (32/64)
    EI_DATA = 5,       // Byte Order
    EI_VERSION = 6,    // ELF Version
    EI_OSABI = 7,      // OS Specific
    EI_ABIVERSION = 8, // OS Specific
    EI_PAD = 9         // Padding
};

#define ELFMAG0 0x7F // e_ident[EI_MAG0]
#define ELFMAG1 'E'  // e_ident[EI_MAG1]
#define ELFMAG2 'L'  // e_ident[EI_MAG2]
#define ELFMAG3 'F'  // e_ident[EI_MAG3]

#define ELFDATA2LSB (1) // Little Endian
#define ELFCLASS32 (1)  // 32-bit Architecture

enum Elf_Type
{
    ET_NONE = 0, // Unkown Type
    ET_REL = 1,  // Relocatable File
    ET_EXEC = 2  // Executable File
};

#define EM_386 (3)     // x86 Machine Type
#define EV_CURRENT (1) // ELF Current Version

typedef struct {
	Elf32_Word		p_type;
	Elf32_Off		p_offset;
	Elf32_Addr		p_vaddr;
	Elf32_Addr		p_paddr;
	Elf32_Word		p_filesz;
	Elf32_Word		p_memsz;
	Elf32_Word		p_flags;
	Elf32_Word		p_align;
} Elf32_Phdr;

static bool elf32_ehdr_check_file(Elf32_Ehdr *hdr)
{
    if (!hdr)
        return false;
    if (hdr->e_ident[EI_MAG0] != ELFMAG0)
    {
        // ERROR("ELF Header EI_MAG0 incorrect.\n");
        return false;
    }
    if (hdr->e_ident[EI_MAG1] != ELFMAG1)
    {
        // ERROR("ELF Header EI_MAG1 incorrect.\n");
        return false;
    }
    if (hdr->e_ident[EI_MAG2] != ELFMAG2)
    {
        // ERROR("ELF Header EI_MAG2 incorrect.\n");
        return false;
    }
    if (hdr->e_ident[EI_MAG3] != ELFMAG3)
    {
        // ERROR("ELF Header EI_MAG3 incorrect.\n");
        return false;
    }
    return true;
}

static bool elf32_ehdr_check_supported(Elf32_Ehdr *hdr)
{
    if (hdr->e_ident[EI_CLASS] != ELFCLASS32)
    {
        // ERROR("Unsupported ELF File Class.\n");
        return false;
    }
    if (hdr->e_ident[EI_DATA] != ELFDATA2LSB)
    {
        // ERROR("Unsupported ELF File byte order.\n");
        return false;
    }
    if (hdr->e_machine != EM_386)
    {
        // ERROR("Unsupported ELF File target.\n");
        return false;
    }
    if (hdr->e_ident[EI_VERSION] != EV_CURRENT)
    {
        // ERROR("Unsupported ELF File version.\n");
        return false;
    }
    if (hdr->e_type != ET_EXEC)
    {
        // ERROR("Unsupported ELF File type.\n");
        return false;
    }
    return true;
}

static void elf32_load_segment(Elf32_Phdr *phdr, uintptr_t base_paddr) {
    Elf32_Addr vaddr = phdr->p_vaddr;
    Elf32_Addr paddr = base_paddr + phdr->p_offset;
    Elf32_Word mem_size = phdr->p_memsz;
    Elf32_Word file_size = phdr->p_filesz;
    Elf32_Word file_offset = 0;

    size_t total_pages = (mem_size + VM_PAGE_SIZE - 1) / VM_PAGE_SIZE;
    for (size_t i=0; i < total_pages; i++) {
        uintptr_t pm_page = pm_alloc(1);
        vm_mapping_mmap(NULL, vaddr, pm_page);

        if (file_offset < file_size) {
            Elf32_Word cpy_size;
            if (file_size - file_offset < VM_PAGE_SIZE) {
                cpy_size = file_size - file_offset;
                memset((void *)(vaddr + cpy_size), 0, VM_PAGE_SIZE - cpy_size);
            } else {
                cpy_size = VM_PAGE_SIZE;
            }

            memcpy((void *)vaddr, pm_kmap_vaddr(paddr + file_offset), cpy_size);
            file_offset += cpy_size;
        } else {
            memset((void *)vaddr, 0, VM_PAGE_SIZE);
        }

        vaddr += VM_PAGE_SIZE;
    }
}

#define PT_LOAD (1)
void *elf32_load(uintptr_t paddr)
{
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)pm_kmap_vaddr(paddr);

    if (!elf32_ehdr_check_file(ehdr)) 
    {
        return NULL;
    }
    if (!elf32_ehdr_check_supported(ehdr))
    {
        return NULL;
    }

    Elf32_Phdr *phdrs = (Elf32_Phdr *)pm_kmap_vaddr(paddr + ehdr->e_phoff);

    for (Elf32_Half i=0; i < ehdr->e_phnum; i++) {
        Elf32_Phdr *phdr = &phdrs[i];

        if (phdr->p_type != PT_LOAD) {
            continue;
        }

        elf32_load_segment(phdr, paddr);
    }

    return ehdr->e_entry;
}
