/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 20:35:59 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/08 21:46:03 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>

#define STATUS_TOKEN_WORD 0
#define STATUS_TOKEN_OPERATOR 1

typedef struct s_token
{
	char			*content;
	int				status;
	struct s_token	*next;
}	t_token;

void	*ft_calloc(size_t size)
{
	void	*res;
	char	*temp;
	size_t	i;

	res = malloc(size);
	if (res == 0)
		return (0);
	temp = (char *)res;
	i = 0;
	while (i < size)
	{
		*(temp + i) = 0;
		i++;
	}
	return (res);
}

void	ft_add_token_back(t_token **tokens, t_token *new)
{
	t_token	*last;

	if (tokens == 0 || new == 0)
		return ;
	if (*tokens == 0)
	{
		*tokens = new;
		return ;
	}
	last = *tokens;
	while (last->next)
		last = (*tokens)->next;
	last->next = new;
}

int	ft_is_blank(const char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	ft_is_cntl_operator(const char *str)
{
	if (*str == '|')
	{
		if (*(str + 1))
		{
			if (*(str + 1) == '|' || *(str + 1) == '&')
				return (1);
		}
		return (1);
	}
	else if (*str == '&')
	{
		if (*(str + 1))
		{
			if (*(str + 1) == '&')
				return (1);
		}
		return (1);
	}
	else if (*str == ';')
	{
		if (*(str + 1))
		{
			if (*(str + 1) == ';' || *(str + 1) == '&')
			{
				if (*(str + 2) && *(str + 2) == '&')
					return (1);
				return (1);
			}
		}
		return (1);
	}
	else if (*str == '(')
		return (1);
	else if (*str == ')')
		return (1);
	else
		return (0);
}

char	*ft_strndup(const char *str, size_t n)
{
	char	*res;
	size_t	i;
	size_t	len;

	if (str == 0)
		return (0);
	i = 0;
	while (str[i])
		i++;
	len = i;
	if (len > n)
		len = n;
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (res == 0)
		return (0);
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = 0;
	return (res);
}

void	ft_split_token(t_token *tokens, const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
}

int	main(void)
{
	char	*str;

	while (1)
	{
		str = readline("minishell : ");
		add_history(str);
		free(str);
	}
}
