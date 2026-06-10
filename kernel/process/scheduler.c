#include <stddef.h>
#include <kernel/arch/interrupt.h>
#include <kernel/arch/timer.h>
#include <kernel/lib/stdio.h>
#include <kernel/process/scheduler.h>
#include <kernel/process/thread.h>

static struct thread_queue scheduler_queue = { NULL };

static void scheduler_idle(void) {
    while (1) {}
}

static interrupt_context_t *scheduler_handler(interrupt_context_t *interrupt_context) {
    thread_queue_enqueue(&scheduler_queue, thread_current());
    scheduler_next();
    return interrupt_context;
}

void scheduler_next(void) {
    struct thread *next_thread = thread_queue_dequeue(&scheduler_queue);
    if (next_thread == NULL) {

        // [TODO] check this
        scheduler_next();
    }
    thread_switch(next_thread); 
}

void scheduler_enqueue(struct thread *thread) {
    arch_interrupt_disable();
    thread_queue_enqueue(&scheduler_queue, thread);
    arch_interrupt_enable();
}

void scheduler_init(void) {
    // thread_init(&scheduler_idle_thread, NULL, scheduler_idle_thread_loop, 0, NULL);

    arch_interrupt_set_handler(0, scheduler_handler);
    arch_timer_init();
}

void scheduler_start(void) {
    arch_timer_set(50);
}

// void scheduler_remove(struct thread *thread) {
//     thread_set_blocked(thread);
//     thread_queue_remove(&scheduler_queue, thread);
// }
