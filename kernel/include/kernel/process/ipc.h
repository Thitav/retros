#ifndef KERNEL_PROCESS_IPC_H
#define KERNEL_PROCESS_IPC_H

#include <stddef.h>
#include <kernel/process/thread.h>
#include <kernel/capability/capability.h>
#include <kernel/types/ipc.h>

struct ipc_endpoint {
    struct thread_queue sender_queue;
    struct thread_queue receiver_queue;
    struct thread_queue caller_queue;
    struct thread_queue callee_queue;
};

void ipc_endpoint_init(struct ipc_endpoint *endpoint);

void ipc_send(struct ipc_endpoint *endpoint, struct thread *sender_thread, struct ipc_message *message);
void ipc_receive(struct ipc_endpoint *endpoint, struct thread *receiver_thread);
void ipc_function(struct ipc_endpoint *endpoint, struct thread *callee_thread);
void ipc_call(struct ipc_endpoint *endpoint, struct thread *caller_thread, struct ipc_message *message);
void ipc_return(struct thread *callee_thread, struct thread *caller_thread, struct ipc_message *message);

#endif
