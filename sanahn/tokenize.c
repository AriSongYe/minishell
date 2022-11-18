/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:57:06 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 13:57:49 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static char	*ft_get_content_qutoes(const char *str, size_t *i)
{
	char	*res;
	char	qutoes;
	size_t	start;
	size_t	len;

	qutoes = str[*i];
	start = *i;
	len = start + 1;
	while (str[len] && (str[len] != qutoes))
		len++;
	if (str[len] == 0)
		return (0);
	len = len - start + 1;
	res = (char *)ft_calloc(len + 1, sizeof(char));
	if (res == 0)
		return (0);
	start = -1;
	while (++start < len)
	{
		res[start] = str[*i];
		(*i)++;
	}
	res[start] = 0;
	return (res);
}

static int	ft_tokenize_qutoes(t_token **tokens, const char *str, size_t *i)
{
	char	*content;
	t_token	*last_word;
	int		prev_type;

	prev_type = ft_get_prev_token_type(str, *i);
	content = ft_get_content_qutoes(str, i);
	last_word = ft_get_last_token(*tokens);
	if (last_word && last_word->type == TYPE_TOKEN_METACHAR)
		last_word = 0;
	if (last_word == 0 || prev_type)
	{
		if (ft_token_add_back(tokens, ft_token_new(content, TYPE_TOKEN_WORD)))
		{
			free(content);
			return (1);
		}
	}
	else if (ft_chunk_add_back(last_word, ft_chunk_new(content)))
	{
		free(content);
		return (1);
	}
	return (0);
}

void	ft_tokenize(t_token **tokens, const char *str)
{
	size_t	i;
	int		error_num;

	i = 0;
	while (ft_is_metachar(str[i]) == 1)
		i++;
	error_num = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			error_num = ft_tokenize_qutoes(tokens, str, &i);
		else
			error_num = ft_tokenize_metachar(tokens, str, &i);
		if (error_num)
		{
			ft_tokens_init(tokens);
			return ;
		}
	}
}
