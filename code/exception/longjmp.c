

#include <setjmp.h>
#include <stdio.h>

jmp_buf env;
int count = 0;

void sub_func(int idx)
{
	printf("sub_func --> idx:%d\n", idx);
	longjmp(env, idx);
}

int main(int argc, char *argv[])
{
	int idx = 0;

	count = setjmp(env);
	if (count == 0)
	{
		printf("count:%d\n", count);
		sub_func(++idx);
	}
	else if (count == 1)
	{
		printf("count:%d\n", count);
		sub_func(++idx);
	}
	else if (count == 2)
	{
		printf("count:%d\n", count);
		sub_func(++idx);
	}
	else if (count == 3)
	{
		printf("count:%d\n", count);
		sub_func(++idx);
	}
	else
	{
		printf("other count\n");
	}

	return 0;
}
