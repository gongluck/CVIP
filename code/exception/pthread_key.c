

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>

#define PTHREAD_COUNT 2

pthread_key_t key;
typedef void *(*thread_cb)(void *);

struct pair
{
	int x;
	int y;
};

void *thread1_proc(void *arg)
{

	struct pair p = {1, 2};
	int i = 0;

	while (++i < 100)
	{
		p.x++;
		p.y++;
		pthread_setspecific(key, &p);

		struct pair *res = (struct pair *)pthread_getspecific(key);
		printf("x: %d, y: %d\n", res->x, res->y);
	}
}

void *thread2_proc(void *arg)
{

	int i = 0;

	while (++i < 100)
	{
		pthread_setspecific(key, &i);

		int *res = (int *)pthread_getspecific(key);
		printf("res: %d\n", *res);
	}
}

int main(int argc, char *argv[])
{

	pthread_key_create(&key, NULL);

	pthread_t thread_id[PTHREAD_COUNT] = {0};
	thread_cb thread_proc[PTHREAD_COUNT] = {
		thread1_proc,
		thread2_proc};

	int i = 0;
	for (i = 0; i < PTHREAD_COUNT; i++)
	{
		pthread_create(&thread_id[i], NULL, thread_proc[i], NULL);
	}

	for (i = 0; i < PTHREAD_COUNT; i++)
	{
		pthread_join(thread_id[i], NULL);
	}

	pthread_key_delete(key)
}
