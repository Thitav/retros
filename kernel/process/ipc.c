#include <kernel/capability/capability.h>
#include <kernel/lib/string.h>
#include <kernel/process/ipc.h>
#include <kernel/process/scheduler.h>

void ipc_endpoint_init(struct ipc_endpoint *endpoint) {
    memset(endpoint, NULL, sizeof(struct ipc_endpoint));
}

void ipc_send(struct ipc_endpoint *endpoint, struct thread *sender_thread,
              struct ipc_message *message) {
    if (!thread_queue_empty(&endpoint->receiver_queue)) {
        struct thread *receiver_thread =
            thread_queue_dequeue(&endpoint->receiver_queue);

        memcpy(&receiver_thread->ipc_context.message, message,
               sizeof(struct ipc_message));

        scheduler_enqueue(sender_thread);
        thread_switch(receiver_thread);
    } else {
        memcpy(&sender_thread->ipc_context.message, message,
               sizeof(struct ipc_message));
        thread_queue_enqueue(&endpoint->sender_queue, sender_thread);
        scheduler_next();
    }
}

void ipc_receive(struct ipc_endpoint *endpoint,
                 struct thread *receiver_thread) {
    if (!thread_queue_empty(&endpoint->sender_queue)) {
        struct thread *sender_thread =
            thread_queue_dequeue(&endpoint->sender_queue);

        memcpy(&receiver_thread->ipc_context.message,
               &sender_thread->ipc_context.message, sizeof(struct ipc_message));

        scheduler_enqueue(sender_thread);
    } else {
        thread_queue_enqueue(&endpoint->receiver_queue, receiver_thread);
        scheduler_next();
    }
}

void ipc_function(struct ipc_endpoint *endpoint, struct thread *callee_thread) {
    if (!thread_queue_empty(&endpoint->caller_queue)) {
        struct thread *caller_thread =
            thread_queue_dequeue(&endpoint->caller_queue);

        memcpy(&callee_thread->ipc_context.message,
               &caller_thread->ipc_context.message, sizeof(struct ipc_message));

        // capability_mint(thread_capability_node(callee_thread),
        //                 CAPABILITY_TYPE_IPC_REPLY,
        //                 CAPABILITY_PERMISSION_WRITE, caller_thread);
        thread_switch(callee_thread);
    } else {
        thread_queue_enqueue(&endpoint->callee_queue, callee_thread);
        scheduler_next();
    }
}

void ipc_call(struct ipc_endpoint *endpoint, struct thread *caller_thread,
              struct ipc_message *message) {
    if (!thread_queue_empty(&endpoint->callee_queue)) {
        struct thread *callee_thread =
            thread_queue_dequeue(&endpoint->callee_queue);

        memcpy(&callee_thread->ipc_context.message, message,
               sizeof(struct ipc_message));

        // capability_mint(thread_capability_node(callee_thread),
        // CAPABILITY_TYPE_IPC_REPLY, CAPABILITY_PERMISSION_WRITE,
        // caller_thread);
        thread_switch(callee_thread);
    } else {
        memcpy(&caller_thread->ipc_context.message, message,
               sizeof(struct ipc_message));
        thread_queue_enqueue(&endpoint->caller_queue, caller_thread);
        scheduler_next();
    }
}

void ipc_return(struct thread *callee_thread, struct thread *caller_thread,
                struct ipc_message *message) {
    memcpy(&caller_thread->ipc_context.message, message,
           sizeof(struct ipc_message));
    scheduler_enqueue(callee_thread);
    thread_switch(caller_thread);
}
