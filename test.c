/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkalinic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 16:30:09 by kkalinic          #+#    #+#             */
/*   Updated: 2022/04/21 16:36:47 by kkalinic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int ft_strlen(char* str)
{
	int i = 0;
	while(str && str[i++])
		;
	return i;
}

void print_err(char* str)
{
	write(2, str, ft_strlen(str));
}

int main(int argc, char** argv)
{
	if (argc == 1)
		return 0;

	char** end = argv;
	char** start = argv;
	while(*end)
	{
		if (!strcmp("|", *end))
		{
			printf("%s\n", *end);
			*end++ = NULL;
			start = end;
			continue;
		}
		else if (!strcmp(";", *end))
		{


			*end++ = NULL;
			start = end;
			continue;
		}
		end++;
	}
	return 0;
}
