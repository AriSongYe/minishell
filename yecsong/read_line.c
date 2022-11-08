/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 15:58:08 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/08 21:50:16 by yecsong          ###   ########.fr       */
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

void	split_token(t_info **info)
{
	while ((*info)->str[(*info)->i])
	{
		if ((*info)->str[(*info)->i] == '<')
		{
			if ((*info)->str[(*info)->i + 1] == '<')
			{
				(*info)->i++;
				add_token(info);
			}
			else
				add_token(info);
		}
		else if ((*info)->str[(*info)->i] == '>')
		{
			if ((*info)->str[(*info)->i + 1] == '>')
			{
				(*info)->i++;
				add_token(info);
			}
			else
				add_token(info);
		}
		else if ((*info)->str[(*info)->i] == '=')
				add_token(info);
		else if ((*info)->str[(*info)->i] == '$')
				add_token(info);
		else if ((*info)->str[(*info)->i] == '-')
				add_token(info);
		else if ((*info)->str[(*info)->i] == '|')
				add_token(info);
		else if ((*info)->str[(*info)->i] == '\'')
		{
			while ((*info)->str[(*info)->i])
			{
				(*info)->i++;
				if ((*info)->str[(*info)->i] == '\'')
					break ;
			}
			add_token(info);
		}
		else if ((*info)->str[(*info)->i] == '\"')
		{
			while ((*info)->str[(*info)->i])
			{
				(*info)->i++;
				if ((*info)->str[(*info)->i] == '\"')
					break ;
			}
			add_token(info);
		}
		else if ((*info)->str[(*info)->i] == ' ')
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
