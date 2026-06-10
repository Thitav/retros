#include <kernel/syscall.h>
#include "ipc.h"
#include <kernel/process/ipc.h>
#include <kernel/process/thread.h>
#include "error.h"

static struct ipc_endpoint *ipc_endpoint_from_capability(struct capability *capability, enum capability_permission required_permissions) {
    if (!capability_check(capability, CAPABILITY_TYPE_IPC_ENDPOINT, required_permissions)) {
        return NULL;
    }
    return (struct ipc_endpoint *)capability_object(capability);
}

static struct thread *ipc_consume_reply_cap(struct thread *current_thread) {
    struct capability *reply_capability = thread_capability(current_thread, CAPABILITY_NODE_RESERVED_SLOT_IPC_REPLY);
    if (!capability_check(reply_capability, CAPABILITY_TYPE_IPC_REPLY, CAPABILITY_PERMISSION_WRITE)) {
        return NULL;
    }
    struct thread *reply_thread = (struct thread *)capability_object(reply_capability);
    capability_burn(thread_capability_node(current_thread), CAPABILITY_NODE_RESERVED_SLOT_IPC_REPLY);
    return reply_thread;
}

void syscall_ipc_send(struct syscall_args *args) {
    struct thread *current_thread = thread_current();
    struct capability *capability = thread_capability(current_thread, args->arg0);
    struct ipc_endpoint *ipc_endpoint = ipc_endpoint_from_capability(capability, CAPABILITY_PERMISSION_WRITE);
    if (ipc_endpoint == NULL) {
        args->ret = SYSCALL_ERROR_INVALID_CAPABILITY;
        return;
    }

    struct ipc_message *message = (struct ipc_message *)args->arg1;
    ipc_send(ipc_endpoint, current_thread, message);
    args->ret = SYSCALL_ERROR_SUCCESS;
}

void syscall_ipc_receive(struct syscall_args *args) {
    struct thread *current_thread = thread_current();
    struct capability *capability = thread_capability(current_thread, args->arg0);
    struct ipc_endpoint *ipc_endpoint = ipc_endpoint_from_capability(capability, CAPABILITY_PERMISSION_READ);
    if (ipc_endpoint == NULL) {
        args->ret = SYSCALL_ERROR_INVALID_CAPABILITY;
        return;
    }

    ipc_receive(ipc_endpoint, current_thread);

    // [TODO] validate address
    struct ipc_message *message_dest = (struct ipc_message *)args->arg1;
    memcpy(message_dest, &current_thread->ipc_context.message, sizeof(struct ipc_message));
    args->ret = SYSCALL_ERROR_SUCCESS;
}

void syscall_ipc_function(struct syscall_args *args) {
    struct thread *current_thread = thread_current();
    struct capability *capability = thread_capability(current_thread, args->arg0);
    struct ipc_endpoint *ipc_endpoint = ipc_endpoint_from_capability(capability, CAPABILITY_PERMISSION_READ);
    if (ipc_endpoint == NULL) {
        args->ret = SYSCALL_ERROR_INVALID_CAPABILITY;
        return;
    }

    ipc_function(ipc_endpoint, current_thread);
    struct ipc_message *message_dest = (struct ipc_message *)args->arg2;
    memcpy(message_dest, &current_thread->ipc_context.message, sizeof(struct ipc_message));
    args->ret = SYSCALL_ERROR_SUCCESS;
}

void syscall_ipc_call(struct syscall_args *args) {
    struct thread *current_thread = thread_current();
    struct capability *capability = thread_capability(current_thread, args->arg0);
    struct ipc_endpoint *ipc_endpoint = ipc_endpoint_from_capability(capability, CAPABILITY_PERMISSION_WRITE);
    if (ipc_endpoint == NULL) {
        args->ret = SYSCALL_ERROR_INVALID_CAPABILITY;
        return;
    }

    struct ipc_message *message = (struct ipc_message *)args->arg1;
    ipc_call(ipc_endpoint, current_thread, message);
    
    struct ipc_message *message_dest = (struct ipc_message *)args->arg2;
    memcpy(message_dest, &current_thread->ipc_context.message, sizeof(struct ipc_message));
    args->ret = SYSCALL_ERROR_SUCCESS;
}

void syscall_ipc_return(struct syscall_args *args) {
    struct thread *current_thread = thread_current();
    struct thread *caller_thread = ipc_consume_reply_cap(current_thread);
    if (caller_thread == NULL) {
        args->ret = SYSCALL_ERROR_INVALID_CAPABILITY;
        return;
    }

    struct ipc_message *message = (struct ipc_message *)args->arg1;
    ipc_return(current_thread, caller_thread, message);
    args->ret = SYSCALL_ERROR_SUCCESS;
}
