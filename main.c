#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

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

	tmp->next = NULL;
	tmp->pr = NULL;
	tmp->cmd = &stru->argv[1];

	if (stru->cnt == 0)
		tmp->type = ';';
	else
		tmp->type = stru->argv[stru->pos[0]];
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

		tmp->cmd = stru->argv[stru->pos[i-1]+1];

		
	}

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

	exit(0);

}
