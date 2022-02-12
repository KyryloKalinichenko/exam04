#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

typedef struct list_s list_t;

typedef struct	main_s
{
	char	**argv;
	int	argc;
	int*	pos; 
	int	count;
	list_t* list;

} main_t;

typedef struct list_s
{
	list_t *next;
	list_t *pr;
	int fd[2];
	char **cmd;
	char type;
	int number;

} list_t;

void cd(char *path){
	if (chdir(path)){
		write(1, "error: cd: cannot change directory to path_to_change\n", 53);
		exit(1);
	}
}

int run(char *cmd, char **arg, char* envp[]){
	if (strcmp("cd", cmd)){	

		if (-1 == execve(cmd, arg, envp)){
			write(2, "error: exec\n", 12);
			exit(EXIT_FAILURE);
		}

	}
	else
		cd(arg[1]);
	return 0;
}

int count(int argc, char** argv){
	int counter = 0;
	for(int i = 0; i< argc; i++){
		if(!(strcmp("|", argv[i])) || !(strcmp(";", argv[i]))){
			counter++;
		}
	}
	return counter;
}

int* pos(char** argv, int size){
	int *pos = malloc(sizeof(int) * (2 + size));
	int next = 1;
	int i = 0;
	pos[0] = 1;
	while(argv[i]){
		if(!(strcmp("|", argv[i])) || !(strcmp(";", argv[i]))){
			pos[next++] = i;
		}
		i++;
	}
	return pos;
}



int len(char** t){
	int i = 0;
	while(t[i])
		i++;
	return i;
}

void print(main_t* stru, char *envp[]){
	list_t* curr;
	curr = stru->list;
	while(curr)
	{
	//	printf("Type = %c\n Cmd = %s\n Address = %p\n", curr->type, *curr->cmd, curr->next);

		if (curr->type == '|' && pipe(curr->fd) == -1){
			write(2, "error: pipe\n", 12);
			exit(EXIT_FAILURE);
		}
		int pid = 0;
		pid = fork();
		if (!pid){
			if (curr->type == '|')
				dup2(curr->fd[1], 1);
			if(curr->pr && curr->pr->type == '|')
				dup2(curr->pr->fd[0], 0);
			// close(curr->fd[0]);
			// close(curr->fd[1]);
			run(*curr->cmd, curr->cmd, envp);
		}

		if (curr->type == '|' || (curr->pr && curr->pr->type == '|'))
		{
			close(curr->fd[1]);
			if (curr->type != '|')
				close(curr->fd[0]);
		}
		if (curr->pr && curr->pr->type == '|')
			close(curr->pr->fd[0]);
		waitpid(pid, 0, 0);
		
		curr = curr->next;
	}
}

void fill_list(main_t *stru, int size){
	list_t* tmp;
	list_t* last;
	
	tmp = malloc(sizeof(list_t));
	tmp->next = NULL;
	tmp->pr = NULL;
	if (stru->count == 1)
		tmp->type = ';';
	else
		tmp->type = *stru->argv[stru->pos[0]];
	
	//printf("Shit detector %s\n", stru->argv[stru->pos[0]]);
	
	tmp->cmd = &stru->argv[1];
	

	stru->list = tmp;
	last = tmp;
	tmp->number = 0;
	//printf("Type = %c\n Cmd = %s\n Address = %p\n", tmp->type, *tmp->cmd, tmp);
	for (int i = 1; i < size; i++)
	{
		tmp = malloc(sizeof(list_t));
		tmp->next = NULL;
		if (i+1 < size)
			tmp->type = *stru->argv[stru->pos[i+1]];
		else
			tmp->type = ';';
		tmp->cmd = &stru->argv[stru->pos[i]+1];
		tmp->number = i;
		stru->argv[stru->pos[i]] = NULL;
		tmp->pr = last;
		
	//	printf("Type = %c\n Cmd = %s\n Address = %p\n", tmp->type, *tmp->cmd, tmp);


		last->next = tmp;
		last = tmp;
	}
}

#ifdef TEST_SH
# define TEST		1
#else
# define TEST		0
#endif


int main(int argc, char** argv, char *envp[])
{
	main_t stru;
	
	if (argc == 1)
		return 1;
	
	stru.argc = argc;
	stru.argv = argv;
	
	stru.count = count(argc, argv) + 1;
	stru.pos = pos(argv, stru.count);
	//printf("Amount %d\n", stru.count);
	fill_list(&stru, stru.count);
	print(&stru, envp);
	if (TEST)
		while(1);
	return 0;
}
