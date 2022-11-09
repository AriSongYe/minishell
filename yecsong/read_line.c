/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 15:58:08 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/09 16:13:41 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*part_dup(char *s1, int start, int end)
{
	char	*ptr;
	int		i;

	i = 0;
	ptr = (char *)malloc(sizeof(char) * (end - start + 2));
	if (!ptr)
		return (NULL);
	while (i < end - start + 1)
	{
		ptr[i] = s1[start + i];
		i++;
	}
	ptr[end - start + 1] = '\0';
	return (ptr);
}

t_token	*last_token(t_token **token)
{
	t_token *last;

	last = (*token);
	while (last->next)
		last = last->next;
	return (last);
}

void	new_token(t_token **new)
{
	t_token	*last;

	if (!(*new))
	{
		(*new) = malloc(sizeof(t_token));
		(*new)->next = NULL;
		if (!(*new))
			return ;
	}
	else
	{
		last = last_token(new);
		last->next = malloc(sizeof(t_token));
		if (!last->next)
			return ;
		last->next->next = NULL;
	}
}

void	add_token(t_info **info)
{
	t_token	*last;

	new_token(&(*info)->token);
	last = last_token(&(*info)->token);
	last->token = part_dup((*info)->str, (*info)->start, (*info)->i - 1);
	(*info)->start = (*info)->i;
}

void	free_token(t_info **info)
{
	t_token	*token;
	t_token	*temp;

	token = (*info)->token;
	while (token)
	{
		temp = token;
		token = token->next;
		free(temp->token);
		free(temp);
	}
}
#define	STR 			0
#define INPUT_FILE		1
#define OUTPUT_FILE		2
#define HERE_DOC		3
#define APPEND_MODE		4
#define ASSIGN			5
#define PIPE			6
#define	WHITE_SPACE		7
#define	SINGLE_QUOTE	8
#define	DOUBLE_QUOTE	9

int	check_status(info, status)
{
}

int	classify_status(char *str, int index)
{
	if (ft_isalpha(str[index]) || ft_isdigit(str[index]))
		return (STR);
	else if (str[index] == '<' && str[index + 1] != '<')
		return (INPUT_FILE);
	else if (str[index] == '<' && str[index + 1] == '<')
		return (OUTPUT_FILE);
	else if (str[index] == '>' && str[index + 1] != '>')
		return (HERE_DOC);
	else if (str[index] == '<' && str[index + 1] == '<')
		return (OUTPUT_FILE);
	else if (str[index] == '<' && str[index + 1] == '<')
		return (APPEND_MODE);
	else if (str[index] == '=')
		return (ASSIGN);
	else if (str[index] =='|')
		return (PIPE);
	else if  ((str[index] >= '\t' && str[index] <= '\r') || str[index] == ' ')
		return (WHITE_SPACE);
	else
		return (STR);
}

void	split_token(t_info **info)
{
	int	status;
	
	while ((*info)->str[(*info)->i])
	{
		status = classify_status((*info)->str, (*info)->i);
		if ((*info)->i != 0 &&
				status != classify_status((*info)->str,(*info)->i - 1))
			add_token(info);
		(*info)->i++;
	}
	t_token	*temp;
	temp = (*info)->token;
	while (temp)
	{
		printf("token = %s\n", temp->token);
		temp = temp->next;
	}
	free_token(info);
}

t_token	*tokenize(char *str)
{
	t_info	*info;
	t_token	*token;

	token = NULL;
	info = malloc(sizeof(t_info));
	info->i = 0;
	info->start = 0;
	info->str = str;
	info->token = token;
	split_token(&info);
	free(info);
	return (token);
}


int	main()
{
	char	*str;
	t_token	*token;

	while (1)
	{
		str = readline("minishell : ");
		add_history(str);
		token = tokenize(str);
		free(str);
	//	system("leaks a.out");
	}
}
