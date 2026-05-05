#include <kernel/memory/pmm.h>
#include <kernel/utils/bitmap.h>
#include <kernel/arch/vmm.h>
#include <kernel/boot.h>
#include <stdlib.h>
#include <stdio.h>

#define ADDRESS_TO_PAGE(addr) ((addr) / ARCH_VMM_PAGE_SIZE)
#define PAGE_TO_ADDRESS(page) ((page) * ARCH_VMM_PAGE_SIZE)
#define BITMAP_POS_TO_PAGE(index, offset) (((index) * BITMAP_BITS_PER_ELEMENT) + (offset))
#define PAGE_TO_BITMAP_INDEX(page) ((page) / BITMAP_BITS_PER_ELEMENT)
#define PAGE_TO_BITMAP_OFFSET(page) ((page) % BITMAP_BITS_PER_ELEMENT)

#define PAGES_BITMAP_LEN BITMAP_ELEMENTS(UINTPTR_MAX / ARCH_VMM_PAGE_SIZE)

struct pmm {
    bitmap_t used_pages[PAGES_BITMAP_LEN];
    size_t used_pages_start;
    size_t available_pages;
};

static struct pmm pmm = {.used_pages_start = SIZE_MAX, .available_pages = 0};

void _init_pages(size_t pages_count, bool used, size_t *current_bitmap_index, unsigned char *current_bitmap_offset)
{
    while (pages_count > 0)
    {
        uintptr_t bitmap_full_elements = PAGE_TO_BITMAP_INDEX(pages_count);
        if (current_bitmap_offset == 0 && bitmap_full_elements > 0)
        {
            for (uintptr_t j = 0; j < bitmap_full_elements; j++)
            {
                if (used)
                {
                    pmm.used_pages[*current_bitmap_index] = BITMAP_T_MAX;
                    pmm.available_pages -= BITMAP_BITS_PER_ELEMENT;
                }
                else
                {
                    pmm.used_pages[*current_bitmap_index] = 0;
                    pmm.available_pages += BITMAP_BITS_PER_ELEMENT;
                }
                (*current_bitmap_index)++;
                pages_count -= BITMAP_BITS_PER_ELEMENT;
            }
            if (pages_count == 0) {
                break;
            }
        }

        if (used)
        {
            bitmap_set_ex(pmm.used_pages, *current_bitmap_index, *current_bitmap_offset);
            pmm.available_pages++;
        }
        else
        {
            bitmap_clear_ex(pmm.used_pages, *current_bitmap_index, *current_bitmap_offset);
            pmm.available_pages--;
        }

        (*current_bitmap_offset)++;
        if (*current_bitmap_offset == BITMAP_BITS_PER_ELEMENT)
        {
            *current_bitmap_offset = 0;
            (*current_bitmap_index)++;
        }
        
        pages_count--;
    }
}

void pmm_init(struct boot_memory_map *memory_maps, size_t memory_maps_length)
{
    uintptr_t current_addr = 0;
    size_t current_bitmap_index = 0;
    unsigned char current_bitmap_offset = 0;

    for (size_t i = 0; i < memory_maps_length; i++)
    {
        struct boot_memory_map *memory_map = &memory_maps[i];
        char base_s[16], size_s[16];
        ltoa(memory_map->base, &base_s[0], 16);
        ltoa(memory_map->size, &size_s[0], 16);
        printf("mapping %s sized %s", base_s, size_s);

        if (memory_map->base != current_addr)
        {
            uintptr_t unmapped_memory = memory_map->base - current_addr;
            size_t unmapped_pages = ADDRESS_TO_PAGE(unmapped_memory + ARCH_VMM_PAGE_SIZE - 1);

            _init_pages(unmapped_pages, true, &current_bitmap_index, &current_bitmap_offset);
            current_addr += unmapped_pages * ARCH_VMM_PAGE_SIZE;
        }

        size_t pages_count = ADDRESS_TO_PAGE((memory_map->base + memory_map->size) - current_addr);
        if (memory_map->status == BOOT_MEMORY_MAP_STATUS_AVAILABLE) {
            if (pmm.used_pages_start == SIZE_MAX) {
                pmm.used_pages_start = current_bitmap_index;
            }
            printf(" available\n");
            _init_pages(pages_count, false, &current_bitmap_index, &current_bitmap_offset);
        } else {
            printf(" reserved\n");
            _init_pages(pages_count, true, &current_bitmap_index, &current_bitmap_offset);
        }

        current_addr += PAGE_TO_ADDRESS(pages_count);
    }

    if (current_bitmap_index < PAGES_BITMAP_LEN - 1 || current_bitmap_offset < BITMAP_BITS_PER_ELEMENT - 1) {
        size_t pages_count = BITMAP_POS_TO_PAGE(PAGES_BITMAP_LEN - current_bitmap_index - 1, BITMAP_BITS_PER_ELEMENT - current_bitmap_offset - 1);
        _init_pages(pages_count, true, &current_bitmap_index, &current_bitmap_offset);
    }
}

size_t pmm_available_pages(void) {
    return pmm.available_pages;
}

uintptr_t pmm_alloc(void) {
    if (pmm.available_pages == 0) {
        return 0;
    }

    for (size_t i = pmm.used_pages_start; i < PAGES_BITMAP_LEN; i++) {
        if (pmm.used_pages[i] == BITMAP_T_MAX) {
            continue;
        }

        unsigned char offset = bitmap_ctz(~pmm.used_pages[i]);
        bitmap_set_ex(pmm.used_pages, i, offset);
        pmm.available_pages--;
        return PAGE_TO_ADDRESS(BITMAP_POS_TO_PAGE(i, offset));
    }
    
    return 0;
}

void pmm_free(uintptr_t addr) {
    uintptr_t page = ADDRESS_TO_PAGE(addr);
    bitmap_clear_ex(pmm.used_pages, PAGE_TO_BITMAP_INDEX(page), PAGE_TO_BITMAP_OFFSET(page));
    pmm.available_pages++;
}
