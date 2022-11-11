/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 20:35:59 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/11 21:10:36 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>

#define TYPE_TOKEN_WORD     0
#define TYPE_TOKEN_OPERATOR 1
#define TYPE_TOKEN_METACHAR 2

typedef struct s_chunk
{
	char			*content;
	struct s_chunk	*next;
}	t_chunk;

typedef struct s_token
{
	int				type;
	t_chunk			*chunks;
	struct s_token	*next;
}	t_token;

char	*ft_strndup(const char *s1, size_t size)
{
	char	*ptr;
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(s1);
	if (len < size)
		size = len;
	ptr = (char *)calloc(size + 1, sizeof(char));
	if (!ptr)
		return (NULL);
	while (s1[i] && i < size)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

int	ft_is_metachar(const char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	if (c == '|' || c == '<' || c == '>')
		return (2);
	return (0);
}

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

t_token	*ft_find_token_last_and_word(t_token *tokens)
{
	t_token	*last;

	if (tokens == 0)
		return (0);
	last = tokens;
	while (last->next)
		last = last->next;
	if (last->type == TYPE_TOKEN_METACHAR)
		return (0);
	return (last);
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

void	ft_print_tokens(t_token *tokens)
{
	t_chunk	*temp;

	while (tokens)
	{
		printf("token type[%d]\n", tokens->type);
		temp = tokens->chunks;
		while (temp)
		{
			printf("%6s -> ", temp->content);
			temp = temp->next;
		}
		printf("(null)\n");
		tokens = tokens->next;
	}
}

int	ft_get_prev_token_type(const char *str, size_t i)
{
	int	prev_type;

	prev_type = 0;
	if (i > 0)
		prev_type = ft_is_metachar(str[i - 1]);
	return (prev_type);
}

char	*ft_get_content_metachar(const char *str, int type)
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

void	ft_metachar_handler(t_token **tokens, t_token *new, int type, int prev_type)
{
	t_token	*last_word;
	t_chunk	*last_chunk;
	char	*last_content;

	if (type == TYPE_TOKEN_METACHAR)
		ft_token_add_back(tokens, new);
	else
	{
		last_word = ft_find_token_last_and_word(*tokens);
		last_chunk = ft_get_last_chunk(last_word);
		last_content = 0;
		if (last_chunk != 0)
			last_content = last_chunk->content;
		if (last_word == 0 || \
			(last_content[ft_strlen(last_content) - 1] != '"' && \
			last_content[ft_strlen(last_content) - 1] != '\'') || \
			prev_type)
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

char	*ft_get_content_qutoes(const char *str, size_t *i)
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

int	ft_tokenize_qutoes(t_token **tokens, const char *str, size_t *i)
{
	char	*content;
	t_token	*last_word;
	int		prev_type;

	prev_type = ft_get_prev_token_type(str, *i);
	content = ft_get_content_qutoes(str, i);
	last_word = ft_find_token_last_and_word(*tokens);
	if (last_word == 0 || prev_type)
	{
		if (ft_token_add_back(tokens, ft_token_new(content, TYPE_TOKEN_WORD)))
		{
			free(content);
			return (1);
		}
	}
	else
	{
		if (ft_chunk_add_back(last_word, ft_chunk_new(content)))
		{
			free(content);
			return (1);
		}
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
	ft_print_tokens(*tokens);
}

void	ft_check_leaks(void)
{
	system("leaks minishell");
}

char	*convert_env(char *content)
{
	int		i;
	int		start;
	char	*temp;
	char	*env_char;
	char	*str;

	i = 0;
	while (content[i])
	{
		if (content[i] == '$')
		{
			start = i;
			i++;
			while (content[i] && content[i] != ' ' && content[i] != '$')
				i++;
			temp = ft_strndup(&content[start + 1], i - start - 1);
			env_char = getenv(temp);
			free(temp);

		}
		else
			i++;
	}
	return (temp);
}

void	get_env(t_token **tokens)
{
	t_token *node;

	node = *tokens;
	while (node)
	{
		while (node->chunks)
		{
			if (node->chunks->content[0] == '\"' && \
				ft_strchr(node->chunks->content, '$'))
				convert_env(node->chunks->content);
			node->chunks = node->chunks->next;
		}
		node = node->next;
	}
}

int	main(void)
{
	char	*str;
	t_token	*tokens;

	atexit(ft_check_leaks);
	tokens = 0;
	ft_tokens_init(&tokens);
	while (42)
	{
		str = readline("minishell : ");
		add_history(str);
		// 토크나이즈
		ft_tokenize(&tokens, str);
		get_env(&tokens);
		free(str);
		// 후처리
		// 환경 변수 가져오기
//		get_env(&tokens);
		ft_tokens_init(&tokens);
	}
	return (0);
}
