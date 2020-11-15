/*
 * @Author: gongluck 
 * @Date: 2020-11-15 15:58:13 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-15 16:41:09
 */

// gcc threadpool.c -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

// 添加队列节点
#define LL_ADD(item, list) \
    do                     \
    {                      \
        item->prev = NULL; \
        item->next = list; \
        list = item;       \
    } while (0)

// 移除队列节点
#define LL_REMOVE(item, list)              \
    do                                     \
    {                                      \
        if (item->prev != NULL)            \
            item->prev->next = item->next; \
        if (item->next != NULL)            \
            item->next->prev = item->prev; \
        if (list == item)                  \
            list = item->next;             \
        item->prev = item->next = NULL;    \
    } while (0)

// 工作线程
typedef struct WORKER
{
    pthread_t thread;
    int terminate;
    struct WORKQUEUE *workqueue;
    struct WORKER *prev;
    struct WORKER *next;
} Worker;

// 工作任务
typedef struct JOB
{
    void (*job_function)(struct JOB *job);
    void *user_data;
    struct JOB *prev;
    struct JOB *next;
} Job;

// 工作调度
typedef struct WORKQUEUE
{
    struct WORKER *workers;
    struct JOB *waiting_jobs;
    pthread_mutex_t jobs_mtx;
    pthread_cond_t jobs_cond;
} WorkQueue;

typedef WorkQueue ThreadPool;

// 工作线程回调函数
static void *WorkerThread(void *ptr)
{
    Worker *worker = (Worker *)ptr;

    while (1)
    {
        pthread_mutex_lock(&worker->workqueue->jobs_mtx);

        while (worker->workqueue->waiting_jobs == NULL)
        {
            if (worker->terminate)
                break;
            pthread_cond_wait(&worker->workqueue->jobs_cond, &worker->workqueue->jobs_mtx);
        }

        if (worker->terminate)
        {
            pthread_mutex_unlock(&worker->workqueue->jobs_mtx);
            break;
        }

        Job *job = worker->workqueue->waiting_jobs;
        if (job != NULL)
        {
            LL_REMOVE(job, worker->workqueue->waiting_jobs);
        }

        pthread_mutex_unlock(&worker->workqueue->jobs_mtx);

        if (job == NULL)
            continue;

        job->job_function(job);
    }

    free(worker);
    pthread_exit(NULL);
}

// 创建线程池
int ThreadPoolCreate(ThreadPool *workqueue, int numWorkers)
{
    if (numWorkers < 1)
        numWorkers = 1;
    memset(workqueue, 0, sizeof(ThreadPool));

    pthread_mutex_init(&workqueue->jobs_mtx, NULL);
    pthread_cond_init(&workqueue->jobs_cond, NULL);

    for (int i = 0; i < numWorkers; i++)
    {
        Worker *worker = (Worker *)malloc(sizeof(Worker));
        if (worker == NULL)
        {
            perror("malloc");
            return 1;
        }

        memset(worker, 0, sizeof(Worker));
        worker->workqueue = workqueue;

        int ret = pthread_create(&worker->thread, NULL, WorkerThread, (void *)worker);
        if (ret)
        {
            perror("pthread_create");
            free(worker);
            return 1;
        }

        LL_ADD(worker, worker->workqueue->workers);
    }

    return 0;
}

// 终止线程池
void ThreadPoolShutdown(ThreadPool *workqueue)
{
    for (Worker *worker = workqueue->workers; worker != NULL; worker = worker->next)
    {
        worker->terminate = 1;
    }

    pthread_mutex_lock(&workqueue->jobs_mtx);

    workqueue->workers = NULL;
    workqueue->waiting_jobs = NULL;

    pthread_cond_broadcast(&workqueue->jobs_cond);

    pthread_mutex_unlock(&workqueue->jobs_mtx);
}

// 添加任务
void ThreadPoolQueue(ThreadPool *workqueue, Job *job)
{
    pthread_mutex_lock(&workqueue->jobs_mtx);

    LL_ADD(job, workqueue->waiting_jobs);

    pthread_cond_signal(&workqueue->jobs_cond);
    pthread_mutex_unlock(&workqueue->jobs_mtx);
}

void test_counter(Job *job)
{
    int index = *(int *)job->user_data;

    printf("index : %d, selfid : %lu\n", index, pthread_self());

    free(job->user_data);
    free(job);
}

int main()
{
    ThreadPool pool;

    ThreadPoolCreate(&pool, 80);

    for (int i = 0; i < 10000; i++)
    {
        Job *job = (Job *)malloc(sizeof(Job));
        if (job == NULL)
        {
            perror("malloc");
            exit(1);
        }

        job->job_function = test_counter;
        job->user_data = malloc(sizeof(int));
        *(int *)job->user_data = i;

        ThreadPoolQueue(&pool, job);
    }

    ThreadPoolShutdown(&pool);
    
    printf("end...\n");
}
