/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:43:42 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/22 18:25:34 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

t_chunk	*ft_chunk_new(char *content)
{
	t_chunk	*res;

	if (content == 0)
		return (0);
	res = (t_chunk *)ft_calloc(1, sizeof(t_chunk));
	if (res == 0)
		return (0);
	res->content = content;
	res->next = 0;
	return (res);
}

t_token	*ft_token_new(char *content, int token_type)
{
	t_token	*res;

	if (content == 0)
		return (0);
	res = (t_token *)ft_calloc(1, sizeof(t_token));
	if (res == 0)
		return (0);
	res->chunks = ft_chunk_new(content);
	if (res->chunks == 0)
	{
		free(res);
		return (0);
	}
	res->type = token_type;
	res->next = 0;
	return (res);
}

void	*ft_get_last_node(void *lst, int data_type)
{
	void	*res;

	if (lst == 0)
		return (0);
	res = lst;
	if (data_type == TYPE_CHUNK)
	{
		while (((t_chunk *)res)->next)
			res = ((t_chunk *)res)->next;
	}
	else if (data_type == TYPE_TOKEN)
	{
		while (((t_token *)res)->next)
			res = ((t_token *)res)->next;
	}
	else
	{
		while (((t_cmd *)res)->next)
			res = ((t_cmd *)res)->next;
	}
	return (res);
}

void	ft_lst_add_back(void **lst, void *new, int data_type)
{
	if (lst == 0 || new == 0)
		return ;
	if (data_type == TYPE_CHUNK)
	{
		if (*((t_chunk **)lst) == 0)
			*((t_chunk **)lst) = (t_chunk *)new;
		else
			((t_chunk *)ft_get_last_node(*lst, data_type))->next \
				= (t_chunk *)new;
	}
	else if (data_type == TYPE_TOKEN)
	{
		if (*((t_token **)lst) == 0)
			*((t_token **)lst) = (t_token *)new;
		else
			((t_token *)ft_get_last_node(*lst, data_type))->next \
				= (t_chunk *)new;
	}
	else
	{
		if (*((t_cmd **)lst) == 0)
			*((t_cmd **)lst) = (t_cmd *)new;
		else
			((t_cmd *)ft_get_last_node(*lst, data_type))->next = (t_cmd *)new;
	}
}

int	ft_get_metachar_type(const char c)
{
	if (c == ' ' || c == '\t')
		return (TYPE_METACHAR_BLANK);
	if (c == '|' || c == '<' || c == '>')
		return (TYPE_METACHAR_OPER);
	return (TYPE_WORD);
}

// void	ft_tokenize_metachar()

// void	ft_lexer(t_token **tokens, const char *str)
// {

// }

int	main(void)
{
	char	*str;
	t_token	*tokens;

	tokens = 0;
	while (42)
	{
		str = readline("minishell : ");
		add_history(str);
		free(str);
	}
	return (0);
}
