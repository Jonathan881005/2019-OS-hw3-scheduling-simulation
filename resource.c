#include "resource.h"
#include <ucontext.h>
#include <stdio.h>



status_type get_resource(resource_type id)
{
    status_type result = STATUS_OK;
    if(resource_use[id] == -1)
    {
        pri_running = resources_priority[id];
        //up to ceiling priority
        //printf("after get r%d pri_running = %d\n",id,pri_running);
        resource_use[id] = task_running;
        //save occupier
    }
    else
        result = STATUS_ERROR;
    return result;
}

status_type release_resource(resource_type id)
{
    status_type result = STATUS_OK;

    if(resource_use[id] == task_running)
    {
        //printf("before realese pri_running = %d, pri_first = %d\n",pri_running,task_const[task_first].static_priority);

        pri_running = task_const[resource_use[id]].static_priority;
        //printf("here:%d\n",task_const[resource_use[id]].static_priority) ;
        resource_use[id] = -1;

        for(int i = 0; i < RESOURCES_COUNT ; i++)
        {
            if(resource_use[i] == task_running && resources_priority[i] > pri_running)
            {
                pri_running = resources_priority[i];
            }
        }
        //printf("after realese pri_running = %d, pri_first = %d\n",pri_running,task_const[task_first].static_priority);

        while(t[0].static_priority > pri_running)
        {
            //printf("T%d,will preempt T%d!!!\n",t[0].id,task_running);
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

    }
    else
        result = STATUS_ERROR;

    //printf("------result is %d\n------",result);
    return result;
}

