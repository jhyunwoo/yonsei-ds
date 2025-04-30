#include <sys/_types/_pid_t.h>
#include <iostream>

int main()
{
    pid_t pid;
    printf("about to call fork\n");
    pid = fork();

}
