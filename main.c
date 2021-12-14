#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

void cd(const char *path)
{
	if (!path)
		write(2, "error: cd: bad arguments\n", 26);
		return ;
	printf("%s\n", path);
	chdir(path);
}

void try_cmd(char **cmd, char *envp[]){
	pid_t id;

	id = fork();
	if (id == 0) {
		execve(*cmd, cmd, envp);
		exit(1);
	}
}

int main(int argc, char** argv, char *envp[])
{
	int i = 1;

	if (argc < 2)
		return -1;
	while(i < argc)
	{
		if (!strcmp((const char*)argv[i], "cd"))
				cd(argv[i+1]);
		else{
			try_cmd(argv + i, envp);
		}
		i++;
	}
	return 0;
}
