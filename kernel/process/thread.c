#include <kernel/lib/stdio.h>
#include <kernel/memory/vm.h>
#include <kernel/process/process.h>
#include <kernel/process/thread.h>

static struct thread *current_thread = NULL;

void thread_set_current(struct thread *thread) { current_thread = thread; }

void thread_init(struct thread *thread, uintptr_t ustack_base_paddr,
                 uintptr_t ustack_table_paddr, struct process *process,
                 thread_entry_t entry, size_t arg_size, void *arg) {
    vm_map_switch(&process->vm_map);
    arch_thread_init(&thread->arch_context, ustack_base_paddr,
                     ustack_table_paddr, entry, arg_size, arg);
    thread->queue_next = NULL;
    thread->queue_prev = NULL;
    thread->process = process;
    thread->state = THREAD_STATE_RUNNING;
    // [TODO] init IPC context
}

void thread_switch(struct thread *next_thread) {
    if (current_thread == next_thread) {
        return;
    }
    if (current_thread == NULL) {
        vm_map_switch(&next_thread->process->vm_map);
        arch_thread_context_switch(NULL, &next_thread->arch_context);
        return;
    }

    if (current_thread->process != next_thread->process) {
        vm_map_switch(&next_thread->process->vm_map);
    }

    arch_thread_context_switch(&current_thread->arch_context,
                               &next_thread->arch_context);
}

void thread_block(struct thread *thread) {
    thread->state = THREAD_STATE_BLOCKED;
}

struct thread *thread_current(void) { return current_thread; }

void thread_queue_enqueue(struct thread_queue *queue, struct thread *thread) {
    if (thread == NULL) {
        return;
    }
    thread->queue_next = NULL;
    if (queue->tail != NULL) {
        queue->tail->queue_next = thread;
        thread->queue_prev = queue->tail;
    } else {
        queue->head = thread;
        thread->queue_prev = NULL;
    }
    queue->tail = thread;
}

struct thread *thread_queue_dequeue(struct thread_queue *queue) {
    struct thread *thread = queue->head;
    if (thread != NULL) {
        queue->head = thread->queue_next;
        if (queue->head != NULL) {
            queue->head->queue_prev = NULL;
        } else {
            queue->tail = NULL;
        }
        thread->queue_next = NULL;
    }
    return thread;
}

void thread_queue_remove(struct thread_queue *queue, struct thread *thread) {
    if (thread->queue_prev != NULL) {
        thread->queue_prev->queue_next = thread->queue_next;
    } else {
        queue->head = thread->queue_next;
    }

    if (thread->queue_next != NULL) {
        thread->queue_next->queue_prev = thread->queue_prev;
    } else {
        queue->tail = thread->queue_prev;
    }

    thread->queue_next = NULL;
    thread->queue_prev = NULL;
}
