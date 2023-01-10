/*
 * @Author: gongluck
 * @Date: 2023-01-10 16:05:08
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-10 16:12:14
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  char buf[BUFSIZ] = {0};
  pid_t pid;

  while (1)
  {
    printf("minibash$ ");
    scanf("%s", buf);
    pid = fork();
    if (pid == 0)
    {
      if (execlp(buf, buf, NULL) < 0)
        puts("Exec error");
    }
    else if (pid > 0)
    {
      int status;
      waitpid(pid, &status, 0);
    }
    else
    {
      printf("Fork error %d\n", pid);
    }
  }
  return 0;
}