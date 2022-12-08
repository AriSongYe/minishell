#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv, char **envp)
{
	printf("envp = %s\n", envp[0]);
	envp[0] = NULL;
	printf("envp = %s\n", envp[0]);
	return (0);
}
