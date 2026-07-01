#include <kernel/capability/capability.h>
#include <kernel/error.h>
#include <kernel/lib/string.h>
#include <stddef.h>
#include <stdint.h>

void capability_mint(struct capability *capability, enum capability_type type,
                     enum capability_permission permissions, void *object,
                     void *metadata) {
    capability->type = type;
    capability->permissions = permissions;
    capability->object = object;
    capability->metadata = metadata;

    capability->parent = NULL;
    capability->child = NULL;
    capability->sibling = NULL;
}

void capability_mint_child(struct capability *parent_capability,
                           struct capability *child_capability,
                           enum capability_type type,
                           enum capability_permission permissions, void *object,
                           void *metadata) {
    child_capability->type = type;
    child_capability->permissions = permissions;
    child_capability->object = object;
    child_capability->metadata = metadata;

    child_capability->parent = parent_capability;
    child_capability->child = NULL;
    child_capability->sibling = parent_capability->child;
    parent_capability->child = child_capability;
}

void capability_burn(struct capability *capability) {
    struct capability *current_child = capability->child;
    struct capability *next_child;

    while (current_child != NULL) {
        next_child = current_child->sibling;
        capability_burn(current_child);
        current_child = next_child;
    }
    memset(capability, CAPABILITY_TYPE_NONE, sizeof(struct capability));
}

void capability_node_init(struct capability_node *node) {
    memset(node, 0, sizeof(struct capability_node));
    bitmap_set(&node->used_slots[0], CAPABILITY_NODE_RESERVED_SLOT_IPC_REPLY);
}

static size_t capability_node_alloc(struct capability_node *node,
                                    enum capability_type type) {
    size_t index;

    switch (type) {
    case CAPABILITY_TYPE_IPC_REPLY:
        index = CAPABILITY_NODE_RESERVED_SLOT_IPC_REPLY;
        break;

    default:
        index =
            bitmap_find(&node->used_slots[0],
                        CAPABILITY_NODE_USED_SLOTS_ELEMENTS, 1, 1, false, NULL);
        ASSERT(index != SIZE_MAX, return SIZE_MAX);
        bitmap_set(&node->used_slots[0], index);
    }

    return index;
}

size_t capability_node_mint(struct capability_node *node,
                            enum capability_type type,
                            enum capability_permission permissions,
                            void *object, void *metadata) {
    size_t index = capability_node_alloc(node, type);
    ASSERT(index != SIZE_MAX, return SIZE_MAX);
    capability_mint(&node->slots[index], type, permissions, object, metadata);
    return index;
}

size_t capability_node_mint_child(struct capability_node *parent_node,
                                  struct capability_node *child_node,
                                  size_t parent_index,
                                  enum capability_type type,
                                  enum capability_permission permissions,
                                  void *object, void *metadata) {
    size_t child_index = capability_node_alloc(child_node, type);
    ASSERT(child_index != SIZE_MAX, return SIZE_MAX);
    capability_mint_child(&parent_node->slots[parent_index],
                          &child_node->slots[child_index], type, permissions,
                          object, metadata);
    return child_index;
}

void capability_node_burn(struct capability_node *node, size_t index) {
    capability_burn(&node->slots[index]);
    bitmap_clear(&node->used_slots[0], index);
}
