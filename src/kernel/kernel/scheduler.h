#ifndef _SCHEDULER_H
#define _SCHEDULER_H

struct thread {
    void (*target)(void);

    struct thread *prev;
    struct thread *next;
};

struct schedule_queue {
    struct thread *head;
    struct thread *tail;
};

void scheduler_add(struct thread *thread);
void scheduler_init(void);

#endif
