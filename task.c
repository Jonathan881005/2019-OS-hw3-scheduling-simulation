#include "task.h"
#include "resource.h"
#include <ucontext.h>
#include <stdio.h>

status_type activate_task(task_type id)
{
    status_type result = STATUS_OK;

    //printf("T%d state = %d\n",id,task_state[id]);
    if(task_state[id]==SUSPENDED)
    {
        task_state[id] = READY;
        t[cur]=task_const[id];
        cur++;
        climb(t);
        //for (int i = 0; i < cur; i++)
        //printf("#task %d, id = %d, priority = %d\n", t[i].id, t[i].id, t[i].static_priority);

    }
    else
        result = STATUS_ERROR;

    while(t[0].static_priority > pri_running)
    {
        task_priority_type pri_temp = pri_running;      //暫存Running的 task priority
        int task_temp = task_running;                   //暫存Running的 task id
        task_state[task_running] = READY;           //RUNNING -> READY

        task_running = t[0].id;
        pri_running = t[0].static_priority;
        task_state[task_running] = RUNNING;             //SUSPENDED -> RUNNING

        t[0].id = task_temp;
        t[0].static_priority = pri_temp;

        task_first = task_temp;

        context_test(task_running);

        task_state[task_running] = SUSPENDED;

        task_running = t[0].id;
        pri_running = t[0].static_priority;
        task_state[task_running] = RUNNING;

        swap(&t[0],&t[cur-1]);
        cur--;

        climb(t);
    }


    return result;
}

status_type terminate_task(void)
{

    status_type result = STATUS_OK;
    for(int i = 0; i < RESOURCES_COUNT; i++)
    {
        if(resource_use[i]==task_running)
            result = STATUS_ERROR;
    }

    if(result == STATUS_OK)
    {
        task_state[task_running] = SUSPENDED;
        pri_running = 0;
        task_running = 0;
        //cur--;
    }

    return result;
}

void context_test(task_type id)
{
    char stack[1024*128];
    ucontext_t child,m;

    getcontext(&child);
    child.uc_stack.ss_sp = stack;//指定stack空間
    child.uc_stack.ss_size = sizeof(stack);//指定stack空間大小
    child.uc_stack.ss_flags = 0;
    child.uc_link = &m;//設定後繼context

    makecontext(&child, task_const[id].entry,0);//修改context指向func1函式

    swapcontext(&m,&child);//切換到child，保存當前context到main
}


/* sort by priority */
void climb (task_const_type t[])
{
    for (int i = 0; i < cur-1; i++)
    {
        for (int j = i+1; j < cur; j++)
        {
            if (t[i].static_priority<t[j].static_priority)
                swap (&t[i],&t[j]);
        }
    }
    task_first = t[0].id;
}


/* swap two task */
void swap (task_const_type *a, task_const_type *b)
{
    task_const_type tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
