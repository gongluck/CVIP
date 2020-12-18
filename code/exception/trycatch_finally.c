#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>

#include <pthread.h>
#include <setjmp.h>

#define ntyThreadData pthread_key_t
#define ntyThreadDataSet(key, value) pthread_setspecific((key), (value))
#define ntyThreadDataGet(key) pthread_getspecific((key))
#define ntyThreadDataCreate(key) pthread_key_create(&(key), NULL)

#define EXCEPTIN_MESSAGE_LENGTH 512

typedef struct _ntyException
{
	const char *name;
} ntyException;

ntyException SQLException = {"SQLException"};
ntyException TimeoutException = {"TimeoutException"};

ntyThreadData ExceptionStack;

typedef struct _ntyExceptionFrame
{
	jmp_buf env;

	int line;
	const char *func;
	const char *file;

	ntyException *exception;
	struct _ntyExceptionFrame *prev;

	char message[EXCEPTIN_MESSAGE_LENGTH + 1];

} ntyExceptionFrame;

#define ntyExceptionPopStack \
	ntyThreadDataSet(ExceptionStack, ((ntyExceptionFrame *)ntyThreadDataGet(ExceptionStack))->prev)

#define ReThrow ntyExceptionThrow(frame.exception, frame.func, frame.file, frame.line, NULL)
#define Throw(e, cause, ...) ntyExceptionThrow(&(e), __func__, __FILE__, __LINE__, cause, ##__VA_ARGS__, NULL)

enum
{
	ExceptionEntered = 0,
	ExceptionThrown,
	ExceptionHandled,
	ExceptionFinalized
};

#define Try                                                                 \
	do                                                                      \
	{                                                                       \
		volatile int Exception_flag;                                        \
		ntyExceptionFrame frame;                                            \
		frame.message[0] = 0;                                               \
		frame.prev = (ntyExceptionFrame *)ntyThreadDataGet(ExceptionStack); \
		ntyThreadDataSet(ExceptionStack, &frame);                           \
		Exception_flag = setjmp(frame.env);                                 \
		if (Exception_flag == ExceptionEntered)                             \
		{

#define Catch(e)                            \
	if (Exception_flag == ExceptionEntered) \
		ntyExceptionPopStack;               \
	}                                       \
	else if (frame.exception == &(e))       \
	{                                       \
		Exception_flag = ExceptionHandled;

#define Finally                                 \
	if (Exception_flag == ExceptionEntered)     \
		ntyExceptionPopStack;                   \
	}                                           \
	{                                           \
		if (Exception_flag == ExceptionEntered) \
			Exception_flag = ExceptionFinalized;

#define EndTry                              \
	if (Exception_flag == ExceptionEntered) \
		ntyExceptionPopStack;               \
	}                                       \
	if (Exception_flag == ExceptionThrown)  \
		ReThrow;                            \
	}                                       \
	while (0)

static pthread_once_t once_control = PTHREAD_ONCE_INIT;

static void init_once(void)
{
	ntyThreadDataCreate(ExceptionStack);
}

void ntyExceptionInit(void)
{
	pthread_once(&once_control, init_once);
}

void ntyExceptionThrow(ntyException *excep, const char *func, const char *file, int line, const char *cause, ...)
{

	va_list ap;
	ntyExceptionFrame *frame = (ntyExceptionFrame *)ntyThreadDataGet(ExceptionStack);

	if (frame)
	{

		frame->exception = excep;
		frame->func = func;
		frame->file = file;
		frame->line = line;

		if (cause)
		{
			va_start(ap, cause);
			vsnprintf(frame->message, EXCEPTIN_MESSAGE_LENGTH, cause, ap);
			va_end(ap);
		}

		ntyExceptionPopStack;

		longjmp(frame->env, ExceptionThrown);
	}
	else if (cause)
	{

		char message[EXCEPTIN_MESSAGE_LENGTH + 1];

		va_start(ap, cause);
		vsnprintf(message, EXCEPTIN_MESSAGE_LENGTH, cause, ap);
		va_end(ap);

		printf("%s: %s\n raised in %s at %s:%d\n", excep->name, message, func ? func : "?", file ? file : "?", line);
	}
	else
	{

		printf("%s: %p\n raised in %s at %s:%d\n", excep->name, excep, func ? func : "?", file ? file : "?", line);
	}
}

/* ** **** ******** **************** debug **************** ******** **** ** */

ntyException A = {"AException"};
ntyException B = {"BException"};
ntyException C = {"CException"};
ntyException D = {"DException"};

void *thread(void *args)
{

	pthread_t selfid = pthread_self();

	Try
	{

		Throw(A, "A");
	}
	Catch(A)
	{

		printf("catch A : %ld\n", selfid);
	}
	EndTry;

	Try
	{

		Throw(B, "B");
	}
	Catch(B)
	{

		printf("catch B : %ld\n", selfid);
	}
	EndTry;

	Try
	{

		Throw(C, "C");
	}
	Catch(C)
	{

		printf("catch C : %ld\n", selfid);
	}
	EndTry;

	Try
	{

		Throw(D, "D");
	}
	Catch(D)
	{

		printf("catch D : %ld\n", selfid);
	}
	EndTry;

	Try
	{

		Throw(A, "A Again");
		Throw(B, "B Again");
		Throw(C, "C Again");
		Throw(D, "D Again");
	}
	Catch(A)
	{

		printf("catch A again : %ld\n", selfid);
	}
	Catch(B)
	{

		printf("catch B again : %ld\n", selfid);
	}
	Catch(C)
	{

		printf("catch C again : %ld\n", selfid);
	}
	Catch(D)
	{

		printf("catch B again : %ld\n", selfid);
	}
	EndTry;
}

#define THREADS 50

int main(void)
{

	ntyExceptionInit();

	Throw(D, NULL);

	Throw(C, "null C");

	printf("\n\n=> Test1: Try-Catch\n");

	Try
	{

		Try
		{
			Throw(B, "recall B");
		}
		Catch(B)
		{
			printf("recall B \n");
		}
		EndTry;

		Throw(A, NULL);
	}
	Catch(A)
	{

		printf("\tResult: Ok\n");
	}
	EndTry;

	printf("=> Test1: Ok\n\n");

	printf("=> Test2: Test Thread-safeness\n");
#if 1
	int i = 0;
	pthread_t threads[THREADS];

	for (i = 0; i < THREADS; i++)
	{
		pthread_create(&threads[i], NULL, thread, NULL);
	}

	for (i = 0; i < THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
#endif
	printf("=> Test2: Ok\n\n");
}
