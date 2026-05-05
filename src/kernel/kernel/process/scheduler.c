#include <stddef.h>
#include <kernel/arch/interrupt.h>
#include <kernel/arch/timer.h>
#include <stdio.h>
#include <kernel/process/scheduler.h>

struct schedule_queue queue;

void _enqueue(struct schedule_queue *queue, struct thread *thread) {
    thread->next = NULL;
    if (queue->tail != NULL) {
        queue->tail->next = thread;
        thread->prev = queue->tail;
    } else {
        queue->head = thread;
        thread->prev = NULL;
    }

    queue->tail = thread;
}

struct thread *_dequeue(struct schedule_queue *queue) {
    struct thread *thread = queue->head;
    if (thread != NULL) {
        queue->head = thread->next;
        if (queue->head != NULL) {
            queue->head->prev = NULL;
        }
    }
    return thread;
}

arch_interrupt_context_t *scheduler_handler(arch_interrupt_context_t *context) {
    struct thread *thread = _dequeue(&queue);
    thread->target();
    _enqueue(&queue, thread);

    return context;
}

void scheduler_add(struct thread *thread) {
    _enqueue(&queue, thread);
}

void scheduler_init() {
    arch_interrupt_set_handler(0, scheduler_handler);
    arch_timer_init();
    arch_timer_set(50);
}
