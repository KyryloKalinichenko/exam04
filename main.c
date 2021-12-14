#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

struct ms {
	int fds[2];
	int type;
	struct ms *next;
	char *cmd;
	char **args;
};

struct main_s{
	char **argv;
	int argc;
	ms *list;
};

void cd(const char *path)
{
	if (!path)
		write(2, "error: cd: bad arguments\n", 26);
		return ;
	printf("%s\n", path);
	chdir(path);
}

int ft_strlen(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	return i;
}

*char ft_strdup(char *str)
{
	char *new;
	int i = 0;

	new = malloc(sizeof(char) * ft_strlen(str) + 1);
	while(str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i] = NULL;
	return new;
}

void try_cmd(char **cmd, char *envp[]){
	pid_t id;
	int fds[2];
	// if next is not NULL && is pipe
	pipe(fds);
	
	id = fork();
	if (id == 0) {
		//if the next cmd is a pipe
		dup2(fds[1], STDOUT_FILENO);
		//if the current cmd is a pipe
		dup2(fds[0], STDIN_FILENO);
		execve(*cmd, cmd, envp);
		exit(1);
	}
}

void parse(main_s *m){
	int i;


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
