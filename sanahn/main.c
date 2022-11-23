/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:43:42 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/23 18:07:31 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

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

void	ft_print_tokens(t_token *tokens)
{
	t_chunk	*temp;

	if (tokens == 0)
		return ;
	while (tokens)
	{
		temp = tokens->chunks;
		while (temp)
		{
			printf("%12s -> ", temp->content);
			temp = temp->next;
		}
		printf("(null)\n");
		tokens = tokens->next;
	}
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
	if (*lst == 0)
	{
		*lst = new;
		return ;
	}
	if (data_type == TYPE_CHUNK)
		((t_chunk *)ft_get_last_node(*lst, data_type))->next = new;
	else if (data_type == TYPE_TOKEN)
		((t_token *)ft_get_last_node(*lst, data_type))->next = new;
	else
		((t_cmd *)ft_get_last_node(*lst, data_type))->next = new;
}

int	ft_get_metachar_type(const char c)
{
	if (c == ' ' || c == '\t')
		return (TYPE_METACHAR_BLANK);
	if (c == '|' || c == '<' || c == '>')
		return (TYPE_METACHAR_OPERATOR);
	return (TYPE_WORD);
}

int	ft_tokenize_type(t_token **tokens, const char *str, int type, size_t *i)
{
	size_t	start;
	t_token	*new;
	char	*content;

	start = *i;
	if (type == TYPE_WORD)
		while (str[*i] && ft_get_metachar_type(str[*i]) == type \
			&& str[*i] != '\'' && str[*i] != '"')
			(*i)++;
	else
		while (str[*i] && ft_get_metachar_type(str[*i]) == type)
			(*i)++;
	if (start != *i)
	{
		content = ft_strndup(str + start, *i - start);
		new = ft_token_new(content, type);
		if (new == 0)
		{
			free(content);
			return (1);
		}
		ft_lst_add_back((void **)tokens, (void *)new, TYPE_TOKEN);
	}
	return (0);
}

int	ft_tokenize_qutoes(t_token **tokens, const char *str, size_t *i)
{
	char	qutoes;
	size_t	start;
	t_token	*new;
	char	*content;

	qutoes = str[*i];
	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != qutoes)
		(*i)++;
	if (str[*i] == 0)
		return (1);
	(*i)++;
	if (start != *i)
	{
		content = ft_strndup(str + start, *i - start);
		new = ft_token_new(content, TYPE_WORD);
		if (new == 0)
		{
			free(content);
			return (1);
		}
		ft_lst_add_back((void **)tokens, (void *)new, TYPE_TOKEN);
	}
	return (0);
}

int	ft_tokenize_word(t_token **tokens, const char *str, size_t *i)
{
	t_token	*res;
	t_token	*temp_token;
	int		is_error;

	res = 0;
	while (str[*i] && ft_get_metachar_type(str[*i]) == TYPE_WORD)
	{
		if (str[*i] == '\'' || str[*i] == '"')
			is_error = ft_tokenize_qutoes(&res, str, i);
		else
			is_error = ft_tokenize_type(&res, str, TYPE_WORD, i);
		if (is_error)
		{
			ft_tokens_init(&res);
			return (1);
		}
	}
	while (res && res->next)
	{
		ft_lst_add_back(\
			(void **)&res->chunks, (void *)res->next->chunks, TYPE_CHUNK);
		temp_token = res->next->next;
		free(res->next);
		res->next = temp_token;
	}
	ft_lst_add_back((void **)tokens, (void *)res, TYPE_TOKEN);
	return (0);
}

void	ft_tokenize(t_token **tokens, const char *str)
{
	size_t	i;
	int		is_error;
	int		type;

	if (tokens == 0 || str == 0)
		return ;
	i = 0;
	while (str[i] && ft_get_metachar_type(str[i]) == TYPE_METACHAR_BLANK)
		i++;
	while (str[i])
	{
		type = ft_get_metachar_type(str[i]);
		if (type == TYPE_WORD)
			is_error = ft_tokenize_word(tokens, str, &i);
		else
			is_error = ft_tokenize_type(tokens, str, type, &i);
		if (is_error)
		{
			ft_tokens_init(tokens);
			return ;
		}
	}
}

