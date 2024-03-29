/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 15:58:08 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/29 19:58:01 by yecsong          ###   ########.fr       */
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

void	add_token(t_info **info, int len)
{
	t_token	*last;

	new_token(&(*info)->token);
	last = last_token(&(*info)->token);
	last->token = part_dup((*info)->str, (*info)->start, (*info)->i + len);
	(*info)->start = (*info)->i + 1 + len;
	if (len == 1)
		(*info)->i++;
}

void	free_token(t_token *target)
{
	t_token	*token;
	t_token	*temp;

	token = target;
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
#define	OPTION			10
#define	DOLLAR			11

int	classify_status(char *str, int index)
{
	if (ft_isalpha(str[index]) || ft_isdigit(str[index]))
		return (STR);
	else if (str[index] == '<' && str[index + 1] != '<')
		return (INPUT_FILE);
	else if (str[index] == '<' && str[index + 1] == '<')
		return (HERE_DOC);
	else if (str[index] == '>' && str[index + 1] != '>')
		return (OUTPUT_FILE);
	else if (str[index] == '<' && str[index + 1] == '<')
		return (APPEND_MODE);
	else if (str[index] == '=')
		return (ASSIGN);
	else if (str[index] == '|')
		return (PIPE);
	else if (str[index] == '-')
		return (OPTION);
	else if (str[index] == '\"')
		return (DOUBLE_QUOTE);
	else if (str[index] == '\'')
		return (SINGLE_QUOTE);
	else if  ((str[index] >= '\t' && str[index] <= '\r') || str[index] == ' ')
		return (WHITE_SPACE);
	else if (str[index] == '$')
		return (OPTION);
	else
		return (STR);
}

void	split_token(t_info **info)
{
	int	status;
	
	while ((*info)->str[(*info)->i])
	{
		status = classify_status((*info)->str, (*info)->i);
		if (status != classify_status((*info)->str,(*info)->i + 1))
		{
			if (status == HERE_DOC || status == APPEND_MODE)
				add_token(info, 1);
			else if (status == WHITE_SPACE &&
					(*info)->str[(*info)->i + 1] == ' ')
				(*info)->i++;
			else if (status == DOUBLE_QUOTE)
			{
				(*info)->i++;
				while ((*info)->str[(*info)->i] &&
						(*info)->str[(*info)->i] != '\"')
					(*info)->i++;
				add_token(info, 1);
			}
			else if (status == SINGLE_QUOTE)
			{
				(*info)->i++;
				while ((*info)->str[(*info)->i] &&
						(*info)->str[(*info)->i] != '\'')
					(*info)->i++;
				add_token(info, 1);
			}
			else	
				add_token(info, 0);
		}
		(*info)->i++;
	}
	if (status == STR)
		add_token(info, 0);
	t_token	*temp;
	temp = (*info)->token;
	while (temp)
	{
		printf("token = %s\n", temp->token);
		temp = temp->next;
	}
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
	token = info->token;
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
		free_token(token);
		system("leaks a.out");
	}
}
