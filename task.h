#ifndef TASK_H
#define TASK_H

#include "typedefine.h"
#include "config.h"

status_type activate_task(task_type id);
status_type terminate_task(void);

/*  State of every task */
int task_state[TASKS_COUNT];

/*  Ready queue */
task_const_type t[TASKS_COUNT];

/*  Resources occupier's id */
int resource_use[RESOURCES_COUNT];


void climb(task_const_type t[]);
void swap(task_const_type *a, task_const_type *b);

#endif /* TASK_H */