/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_dollar_qutoes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:59:51 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 16:28:26 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static char	*ft_get_errno_str(void)
{
	return (ft_strdup("errno"));
}

static char	*ft_get_content_dollar(char *str, size_t *i)
{
	char	*res;
	char	*temp;
	size_t	start;

	start = *i;
	if (str[start] == '$')
	{
		if (str[++(*i)] == '?' && (*i)++)
			return (ft_get_errno_str());
		if (str[*i] == 0)
			return (ft_strdup("$"));
		while (str[*i] && ft_is_metachar(str[*i]) != 1 && str[*i] != '$')
			(*i)++;
		temp = ft_strndup(str + start + 1, *i - start - 1);
		if (temp == 0)
			return (0);
		res = getenv(temp);
		free(temp);
		if ((str[*i] == '$' && (*i)++) || res == 0)
			return ((char *)ft_calloc(1, sizeof(char)));
		return (ft_strdup(res));
	}
	while (str[*i] && str[*i] != '$')
		(*i)++;
	return (ft_strndup(str + start, *i - start));
}

int	ft_dollar_handler(t_chunk *chunk)
{
	char	*res;
	char	*res_temp;
	char	*temp;
	size_t	i;

	if (*(chunk->content) == '\'' || *(chunk->content) == '"' || \
		!ft_strchr(chunk->content, '$'))
		return (0);
	res = (char *)ft_calloc(1, sizeof(char));
	if (res == 0)
		return (1);
	i = 0;
	while (chunk->content[i])
	{
		temp = ft_get_content_dollar(chunk->content, &i);
		res_temp = ft_strjoin(res, temp);
		free(temp);
		free(res);
		if (res_temp == 0)
			return (1);
		res = res_temp;
	}
	free(chunk->content);
	chunk->content = res;
	return (0);
}

int	ft_qutoes_handler(t_chunk *chunk)
{
	char	*temp;

	if (*(chunk->content) == '\'')
	{
		temp = ft_strtrim(chunk->content, "'");
		if (temp == 0)
			return (1);
		free(chunk->content);
		chunk->content = temp;
	}
	else if (*(chunk->content) == '"')
	{
		temp = ft_strtrim(chunk->content, "\"");
		if (temp == 0)
			return (1);
		free(chunk->content);
		chunk->content = temp;
		ft_dollar_handler(chunk);
	}
	return (0);
}

int	ft_del_empty_token(t_token **tokens)
{
	t_token	*temp;
	t_token	*temp_next;

	if (tokens == 0)
		return (1);
	while (*tokens && ft_strlen((*tokens)->chunks->content) == 0)
	{
		temp = (*tokens)->next;
		ft_token_del(*tokens);
		*tokens = temp;
	}
	temp = *tokens;
	while (temp && temp->next)
	{
		if (ft_strlen(temp->next->chunks->content) == 0)
		{
			temp_next = temp->next->next;
			ft_token_del(temp->next);
			temp->next = temp_next;
		}
		else
			temp = temp->next;
	}
	return (0);
}
