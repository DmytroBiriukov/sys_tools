#ifndef __FRK_HPP_INCLUDED__   // if x.h hasn't been included yet...
#define __FRK_HPP_INCLUDED__   //   #define this so the compiler knows it has been included

#include <unistd.h>
#include <stdio.h>

namespace sys_tools{
    
    struct processDescription{
        pid_t pid;
        pid_t ppid;
        pid_t pgrp;        
    };

    void forker(processDescription* child, unsigned short i, unsigned short n, void (*work_load)(char*, char*, void*), void* obj)
    {
        pid_t pid;

        if(i < n)
        {   sleep(1);
        
            if ((pid = fork()) < 0)
            {
                perror("fork");
            }
            else if (pid == 0)
            {
                //Child stuff here
                work_load("Child", "Hello from child!", obj);                                                
                (child + i)->pid = getpid();
                (child + i)->ppid = getppid();
                (child + i)->pgrp = getpgrp();
            }
            else if(pid > 0)
            {
                //Parent                                                               
                forker(child, i+1, n, work_load, obj);
            }
        }         
    }


    void execv_forker(processDescription* child, unsigned short i, unsigned short n, const char** path,  char** const *argv)
    {
        pid_t pid;
        if(i < n){   
            sleep(1);        
            if ((pid = fork()) < 0)
            {
                perror("fork");
            }
            else if (pid == 0)
            {                
                execv( *(path + i), *(argv + i));               
            }
            else if(pid > 0)
            {
                //Parent                                                               
                (child + i)->pid = getpid();
                (child + i)->ppid = getppid();
                (child + i)->pgrp = getpgrp();
                execv_forker(child, i+1, n, path, argv);
            }
        }         
    }
}

#endif