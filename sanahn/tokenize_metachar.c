/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_metachar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:49:40 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 14:22:44 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

int	ft_is_metachar(const char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	if (c == '|' || c == '<' || c == '>')
		return (2);
	return (0);
}

int	ft_get_prev_token_type(const char *str, size_t i)
{
	int	prev_type;

	prev_type = 0;
	if (i > 0)
		prev_type = ft_is_metachar(str[i - 1]);
	return (prev_type);
}

static char	*ft_get_content_metachar(const char *str, int type)
{
	char	*res;
	size_t	len;
	size_t	i;

	len = 0;
	while (str[len] && ft_is_metachar(str[len]) == type \
		&& str[len] != '"' && str[len] != '\'')
		len++;
	res = (char *)ft_calloc(len + 1, sizeof(char));
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

static void	ft_metachar_handler(t_token **tokens, t_token *new, int t, int p_t)
{
	t_token	*last_word;
	t_chunk	*last_chunk;
	char	*last_content;

	if (t == TYPE_TOKEN_METACHAR)
		ft_token_add_back(tokens, new);
	else
	{
		last_word = ft_get_last_token(*tokens);
		if (last_word && last_word->type == TYPE_TOKEN_METACHAR)
			last_word = 0;
		last_chunk = ft_get_last_chunk(last_word);
		last_content = 0;
		if (last_chunk != 0)
			last_content = last_chunk->content;
		if (last_word == 0 || \
			(last_content[ft_strlen(last_content) - 1] != '"' && \
			last_content[ft_strlen(last_content) - 1] != '\'') || p_t)
			ft_token_add_back(tokens, new);
		else
		{
			ft_chunk_add_back(last_word, new->chunks);
			free(new);
		}
	}
}

int	ft_tokenize_metachar(t_token **tokens, const char *str, size_t *i)
{
	char	*content;
	t_token	*new_token;
	int		type;
	int		prev_type;

	if (ft_is_metachar(str[*i]) == 1)
		(*i)++;
	else
	{
		prev_type = ft_get_prev_token_type(str, *i);
		type = ft_is_metachar(str[*i]);
		content = ft_get_content_metachar(str + *i, type);
		new_token = ft_token_new(content, type);
		if (new_token == 0)
		{
			free(content);
			return (1);
		}
		ft_metachar_handler(tokens, new_token, type, prev_type);
		while (str[*i] && ft_is_metachar(str[*i]) == type \
			&& str[*i] != '"' && str[*i] != '\'')
			(*i)++;
	}
	return (0);
}
