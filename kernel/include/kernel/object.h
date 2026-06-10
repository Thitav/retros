#ifndef KERNEL_OBJECT_H
#define KERNEL_OBJECT_H

#include <stddef.h>
#include <kernel/process/ipc.h>

enum kernel_object_type {
    KERNEL_OBJECT_TYPE_IPC_ENDPOINT = 0,

    MAX_KERNEL_OBJECT_TYPE,
};

static const size_t kernel_object_size[MAX_KERNEL_OBJECT_TYPE] = {
    [KERNEL_OBJECT_TYPE_IPC_ENDPOINT] = sizeof(struct ipc_endpoint),
};

#endif
