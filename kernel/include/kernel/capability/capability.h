#ifndef KERNEL_CAPABILITY_H
#define KERNEL_CAPABILITY_H

#include <kernel/utils/bitmap.h>
#include <stdbool.h>
#include <stddef.h>

enum capability_type
{
    CAPABILITY_TYPE_NONE = 0,
    CAPABILITY_TYPE_IPC_ENDPOINT,
    CAPABILITY_TYPE_IPC_REPLY,
    CAPABILITY_TYPE_UNTYPED_FRAME,
};

enum capability_permission
{
    CAPABILITY_PERMISSION_READ = (1 << 0),
    CAPABILITY_PERMISSION_WRITE = (1 << 1),
};

struct capability
{
    enum capability_type type;
    enum capability_permission permissions;
    void *object;
};

#define CAPABILITY_NODE_RESERVED_SLOT_IPC_REPLY (1)

#define CAPABILITY_NODE_SLOTS (256)
#define CAPABILITY_NODE_USED_SLOTS_ELEMENTS (BITMAP_ELEMENTS(CAPABILITY_NODE_SLOTS))
struct capability_node
{
    struct capability slots[CAPABILITY_NODE_SLOTS];
    bitmap_t used_slots[CAPABILITY_NODE_USED_SLOTS_ELEMENTS];
};

size_t capability_mint(struct capability_node *node, enum capability_type type, enum capability_permission permissions, void *object);
void capability_burn(struct capability_node *node, size_t index);

static inline bool capability_check(struct capability *capability, enum capability_type type, enum capability_permission permissions)
{
    return (capability->type == type) && ((capability->permissions & permissions) == permissions);
}

static inline void *capability_object(struct capability *capability)
{
    return capability->object;
}

void capability_node_init(struct capability_node *node);

static inline struct capability *capability_node_get(struct capability_node *capability_node, size_t index)
{
    return &capability_node->slots[index];
}

#endif
