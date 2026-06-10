#include "frame.h"
#include "error.h"
#include <kernel/capability/capability.h>
#include <kernel/capability/untyped_frame.h>
#include <kernel/object.h>
#include <kernel/process/thread.h>
#include <kernel/syscall.h>

static struct untyped_frame *untyped_frame_from_capability(struct capability *capability, enum capability_permission required_permissions)
{
    if (!capability_check(capability, CAPABILITY_TYPE_UNTYPED_FRAME, required_permissions))
    {
        return NULL;
    }
    return (struct untyped_frame *)capability_object(capability);
}

void syscall_untyped_frame_retype(struct syscall_args *args)
{
    if (args->arg1 >= MAX_KERNEL_OBJECT_TYPE)
    {
        // [TODO] error
        return;
    }

    struct thread *current_thread = thread_current();
    struct capability *capability = thread_capability(current_thread, args->arg0);
    struct untyped_frame *frame = untyped_frame_from_capability(capability, CAPABILITY_PERMISSION_WRITE);
    if (frame == NULL)
    {
        args->ret = SYSCALL_ERROR_INVALID_CAPABILITY;
        return;
    }

    enum kernel_object_type type = args->arg1;
    size_t object_size = kernel_object_size[type];
    void *ptr = untyped_frame_retype(frame, object_size);
    if (ptr == NULL)
    {
        // [TODO] error
        return;
    }

    switch (type)
    {
    case KERNEL_OBJECT_TYPE_IPC_ENDPOINT:
        ipc_endpoint_init((struct ipc_endpoint *)ptr);
        size_t capability_index = capability_mint(thread_capability_node(current_thread), CAPABILITY_TYPE_IPC_ENDPOINT,
                                                  CAPABILITY_PERMISSION_READ | CAPABILITY_PERMISSION_WRITE, ptr);
        args->ret = capability_index;
        break;
    default:
        // unreachable
        break;
    }
}
