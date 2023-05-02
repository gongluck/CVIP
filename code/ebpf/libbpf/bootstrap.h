/*
 * @Author: gongluck
 * @Date: 2023-05-02 20:34:08
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-05-02 21:48:10
 */

#ifndef __BOOTSTRAP_H
#define __BOOTSTRAP_H

#define TASK_COMM_LEN 16
#define MAX_FILENAME_LEN 127

struct myevent
{
    int pid;
    int tgid;
    char comm[TASK_COMM_LEN];
    unsigned long long duration_ns;
    char filename[MAX_FILENAME_LEN];
    bool exit_event;
    unsigned exit_code;
};

#endif /* __BOOTSTRAP_H */