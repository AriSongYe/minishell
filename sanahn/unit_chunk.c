/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_chunk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:38:31 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 14:25:23 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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

int	ft_chunk_add_back(t_token *token, t_chunk *new)
{
	t_chunk	*last;

	if (token == 0 || new == 0)
		return (1);
	if (token->chunks == 0)
	{
		token->chunks = new;
		return (0);
	}
	last = token->chunks;
	while (last->next)
		last = last->next;
	last->next = new;
	return (0);
}

t_chunk	*ft_get_last_chunk(t_token *token)
{
	t_chunk	*last;

	if (token == 0)
		return (0);
	last = token->chunks;
	while (last->next)
		last = last->next;
	return (last);
}

int	ft_chunk_join(t_chunk *chunk)
{
	char	*res;
	t_chunk	*temp_chunk;

	while (chunk->next)
	{
		res = ft_strjoin(chunk->content, chunk->next->content);
		if (res == 0)
			return (1);
		free(chunk->content);
		free(chunk->next->content);
		temp_chunk = chunk->next->next;
		free(chunk->next);
		chunk->next = temp_chunk;
		chunk->content = res;
	}
	return (0);
}

int	ft_chunk_iteri(t_token *tokens, int (*fp)(t_chunk *))
{
	t_chunk	*chunk;
	int		return_value;

	while (tokens)
	{
		chunk = tokens->chunks;
		while (chunk)
		{
			return_value = fp(chunk);
			if (return_value == 1)
				return (1);
			chunk = chunk->next;
		}
		tokens = tokens->next;
	}
	return (0);
}
