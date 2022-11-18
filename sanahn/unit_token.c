/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:46:46 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 14:31:21 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

t_token	*ft_token_new(char *content, int type)
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
	res->type = type;
	res->next = 0;
	return (res);
}

int	ft_token_add_back(t_token **tokens, t_token *new)
{
	t_token	*last;

	if (tokens == 0 || new == 0)
		return (1);
	if (*tokens == 0)
	{
		*tokens = new;
		return (0);
	}
	last = *tokens;
	while (last->next)
		last = last->next;
	last->next = new;
	return (0);
}

t_token	*ft_get_last_token(t_token *tokens)
{
	t_token	*last;

	if (tokens == 0)
		return (0);
	last = tokens;
	while (last->next)
		last = last->next;
	return (last);
}

void	ft_token_del(t_token *token)
{
	t_chunk	*temp;

	if (token == 0)
		return ;
	while (token->chunks)
	{
		temp = token->chunks->next;
		free(token->chunks->content);
		free(token->chunks);
		token->chunks = temp;
	}
	free(token);
}

void	ft_tokens_init(t_token **tokens)
{
	t_token	*temp;

	if (tokens == 0)
		return ;
	while (*tokens)
	{
		temp = (*tokens)->next;
		ft_token_del(*tokens);
		*tokens = temp;
	}
	*tokens = 0;
}
