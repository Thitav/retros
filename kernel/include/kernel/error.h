#ifndef KERNEL_ERROR_H
#define KERNEL_ERROR_H

#define KERROR_PACK(module, code)                                              \
    (-(int)((((module) & 0xFF) << 8) | ((code) & 0xFF)))
#define KERROR_UNPACK_MODULE(error) (((-(int)(error)) >> 8) & 0xFF)
#define KERROR_UNPACK_CODE(error) ((-(int)(error)) & 0xFF)

#define KERROR_MODULE_VM (1)

typedef enum {
    KERROR_SUCCESS = 0,
    KERROR_VM_MISSING_TABLE = KERROR_PACK(KERROR_MODULE_VM, 1),
    KERROR_VM_TMP_MAP_IN_USE = KERROR_PACK(KERROR_MODULE_VM, 2),
    KERROR_VM_TMP_MAP_EMPTY = KERROR_PACK(KERROR_MODULE_VM, 3),
} kerror_t;

#define IS_ERROR(error) (((kerror_t)(error)) < 0)
#define IS_SUCCESS(error) (((kerror_t)(error)) >= 0)

#define ASSERT(condition, on_false)                                            \
    if (!(condition)) {                                                        \
        on_false;                                                              \
    }

#define ASSERT_SUCCESS(condition, on_false)                                    \
    ASSERT(IS_SUCCESS(condition), on_false)

void panic(char *message);

#endif
