#include "resource.h"
#include "stdio.h"
#include "task.h"
#include <stddef.h>
//ready queue


int main()
{
    /****************** Initial config **********************/
    for (int i = 0; i < TASKS_COUNT; i++)
    {
        task_state[i] = SUSPENDED;
        t[i].id = -1;
        t[i].static_priority = -1;
    }
    for (int i = 0; i < RESOURCES_COUNT; i++)
        resource_use[i] = -1;
    pri_running = -1;
    task_running = -1;


    /****************** Auto start to ready state **********************/
    cur=0;
    for (int i = 0; i < AUTO_START_TASKS_COUNT; i++)
    {
        int find = -1,now = 0;
        while (find==-1 && now<TASKS_COUNT)
        {
            if (task_const[now].id == auto_start_tasks_list[i])
            {
                find = now;
                //printf("find:%d = now:%d\n", find, now);
            }
            now++;
        }
        t[cur++] = task_const[find];
        task_state[find] = READY;
        climb(t);
    }

    /**see see***/
    //for (int i = 0; i < cur; i++)
    //printf("#task %d, id = %d, priority = %d\n", t[i].id, t[i].id, t[i].static_priority);


    //printf("111cur:=%d\n",cur);

    int j=6;
    while (cur!=-1)
    {
        task_running = t[0].id;
        pri_running = t[0].static_priority;
        swap (&t[0],&t[cur-1]);
        cur--;
        climb(t);
        //printf("get cur:=%d\n",cur);
        //for (int i = 0; i < cur; i++)
        //printf("#task %d, id = %d, priority = %d\n", t[i].id, t[i].id, t[i].static_priority);


        //printf("pri_running is %d\n",pri_running);
        task_const[task_running].entry();
        //printf("finish cur:=%d\n",cur);
        //for (int i = 0; i < cur; i++)
        //printf("#task %d, id = %d, priority = %d\n", t[i].id, t[i].id, t[i].static_priority);

        climb(t);
        j--;
        if(j==0)
            break;
    };

    return 0;
}

