#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H

#include <stdbool.h>
#include <kernel/arch/interrupt.h>
#include <kernel/capability/capability.h>
#include <kernel/process/process.h>
#include <kernel/types/ipc.h>
#include <kernel/types/thread.h>
#include <arch/thread.h>

typedef arch_thread_context_t thread_context_t;

static inline void thread_context_switch(thread_context_t *current, thread_context_t *next) {
     arch_thread_context_switch(current, next);
}

enum thread_state {
    THREAD_STATE_RUNNING,
    THREAD_STATE_BLOCKED,
};

struct thread_ipc_context {
    struct ipc_message message;
};

struct thread {
    arch_thread_context_t arch_context;
    struct thread_ipc_context ipc_context;
    struct process *process;
    enum thread_state state;

    struct thread *queue_next;
    struct thread *queue_prev;
};

struct thread_queue {
    struct thread *head;
    struct thread *tail;
};

void thread_set_current(struct thread *thread);
void thread_init(struct thread *thread, struct process *process, thread_entry_t entry, int argc, char **argv);
void thread_switch(struct thread *next_thread);
struct thread *thread_current(void);

static inline struct capability* thread_capability(struct thread *thread, size_t index) {
    return process_capability(thread->process, index);
}

static inline struct capability_node* thread_capability_node(struct thread *thread) {
    return process_capability_node(thread->process);
} 


void thread_queue_enqueue(struct thread_queue *queue, struct thread *thread);
struct thread *thread_queue_dequeue(struct thread_queue *queue);
void thread_queue_remove(struct thread_queue *queue, struct thread *thread);
static inline bool thread_queue_empty(struct thread_queue *queue) {
    return queue->head == NULL;
}

static inline void thread_set_running(struct thread *thread) {
    thread->state = THREAD_STATE_RUNNING;
}
static inline void thread_set_blocked(struct thread *thread) {
    thread->state = THREAD_STATE_BLOCKED;
}

#endif