int	ft_get_operator_type(t_token *token)
{
	char	*content;

	if (token == 0 || token->type != TYPE_METACHAR_OPERATOR)
		return (-1);
	content = token->chunks->content;
	if (*content == '|' && ft_strncmp(content, "|", 2) == 0)
		return (TYPE_OPERATOR_PIPE);
	else if (*content == '<')
	{
		if (ft_strncmp(content, "<", 2) == 0)
			return (TYPE_OPERATOR_LESS);
		if (ft_strncmp(content, "<<", 3) == 0)
			return (TYPE_OPERATOR_DLESS);
	}
	else if (*content == '>')
	{
		if (ft_strncmp(content, ">", 2) == 0)
			return (TYPE_OPERATOR_GREAT);
		if (ft_strncmp(content, ">>", 3) == 0)
			return (TYPE_OPERATOR_DGREAT);
	}
	else
		return (TYPE_OPERATOR_ERROR);
	return (-1);
}

void	ft_token_set_type(t_token *tokens)
{
	char	*content;

	while (tokens)
	{
		if (tokens->type == TYPE_METACHAR_OPERATOR)
			tokens->type = ft_get_operator_type(tokens);
		else if (tokens->type == TYPE_WORD)
		{
			content = tokens->chunks->content;
			if (*content == '\'' || *content == '"')
				tokens->type = TYPE_WORD_QUTOES;
			else if (tokens->chunks->next)
				tokens->type = TYPE_WORD_QUTOES;
		}
		tokens = tokens->next;
	}
}

int	ft_is_io_number(t_token *token)
{
	char	*content;
	size_t	i;
	size_t	start;
	int		temp;

	if (token == 0 || token->type != TYPE_WORD)
		return (0);
	content = token->chunks->content;
	i = 0;
	while (content[i] == '0')
		i++;
	start = i;
	while (content[i] >= '0' && content[i] <= '9')
		i++;
	if (content[i] != 0 || i - start > 10 \
		|| (i - start == 10 && content[start] > '2'))
		return (0);
	if (i - start < 10)
		return (1);
	i = 0;
	temp = ft_atoi(content + start);
	if (temp > 0)
		return (1);
	return (0);
}

void	ft_join_io_number(t_token **tokens)
{
	t_token	*temp;
	t_token	*temp_next;

	if (ft_is_io_number(*tokens) && \
		(*tokens)->next && (*tokens)->next->type >= TYPE_OPERATOR_LESS)
	{
		ft_lst_add_back((void **)&((*tokens)->next->chunks), \
			(void *)(*tokens)->chunks, TYPE_CHUNK);
		temp_next = (*tokens)->next;
		free(*tokens);
		*tokens = temp_next;
	}
	temp = *tokens;
	while (temp && temp->next)
	{
		if (ft_is_io_number(temp->next) && \
			temp->next->next && temp->next->next->type >= TYPE_OPERATOR_LESS)
		{
			ft_lst_add_back((void **)&(temp->next->next->chunks), \
				(void *)temp->next->chunks, TYPE_CHUNK);
			temp_next = temp->next->next;
			free(temp->next);
			temp->next = temp_next;
		}
		temp = temp->next;
	}
}

void	ft_blank_token_del(t_token **tokens)
{
	t_token	*temp;
	t_token	*temp_next;

	if (tokens == 0 || *tokens == 0)
		return ;
	if ((*tokens)->type == TYPE_METACHAR_BLANK)
	{
		temp = (*tokens)->next;
		ft_token_del(*tokens);
		*tokens = temp;
	}
	temp = *tokens;
	while (temp && temp->next)
	{
		if (temp->next->type == TYPE_METACHAR_BLANK)
		{
			temp_next = temp->next->next;
			ft_token_del(temp->next);
			temp->next = temp_next;
		}
		temp = temp->next;
	}
}

void	ft_lexer(t_token **tokens, const char *str)
{
	ft_tokenize(tokens, str);
	ft_token_set_type(*tokens);
	ft_join_io_number(tokens);
	ft_blank_token_del(tokens);
	ft_print_tokens(*tokens);
}

int	main(void)
{
	char	*str;
	t_token	*tokens;

	tokens = 0;
	while (42)
	{
		str = readline("minishell : ");
		add_history(str);
		ft_lexer(&tokens, str);
		ft_tokens_init(&tokens);
		free(str);
	}
	return (0);
}
