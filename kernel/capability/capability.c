#include <kernel/capability/capability.h>
#include <stddef.h>
#include <kernel/lib/string.h>

void capability_node_init(struct capability_node *node) {
    memset(node, 0, sizeof(struct capability_node));
    bitmap_set(&node->used_slots, CAPABILITY_NODE_RESERVED_SLOT_IPC_REPLY);
}

size_t capability_mint(struct capability_node *node, enum capability_type type, enum capability_permission permissions, void *object)
{
    size_t index;

    switch (type)
    {
    case CAPABILITY_TYPE_IPC_REPLY:
        index = CAPABILITY_NODE_RESERVED_SLOT_IPC_REPLY;
        break;

    default:
        size_t i;
        for (i = 0; i < CAPABILITY_NODE_USED_SLOTS_ELEMENTS; i++)
        {
            if (node->used_slots[i] != BITMAP_T_MAX)
            {
                break;
            }
        }
        // [TODO] handle full node
        index = (i * BITMAP_BITS_PER_ELEMENT) + bitmap_ctz(~node->used_slots[i]);
        bitmap_set(&node->used_slots, index);
    }

    node->slots[index].type = type;
    node->slots[index].permissions = permissions;
    node->slots[index].object = object;
    return index;
}

void capability_burn(struct capability_node *node, size_t index) {
    memset(&node->slots[index], CAPABILITY_TYPE_NONE, sizeof(struct capability));
    bitmap_clear(&node->used_slots, index);
}
