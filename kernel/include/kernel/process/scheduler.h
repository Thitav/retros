#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <kernel/process/thread.h>

void scheduler_next(void);
void scheduler_init(void);
void scheduler_start(void);
void scheduler_enqueue(struct thread *thread);
// void scheduler_block_thread(struct thread *thread);

#endif
