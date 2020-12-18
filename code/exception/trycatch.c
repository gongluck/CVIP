
#include <stdio.h>
#include <setjmp.h>

typedef struct _tagExcepSign
{
	jmp_buf _stackinfo;
	int _exceptype;
} tagExcepSign;

#define ExcepType(ExcepSign) ((ExcepSign)._exceptype)

#define Try(ExcepSign) if (((ExcepSign)._exceptype = setjmp((ExcepSign)._stackinfo)) == 0)

#define Catch(ExcepSign, ExcepType) else if ((ExcepSign)._exceptype == ExcepType)

#define Finally else

#define Throw(ExcepSign, ExcepType) longjmp((ExcepSign)._stackinfo, ExcepType)

void ExceptionTest(int expType)
{
	tagExcepSign ex;

	expType = expType < 0 ? -expType : expType;

	Try(ex)
	{
		if (expType > 0)
		{
			Throw(ex, expType);
		}
		else
		{
			printf("no exception\n");
		}
	}
	Catch(ex, 1)
	{
		printf("no exception 1\n");
	}
	Catch(ex, 2)
	{
		printf("no exception 2\n");
	}
	Finally
	{
		printf("other exp\n");
	}
}

int main()
{
	ExceptionTest(0);
	ExceptionTest(1);
	ExceptionTest(2);
	ExceptionTest(3);
}
