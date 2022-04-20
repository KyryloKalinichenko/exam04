#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


#ifndef TEST
#define TEST 0
#endif 


/*
	- fd
	- return value
	- printed res
	- printed err
	- 

*/
typedef struct lst_s lst_t;

typedef struct main_s {

	int		argc;
	char**	argv;
	char**	env;
	int		cnt;
	int*	pos;
	lst_t*	lst;
} main_t;


typedef struct lst_s {

	lst_t*	pr;
	lst_t*	next;
	int	fd[2];
	char**	cmd;
	char	type;

} lst_t;

int	ft_strlen(char* str){
	int i = 0;
	while(str && str[i++])
		;
	return i;
}

void	print_err(char* str){
	write(2, str, ft_strlen(str));
}


void count(main_t* stru)
{
	stru->cnt = 0;
	for (int i = 1; i < stru->argc; i++){
		if (!strcmp(stru->argv[i], "|")
			|| !strcmp(stru->argv[i], ";"))
			stru->cnt++;
	}
}

void pos(main_t* stru)
{
	
	if (!(stru->pos = malloc(sizeof(int) * stru->cnt)))
   	{
		print_err("error: fatal\n");
		exit(EXIT_FAILURE);
	}
	int pos = 0;
	for (int i = 1; i < stru->argc; i++){
		if (!strcmp(stru->argv[i], "|")
			|| !strcmp(stru->argv[i], ";"))
			stru->pos[pos++] = i;
		}
}

void lst_create(main_t* stru){
	lst_t* tmp = malloc(sizeof(lst_t));
	lst_t* pr;

	if (!tmp)
   	{
		print_err("error: fatal\n");
		exit(EXIT_FAILURE);
	}
	stru->lst = tmp;
	tmp->next = NULL;
	tmp->pr = NULL;
	tmp->cmd = &stru->argv[1];

	if (stru->cnt == 0)
		tmp->type = ';';
	else{
		tmp->type = *stru->argv[stru->pos[0]];
		stru->argv[stru->pos[0]] = NULL;
	}
	pr = tmp;

	for (int i = 1; i <= stru->cnt; i++)
	{
		tmp = malloc(sizeof(lst_t));
		if (!tmp)
   		{
			print_err("error: fatal\n");
			exit(EXIT_FAILURE);
		}
		tmp->next = NULL;
		tmp->pr = pr;

		pr->next = tmp;

		tmp->cmd = &stru->argv[stru->pos[i-1]+1];
		if (i == stru->cnt)
			tmp->type = ';';
		else{
			tmp->type = *stru->argv[stru->pos[i]];
			stru->argv[stru->pos[i]] = NULL;
		pr = tmp;
		}
	}

}

int exec(char** cmd, char** env){
	if (-1 == execve(*cmd, cmd, env))
	{
		print_err("error: cannot execute ");
		print_err(*cmd);
		print_err("\n");

		return 1;
	}
	return 0;
}

int	comm(main_t* stru, lst_t* curr){
	if(curr->type == '|' && -1 == pipe(curr->fd))
	{
		print_err("error: fatal\n");
		exit(EXIT_FAILURE);
	}
	pid_t pid = fork();
	int stat;
	if (!pid){
		if (curr->type == '|' && dup2(curr->fd[1], 1) == -1)
		{
			print_err("error: fatal\n");
			exit(EXIT_FAILURE);
		}
		if ((curr->pr && curr->pr->type == '|') && dup2(curr->pr->fd[0], 0) == -1)
		{
			print_err("error: fatal\n");
			exit(EXIT_FAILURE);
		}
		if (exec(curr->cmd, stru->env))
			return 1;
	}
	waitpid(pid, &stat, 0);
	if (curr->type == '|')
		close(curr->fd[1]);
	if (curr->pr && curr->pr->type == '|')
		close(curr->pr->fd[0]);
	
	return WEXITSTATUS(stat);
}

void	lst_use(main_t* stru){
	lst_t* curr = stru->lst;
	int res = 0;
	while(curr){
	///	printf("CMD %s TYPE %c \n", *curr->cmd, curr->type);
		if (*curr->cmd == NULL)
		{
			curr = curr->next;
			continue;
		}
		else if (!strcmp(*curr->cmd, "cd"))
		{
			if (!curr->cmd[1] || curr->cmd[2])
			{
				print_err("error: cd: bad arguments\n");
				res = 1;
				curr = curr->next;
				continue ;


			}
			if (-1 == chdir(curr->cmd[1]))
			{
				print_err("error: cd: cannot change directory to ");
				print_err(curr->cmd[1]);
				print_err("\n");
				res = 1;
			}
		}
		else
			res = comm(stru, curr);
		curr = curr->next;
	}
	while (TEST)
		;
	exit(res);
}

int main(int argc, char** argv, char** env)
{
	if (argc == 1)
		return 0;

	main_t stru;

	stru.argc = argc;
	stru.argv = argv;
	stru.env = env;
	
	count(&stru);
	pos(&stru);
	lst_create(&stru);
	lst_use(&stru);

	exit(0);

}
