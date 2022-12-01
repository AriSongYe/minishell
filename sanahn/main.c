/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:43:42 by sanahn            #+#    #+#             */
/*   Updated: 2022/12/01 16:48:05 by sanahn           ###   ########.fr       */
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

void	ft_print_tokens(t_token *tokens, int level)
{
	// t_chunk	*temp;
	int		i;

	if (tokens == 0)
		return ;
	while (tokens)
	{
		i = 0;
		// temp = tokens->chunks;
		// while (temp)
		// {
		// 	while (i++ < level)
		// 		printf("  ");
		// 	printf("%-6s -> ", temp->content);
		// 	temp = temp->next;
		// }
		// printf("(null)\n");
		while (i++ < level)
			printf("  ");
		printf("%-6s\n", tokens->content);
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
		free(token->content);
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
		while (((t_chunk *)res)->next)
			res = ((t_chunk *)res)->next;
	else if (data_type == TYPE_TOKEN)
		while (((t_token *)res)->next)
			res = ((t_token *)res)->next;
	else if (data_type == TYPE_IO)
		while (((t_io *)res)->next)
			res = ((t_io *)res)->next;
	else if (data_type == TYPE_CMD_TABLE)
		while (((t_cmd_table *)res)->next)
			res = ((t_cmd_table *)res)->next;
	else if (data_type == TYPE_CMD)
		while (((t_cmd *)res)->next)
			res = ((t_cmd *)res)->next;
	else
		while (((t_entity *)res)->next)
			res = ((t_entity *)res)->next;
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
	else if (data_type == TYPE_IO)
		((t_io *)ft_get_last_node(*lst, data_type))->next = new;
	else if (data_type == TYPE_CMD_TABLE)
		((t_cmd_table *)ft_get_last_node(*lst, data_type))->next = new;
	else if (data_type == TYPE_CMD)
		((t_cmd *)ft_get_last_node(*lst, data_type))->next = new;
	else
		((t_entity *)ft_get_last_node(*lst, data_type))->next = new;
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
	if (*content == '<')
	{
		if (ft_strncmp(content, "<", 2) == 0)
			return (TYPE_OPERATOR_LESS);
		if (ft_strncmp(content, "<<", 3) == 0)
			return (TYPE_OPERATOR_DLESS);
	}
	if (*content == '>')
	{
		if (ft_strncmp(content, ">", 2) == 0)
			return (TYPE_OPERATOR_GREAT);
		if (ft_strncmp(content, ">>", 3) == 0)
			return (TYPE_OPERATOR_DGREAT);
	}
	return (TYPE_OPERATOR_ERROR);
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
	ft_blank_token_del(tokens);
}

// ----------------------------------------------------------------------------------------------------------------

t_token	*ft_token_pop(t_token **tokens)
{
	t_token	*res;

	if (tokens == 0 || *tokens == 0)
		return (0);
	res = *tokens;
	*tokens = res->next;
	res->next = 0;
	return (res);
}

t_env	*ft_env_new(t_entity *entity)
{
	t_env	*res;

	if (entity == 0)
		return (0);
	res = (t_env *)ft_calloc(1, sizeof(t_env));
	if (res == 0)
		return (0);
	ft_lst_add_back((void **)&(res->temp), (void *)entity, TYPE_ENTITY);
	return (res);
}

t_entity	*ft_entity_new(char *name, t_token *value)
{
	t_entity	*res;

	if (name == 0 || value == 0)
		return (0);
	res = (t_entity *)ft_calloc(1, sizeof(t_entity));
	if (res == 0)
		return (0);
	res->name = name;
	res->value = value;
	res->next = 0;
	return (res);
}

int	ft_is_env_assign_syntax(t_token *token)
{
	char	*content;
	size_t	i;

	if (token == 0 || token->type >= TYPE_METACHAR_OPERATOR || \
		token->type == TYPE_METACHAR_BLANK)
		return (0);
	content = token->chunks->content;
	if (ft_strchr(content, '=') == 0)
		return (0);
	if (*content != '_' && ft_isalpha(*content) == 0)
		return (0);
	i = 1;
	while (content[i] && (ft_isalnum(content[i]) || content[i] == '_'))
		i++;
	if (content[i] != '=')
		return (0);
	return (1);
}

int	ft_get_content_env(char **name, t_token *value)
{
	char	*pchar_equal;
	char	*temp_content;
	t_chunk	*temp_chunk;

	pchar_equal = ft_strchr(value->chunks->content, '=');
	*pchar_equal = 0;
	*name = ft_strdup(value->chunks->content);
	if (*name == 0)
		return (1);
	temp_chunk = 0;
	if (*(pchar_equal + 1) == 0)
	{
		temp_chunk = value->chunks->next;
		free(value->chunks->content);
		free(value->chunks);
		value->chunks = temp_chunk;
		return (0);
	}
	temp_content = ft_strdup(pchar_equal + 1);
	if (temp_content == 0)
	{
		free(*name);
		*name = 0;
		return (1);
	}
	free(value->chunks->content);
	value->chunks->content = temp_content;
	return (0);
}

void	ft_env_add_back(t_entity **entitys, t_entity *new)
{
	t_entity	*temp_entity;

	temp_entity = *entitys;
	while (temp_entity)
	{
		if (!ft_strncmp(temp_entity->name, new->name, ft_strlen(new->name) + 1))
		{
			ft_token_del(temp_entity->value);
			temp_entity->value = new->value;
			free(new->name);
			free(new);
			return ;
		}
		temp_entity = temp_entity->next;
	}
	ft_lst_add_back((void **)entitys, (void *)new, TYPE_ENTITY);
}

int	ft_assign_env(t_token **tokens, t_env **envs)
{
	char		*name;
	t_token		*temp_token;
	t_entity	*new_entity;

	if (tokens == 0 || envs == 0)
		return (0);
	while (*tokens && ft_is_env_assign_syntax(*tokens))
	{
		temp_token = ft_token_pop(tokens);
		ft_get_content_env(&name, temp_token);
		new_entity = ft_entity_new(name, temp_token);
		if (*envs == 0)
			*envs = ft_env_new(new_entity);
		else
			ft_env_add_back(&((*envs)->temp), new_entity);
		if (*envs == 0)
		{
			free(name);
			ft_token_del(temp_token);
			free(new_entity);
			return (1);
		}
	}
	return (0);
}

int	ft_set_io(t_cmd_table *cmd_table, t_token **tokens, int *err_num)
{
	t_io	*res;

	res = (t_io *)ft_calloc(1, sizeof(t_io));
	if (res == 0)
	{
		*err_num = 1;
		return (1);
	}
	res->type = (*tokens)->type;
	ft_token_del(ft_token_pop(tokens));
	res->file = ft_token_pop(tokens);
	if (res->file == 0)
	{
		free(res);
		*err_num = 2;
		return (1);
	}
	if (res->type <= TYPE_OPERATOR_DLESS)
		ft_lst_add_back((void **)&(cmd_table->input), (void *)res, TYPE_IO);
	else
		ft_lst_add_back((void **)&(cmd_table->output), (void *)res, TYPE_IO);
	return (0);
}

t_cmd_table	*ft_cmd_table_new(t_token **tokens, int *err_num)
{
	t_cmd_table	*res;

	res = (t_cmd_table *)ft_calloc(1, sizeof(t_cmd_table));
	if (res == 0)
		*err_num = 1;
	if (*tokens && (*tokens)->type == TYPE_OPERATOR_PIPE)
	{
		free(res);
		*err_num = 2;
	}
	if (*err_num)
		return (0);
	while (*tokens && (*tokens)->type != TYPE_OPERATOR_PIPE)
	{
		if ((*tokens)->type == TYPE_WORD || (*tokens)->type == TYPE_WORD_QUTOES)
			ft_lst_add_back((void **)&(res->arguments), \
				(void *)ft_token_pop(tokens), TYPE_TOKEN);
		else if ((*tokens)->type == TYPE_OPERATOR_ERROR)
			ft_lst_add_back((void **)&(res->op_error), \
				(void *)ft_token_pop(tokens), TYPE_TOKEN);
		else if (ft_set_io(res, tokens, err_num))
			return (0);
	}
	return (res);
}

int	ft_set_cmd_tables(t_token **tokens, t_cmd_table **cmd_tables)
{
	t_cmd_table	*temp_table;
	t_token		*temp_token;
	int			err_num;

	if (tokens == 0 || cmd_tables == 0)
		return (0);
	err_num = 0;
	temp_table = ft_cmd_table_new(tokens, &err_num);
	if (temp_table == 0)
		return (err_num);
	ft_lst_add_back((void **)cmd_tables, (void *)temp_table, TYPE_CMD_TABLE);
	while (*tokens)
	{
		if ((*tokens)->type == TYPE_OPERATOR_PIPE)
		{
			temp_token = ft_token_pop(tokens);
			temp_table = ft_cmd_table_new(tokens, &err_num);
			if (temp_table == 0)
				return (err_num);
			ft_lst_add_back(\
				(void **)cmd_tables, (void *)temp_table, TYPE_CMD_TABLE);
			ft_token_del(temp_token);
		}
	}
	return (0);
}

void	ft_parser(t_token **tokens, t_cmd_table **cmd_tables, t_env **envs)
{
	ft_assign_env(tokens, envs);
	ft_set_cmd_tables(tokens, cmd_tables);
}

void	ft_print_cmd_tables(t_cmd_table *cmd_tables)
{
	t_io	*temp_io;

	while (cmd_tables)
	{
		printf("------------------------------\n");
		if (cmd_tables->arguments)
		{
			printf("arguments\n");
			ft_print_tokens(cmd_tables->arguments, 1);
		}
		if (cmd_tables->input)
		{
			printf("input\n");
			temp_io = cmd_tables->input;
			while (temp_io)
			{
				printf("  type : %d\n", temp_io->type);
				printf("  files\n");
				ft_print_tokens(temp_io->file, 2);
				temp_io = temp_io->next;
			}
		}
		if (cmd_tables->output)
		{
			printf("output\n");
			temp_io = cmd_tables->output;
			while (temp_io)
			{
				printf("  type : %d\n", temp_io->type);
				printf("  files\n");
				ft_print_tokens(temp_io->file, 2);
				temp_io = temp_io->next;
			}
		}
		if (cmd_tables->op_error)
		{
			printf("op_error\n");
			ft_print_tokens(cmd_tables->op_error, 1);
		}
		printf("------------------------------\n");
		cmd_tables = cmd_tables->next;
	}
}

void	ft_print_envs(t_env *envs)
{
	t_entity	*temp_entity;

	if (envs == 0)
	{
		printf("NULL\n");
		return ;
	}
	if (envs->local || envs->temp)
	{
		printf("------------------------------\n");
		printf("envs\n");
	}
	if (envs->temp)
	{
		printf("temp\n");
		temp_entity = envs->temp;
		while (temp_entity)
		{
			printf("  %s=", temp_entity->name);
			ft_print_tokens(temp_entity->value, 0);
			temp_entity = temp_entity->next;
		}
	}
	if (envs->local)
	{
		printf("local\n");
		temp_entity = envs->local;
		while (temp_entity)
		{
			printf("  %s=", temp_entity->name);
			ft_print_tokens(temp_entity->value, 0);
			temp_entity = temp_entity->next;
		}
	}
}

void	ft_cmd_tables_init(t_cmd_table **cmd_tables)
{
	t_io	*temp_io;

	if (cmd_tables == 0)
		return ;
	ft_tokens_init(&((*cmd_tables)->arguments));
	ft_tokens_init(&((*cmd_tables)->op_error));
	temp_io = (*cmd_tables)->input;
	while ((*cmd_tables)->input)
	{
		temp_io = (*cmd_tables)->input->next;
		ft_tokens_init(&((*cmd_tables)->input->file));
		free((*cmd_tables)->input);
		(*cmd_tables)->input = temp_io;
	}
	while ((*cmd_tables)->output)
	{
		temp_io = (*cmd_tables)->output->next;
		ft_tokens_init(&((*cmd_tables)->output->file));
		free((*cmd_tables)->output);
		(*cmd_tables)->output = temp_io;
	}
	*cmd_tables = 0;
}

// ----------------------------------------------------------------------------

char	*ft_get_content_dollar_env(char *str, t_entity *local_envs, size_t *i)
{
	size_t	start;
	char	*name;
	char	*res;

	start = *i;
	while (str[*i] && str[*i] != '$' && str[*i] != '"')
		(*i)++;
	name = ft_strndup(str + start, *i - start);
	if (name == 0)
		return (0);
	res = getenv(name);
	if (res == 0)
	{
		while (local_envs && \
			ft_strncmp(local_envs->name, name, ft_strlen(name) + 1))
			local_envs = local_envs->next;
		if (local_envs == 0 || local_envs->value->content == 0)
			return ((char *)ft_calloc(1, sizeof(char)));
		return (ft_strdup(local_envs->value->content));
	}
	return (ft_strdup(res));
}

char	*ft_get_errno_str(void)
{
	return (ft_strdup("errno"));
}

char	*ft_get_content_dollar(t_chunk *chunk, t_entity *local_envs, size_t *i)
{
	size_t	start;

	start = *i;
	if (chunk->content[start] != '$')
	{
		while (chunk->content[*i] && chunk->content[*i] != '$')
			(*i)++;
		return (ft_strndup(chunk->content + start, *i - start));
	}
	if (chunk->content[++(*i)] == '?' && ++(*i))
		return (ft_get_errno_str());
	if (chunk->content[*i] == 0)
	{
		if (chunk->next && \
			(chunk->next->content[0] == '\'' || chunk->next->content[0] == '"'))
			return ((char *)ft_calloc(1, sizeof(char)));
		return (ft_strdup("$"));
	}
	if (chunk->content[*i] == '$' && ++(*i))
		return ((char *)ft_calloc(1, sizeof(char)));
	return (ft_get_content_dollar_env(chunk->content, local_envs, i));
}

int	ft_dollar_expander(t_chunk *chunk, t_entity *local_envs)
{
	char	*res;
	char	*temp_res;
	char	*temp_str;
	size_t	i;

	if (chunk == 0 || chunk->content == 0 || chunk->content[0] == '\'' || \
		ft_strchr(chunk->content, '$') == 0)
		return (0);
	res = (char *)ft_calloc(1, sizeof(char));
	i = 0;
	while (chunk->content[i])
	{
		temp_str = ft_get_content_dollar(chunk, local_envs, &i);
		temp_res = ft_strjoin(res, temp_str);
		free(res);
		free(temp_str);
		if (temp_res == 0)
			return (1);
		res = temp_res;
	}
	free(chunk->content);
	if (res && ft_strlen(res) == 0)
	{
		free(res);
		res = 0;
	}
	chunk->content = res;
	return (0);
}

int	ft_token_iteri(\
	t_token *tokens, t_entity *local_envs, int (*fp)(t_chunk *, t_entity *))
{
	t_chunk	*temp_chunk;
	int		err_num;

	while (tokens)
	{
		temp_chunk = tokens->chunks;
		while (temp_chunk)
		{
			err_num = fp(temp_chunk, local_envs);
			if (err_num)
				return (err_num);
			temp_chunk = temp_chunk->next;
		}
		tokens = tokens->next;
	}
	return (0);
}

int	ft_cmd_table_iteri(t_cmd_table *cmd_tables, t_entity *local_envs, \
	int (*fp)(t_chunk *, t_entity *), int apply_heredoc)
{
	t_io	*temp_io;
	int		err_num;
	int		temp;

	err_num = 0;
	while (!err_num && cmd_tables)
	{
		err_num = ft_token_iteri(cmd_tables->arguments, local_envs, fp);
		temp = 0;
		while (!err_num && temp++ < 2)
		{
			temp_io = cmd_tables->input;
			if (temp == 2)
				temp_io = cmd_tables->output;
			while (!err_num && temp_io)
			{
				if (temp_io->type != TYPE_OPERATOR_DLESS || apply_heredoc)
					err_num = ft_token_iteri(temp_io->file, local_envs, fp);
				temp_io = temp_io->next;
			}
		}
		cmd_tables = cmd_tables->next;
	}
	return (err_num);
}

int	ft_env_value_iteri(\
	t_entity *temp_envs, t_entity *local_envs, int (*fp)(t_chunk *, t_entity *))
{
	int	err_num;

	while (temp_envs)
	{
		err_num = ft_token_iteri(temp_envs->value, local_envs, fp);
		if (err_num)
			return (err_num);
		temp_envs = temp_envs->next;
	}
	return (0);
}

int	ft_qutoes_remove(t_chunk *chunk, t_entity *local_envs)
{
	char	*temp_str;

	(void)local_envs;
	if (chunk == 0 || chunk->content == 0)
		return (0);
	if (chunk->content[0] == '\'')
	{
		temp_str = ft_strtrim(chunk->content, "'");
		if (temp_str == 0)
			return (1);
		free(chunk->content);
		chunk->content = temp_str;
		return (0);
	}
	if (chunk->content[0] == '"')
	{
		temp_str = ft_strtrim(chunk->content, "\"");
		if (temp_str == 0)
			return (1);
		free(chunk->content);
		chunk->content = temp_str;
		return (0);
	}
	return (0);
}

int	ft_join_chunks(t_token *token)
{
	t_chunk	*temp_chunk;
	char	*content;

	if (token == 0)
		return (0);
	while (token->chunks && token->chunks->next)
	{
		if (token->chunks->content == 0)
		{
			temp_chunk = token->chunks->next;
			free(token->chunks);
			token->chunks = temp_chunk;
			continue ;
		}
		if (token->chunks->next->content == 0)
		{
			temp_chunk = token->chunks->next->next;
			free(token->chunks->next);
			token->chunks->next = temp_chunk;
			continue ;
		}
		temp_chunk = token->chunks->next->next;
		content = ft_strjoin(token->chunks->content, token->chunks->next->content);
		if (content == 0)
			return (1);
		free(token->chunks->content);
		free(token->chunks->next->content);
		token->chunks->content = content;
		free(token->chunks->next);
		token->chunks->next = temp_chunk;
	}
	if (token->chunks)
	{
		token->content = token->chunks->content;
		free(token->chunks);
		token->chunks = 0;
	}
	return (0);
}

int	ft_token_table_env_iteri(\
	t_cmd_table *cmd_tables, t_entity *temp_envs, int (*fp)(t_token *))
{
	t_token		*temp_token;
	int			err_num;

	temp_token = cmd_tables->arguments;
	while (temp_token)
	{
		err_num = fp(temp_token);
		if (err_num)
			return (err_num);
		temp_token = temp_token->next;
	}
	if (cmd_tables->input)
	{
		temp_token = cmd_tables->input->file;
		while (temp_token)
		{
			err_num = fp(temp_token);
			if (err_num)
				return (err_num);
			temp_token = temp_token->next;
		}
	}
	if (cmd_tables->output)
	{
		temp_token = cmd_tables->output->file;
		while (temp_token)
		{
			err_num = fp(temp_token);
			if (err_num)
				return (err_num);
			temp_token = temp_token->next;
		}
	}
	while (temp_envs)
	{
		err_num = fp(temp_envs->value);
		if (err_num)
			return (err_num);
		temp_envs = temp_envs->next;
	}
	return (0);
}

int	ft_expander(t_cmd_table *cmd_tables, t_env *envs)
{
	int	err_num;

	if (cmd_tables == 0)
		return (0);
	if (envs == 0)
	{
		err_num = ft_cmd_table_iteri(cmd_tables, 0, ft_dollar_expander, 0);
		if (err_num)
			return (err_num);
		err_num = ft_cmd_table_iteri(cmd_tables, 0, ft_qutoes_remove, 1);
		if (err_num)
			return (err_num);
		err_num = ft_token_table_env_iteri(cmd_tables, 0, ft_join_chunks);
		if (err_num)
			return (err_num);
	}
	else
	{
		err_num = ft_cmd_table_iteri(\
			cmd_tables, envs->local, ft_dollar_expander, 0);
		if (err_num)
			return (err_num);
		err_num = ft_env_value_iteri(\
			envs->temp, envs->local, ft_dollar_expander);
		if (err_num)
			return (err_num);
		err_num = ft_cmd_table_iteri(\
			cmd_tables, envs->local, ft_qutoes_remove, 1);
		if (err_num)
			return (err_num);
		err_num = ft_env_value_iteri(\
			envs->temp, envs->local, ft_qutoes_remove);
		if (err_num)
			return (err_num);
		err_num = ft_token_table_env_iteri(cmd_tables, envs->temp, ft_join_chunks);
		if (err_num)
			return (err_num);
	}
	return (0);
}

t_entity	*ft_entity_pop(t_entity **entitys)
{
	t_entity	*res;

	if (entitys == 0 || *entitys == 0)
		return (0);
	res = *entitys;
	*entitys = res->next;
	res->next = 0;
	return (res);
}

int	ft_save_env(t_env *envs, t_cmd_table *cmd_tables)
{
	t_entity	*temp_entity;

	if (envs == 0)
		return (0);
	if (cmd_tables->arguments == 0 && cmd_tables->input == 0 && \
		cmd_tables->next == 0 && cmd_tables->op_error == 0 && \
		cmd_tables->output == 0)
	{
		while (envs->temp)
			ft_env_add_back(&(envs->local), ft_entity_pop(&(envs->temp)));
	}
	else
	{
		while (envs->temp)
		{
			temp_entity = envs->temp->next;
			free(envs->temp->name);
			ft_token_del(envs->temp->value);
			envs->temp = temp_entity;
		}
	}
	return (0);
}

int	main(void)
{
	char		*str;
	t_token		*tokens;
	t_env		*envs;
	t_cmd_table	*cmd_tables;
	// t_cmd		*cmds;

	tokens = 0;
	envs = 0;
	cmd_tables = 0;
	// cmds = 0;
	while (42)
	{
		str = readline("minishell : ");
		add_history(str);
		ft_lexer(&tokens, str);
		ft_parser(&tokens, &cmd_tables, &envs);
		ft_expander(cmd_tables, envs);
		// ft_print_tokens(tokens, 1);
		ft_print_envs(envs);
		ft_save_env(envs, cmd_tables);
		ft_print_cmd_tables(cmd_tables);
		// ft_tokens_init(&tokens);
		ft_cmd_tables_init(&cmd_tables);
		free(str);
	}
	return (0);
}
