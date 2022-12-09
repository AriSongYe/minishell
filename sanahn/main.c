/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:43:42 by sanahn            #+#    #+#             */
/*   Updated: 2022/12/09 15:37:20 by sanahn           ###   ########.fr       */
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

void	ft_print_err_msg(char *msg)
{
	size_t	i;

	write(2, "minishell: ", 11);
	i = 0;
	while (msg[i])
	{
		write(2, msg + i, 1);
		i++;
	}
	write(2, "\n", 1);
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
		// 	printf("%-6s[%d] -> ", temp->content, tokens->type);
		// 	temp = temp->next;
		// }
		// printf("(null)\n");
		while (i++ < level)
			printf("  ");
		printf("%-6s\n", tokens->chunks->content);
		tokens = tokens->next;
	}
}

t_chunk	*ft_chunk_new(char *content)
{
	t_chunk	*res;

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

void	ft_chunk_del(t_token *token, int is_all)
{
	t_chunk	*temp_next;

	if (token == 0 || token->chunks == 0)
		return ;
	temp_next = token->chunks->next;
	if (is_all)
		free(token->chunks->content);
	free(token->chunks);
	token->chunks = temp_next;
}

void	ft_token_del(t_token **tokens, int is_all)
{
	void	*temp_next;

	if (*tokens == 0)
		return ;
	if (is_all)
	{
		while ((*tokens)->chunks)
		{
			temp_next = (*tokens)->chunks->next;
			free((*tokens)->chunks->content);
			free((*tokens)->chunks);
			free((*tokens)->content);
			(*tokens)->chunks = temp_next;
		}
	}
	temp_next = (*tokens)->next;
	free(*tokens);
	*tokens = temp_next;
}

void	ft_tokens_init(t_token **tokens)
{
	while (*tokens)
		ft_token_del(tokens, 1);
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
		while (str[*i] && ft_get_metachar_type(str[*i]) == type \
			&& str[*i] != '|')
			(*i)++;
	content = ft_strndup(str + start, *i - start);
	if (content == 0)
		return (1);
	new = ft_token_new(content, type);
	if (new == 0)
	{
		free(content);
		return (1);
	}
	ft_lst_add_back((void **)tokens, (void *)new, TYPE_TOKEN);
	return (0);
}

int	ft_tokenize_quotes(t_token **tokens, const char *str, size_t *i)
{
	char	quotes;
	size_t	start;
	t_token	*new;
	char	*content;

	quotes = str[*i];
	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != quotes)
		(*i)++;
	if (str[*i] == 0)
		return (1);
	(*i)++;
	content = ft_strndup(str + start, *i - start);
	if (content == 0)
		return (1);
	new = ft_token_new(content, TYPE_WORD);
	if (new == 0)
	{
		free(content);
		return (1);
	}
	ft_lst_add_back((void **)tokens, (void *)new, TYPE_TOKEN);
	return (0);
}

int	ft_tokenize_word(t_token **tokens, const char *str, size_t *i)
{
	t_token	*res;
	int		is_error;

	res = 0;
	while (str[*i] && ft_get_metachar_type(str[*i]) == TYPE_WORD)
	{
		if (str[*i] == '\'' || str[*i] == '"')
			is_error = ft_tokenize_quotes(&res, str, i);
		else
			is_error = ft_tokenize_type(&res, str, TYPE_WORD, i);
		if (is_error)
		{
			ft_tokens_init(&res);
			return (1);
		}
	}
	while (res->next)
	{
		ft_lst_add_back(\
			(void **)&res->chunks, (void *)res->next->chunks, TYPE_CHUNK);
		ft_token_del(&res->next, 0);
	}
	ft_lst_add_back((void **)tokens, (void *)res, TYPE_TOKEN);
	return (0);
}

int	ft_tokenize_pipe(t_token **tokens, const char *str, size_t *i)
{
	size_t	start;
	char	*content;
	t_token	*new;

	start = *i;
	if (str[*i] == '|')
		(*i)++;
	content = ft_strndup(str + start, *i - start);
	if (content == 0)
		return (1);
	new = ft_token_new(content, TYPE_METACHAR_OPERATOR);
	if (new == 0)
	{
		free(content);
		return (1);
	}
	ft_lst_add_back((void **)tokens, (void *)new, TYPE_TOKEN);
	return (0);
}

int	ft_tokenize(t_token **tokens, const char *str)
{
	size_t	i;
	int		err_num;
	int		type;

	if (*str == 0)
		return (0);
	i = 0;
	while (str[i] && ft_get_metachar_type(str[i]) == TYPE_METACHAR_BLANK)
		i++;
	while (str[i])
	{
		type = ft_get_metachar_type(str[i]);
		if (type == TYPE_WORD)
			err_num = ft_tokenize_word(tokens, str, &i);
		else if (str[i] == '|')
			err_num = ft_tokenize_pipe(tokens, str, &i);
		else
			err_num = ft_tokenize_type(tokens, str, type, &i);
		if (err_num)
		{
			ft_tokens_init(tokens);
			return (err_num);
		}
	}
	return (0);
}

//----------------------------------------------------------------------------------------------------------------

int	ft_get_operator_type(t_token *token)
{
	char	*content;

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
				tokens->type = TYPE_WORD_QUOTES;
			else if (tokens->chunks->next)
				tokens->type = TYPE_WORD_QUOTES;
		}
		tokens = tokens->next;
	}
}

void	ft_blank_token_del(t_token **tokens)
{
	t_token	*temp;

	if (*tokens == 0)
		return ;
	if ((*tokens)->type == TYPE_METACHAR_BLANK)
		ft_token_del(tokens, 1);
	temp = *tokens;
	while (temp && temp->next)
	{
		if (temp->next->type == TYPE_METACHAR_BLANK)
			ft_token_del(&temp->next, 1);
		temp = temp->next;
	}
}

int	ft_lexer(t_token **tokens, const char *str)
{
	int	err_num;

	if (tokens == 0 || str == 0)
		return (1);
	*tokens = 0;
	err_num = ft_tokenize(tokens, str);
	if (err_num)
		return (err_num);
	ft_token_set_type(*tokens);
	ft_blank_token_del(tokens);
	return (0);
}

//
// ----------------------------------------------------------------------------------------------------------------

t_token	*ft_token_pop(t_token **tokens)
{
	t_token	*res;

	if (*tokens == 0)
		return (0);
	res = *tokens;
	*tokens = (*tokens)->next;
	res->next = 0;
	return (res);
}

void	ft_entity_add_back(t_entity **entities, t_entity *new)
{
	t_entity	*temp;

	temp = *entities;
	while (temp)
	{
		if (ft_strncmp(temp->name, new->name, ft_strlen(new->name) + 1) == 0)
		{
			ft_token_del(&temp->value, 1);
			temp->value = new->value;
			free(new->name);
			free(new);
			return ;
		}
		temp = temp->next;
	}
	ft_lst_add_back((void **)entities, (void *)new, TYPE_ENTITY);
}

int	ft_env_add_back(t_env **envs, t_entity *new, int to_temp)
{
	if (*envs == 0)
	{
		*envs = (t_env *)ft_calloc(1, sizeof(t_env));
		if (*envs == 0)
			return (1);
		ft_lst_add_back(\
			(void **)&(*envs)->temp, (void *)new, TYPE_ENTITY);
		return (0);
	}
	if (to_temp)
		ft_entity_add_back(&(*envs)->temp, new);
	else
		ft_entity_add_back(&(*envs)->local, new);
	return (0);
}

int	ft_get_content_entity(char **name, t_token *value)
{
	char	*addr_equal;
	char	*temp_content;

	addr_equal = ft_strchr(value->chunks->content, '=');
	*addr_equal = 0;
	*name = ft_strdup(value->chunks->content);
	if (*name == 0)
		return (1);
	if (*(addr_equal + 1) == 0)
	{
		if (value->chunks->next == 0)
		{
			free(value->chunks->content);
			value->chunks->content = 0;
		}
		else
			ft_chunk_del(value, 1);
		return (0);
	}
	temp_content = ft_strdup(addr_equal + 1);
	if (temp_content == 0)
		return (1);
	free(value->chunks->content);
	value->chunks->content = temp_content;
	return (0);
}

t_entity	*ft_entity_new_token(t_token *token)
{
	t_entity	*res;
	char		*name;

	res = (t_entity *)ft_calloc(1, sizeof(t_entity));
	if (res == 0)
		return (0);
	if (ft_get_content_entity(&name, token))
	{
		free(name);
		return (0);
	}
	res->name = name;
	res->value = token;
	res->next = 0;
	return (res);
}

int	ft_is_env_assignment_syntax(t_token *token)
{
	char	*content;
	size_t	i;

	if (token->type != TYPE_WORD && token->type != TYPE_WORD_QUOTES)
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

int	ft_assign_env_prefix(t_token **tokens, t_env **envs)
{
	t_token		*temp;
	t_entity	*new;

	while (*tokens && ft_is_env_assignment_syntax(*tokens))
	{
		temp = ft_token_pop(tokens);
		new = ft_entity_new_token(temp);
		if (new == 0)
		{
			ft_token_del(&temp, 1);
			return (1);
		}
		if (ft_env_add_back(envs, new, 1))
		{
			free(new->name);
			ft_token_del(&new->value, 1);
			free(new);
			return (1);
		}
	}
	return (0);
}

void	ft_print_err_msg_op_args(char *op)
{
	if (*op == '|')
		ft_print_err_msg("syntax error near unexpected token `|'");
	else if (*op == '<')
	{
		if (op[1] == '<')
			ft_print_err_msg("syntax error near unexpected token `<<'");
		else
			ft_print_err_msg("syntax error near unexpected token `<'");
	}
	else if (*op == '>')
	{
		if (op[1] == '>')
			ft_print_err_msg("syntax error near unexpected token `>>'");
		else
			ft_print_err_msg("syntax error near unexpected token `>'");
	}
}

int	ft_set_io(t_cmd_table *cmd_table, t_token **tokens)
{
	t_io	*res;
	t_token	*temp;

	res = (t_io *)ft_calloc(1, sizeof(t_io));
	if (res == 0)
		return (1);
	res->type = (*tokens)->type;
	temp = ft_token_pop(tokens);
	ft_token_del(&temp, 1);
	if (*tokens == 0)
		ft_print_err_msg("syntax error near unexpected token `newline'");
	else if ((*tokens)->type >= TYPE_METACHAR_OPERATOR)
		ft_print_err_msg_op_args((*tokens)->chunks->content);
	if (*tokens == 0 || (*tokens)->type >= TYPE_METACHAR_OPERATOR)
	{
		free(res);
		return (2);
	}
	res->file = ft_token_pop(tokens);
	if (res->type <= TYPE_OPERATOR_DLESS)
		ft_lst_add_back((void **)&(cmd_table->input), (void *)res, TYPE_IO);
	else
		ft_lst_add_back((void **)&(cmd_table->output), (void *)res, TYPE_IO);
	return (0);
}

void	ft_cmd_tables_init(t_cmd_table **cmd_tables)
{
	void	*temp_next;

	if (*cmd_tables == 0)
		return ;
	while (*cmd_tables)
	{
		ft_tokens_init(&(*cmd_tables)->arguments);
		ft_tokens_init(&(*cmd_tables)->op_error);
		while ((*cmd_tables)->input)
		{
			temp_next = (*cmd_tables)->input->next;
			ft_tokens_init(&(*cmd_tables)->input->file);
			(*cmd_tables)->input = temp_next;
		}
		while ((*cmd_tables)->output)
		{
			temp_next = (*cmd_tables)->output->next;
			ft_tokens_init(&(*cmd_tables)->output->file);
			(*cmd_tables)->output = temp_next;
		}
		temp_next = (*cmd_tables)->next;
		free(*cmd_tables);
		*cmd_tables = temp_next;
	}
}

void	ft_print_err_msg_op(char *op)
{
	char	first_op;

	first_op = *op;
	if (op[1] == first_op)
	{
		if (op[2] == '<' && op[3] && op[3] == '<')
			ft_print_err_msg("syntax error near unexpected token `<<'");
		else if (op[2] == '<')
			ft_print_err_msg("syntax error near unexpected token `<'");
		else if (op[2] == '>' && op[3] && op[3] == '>')
			ft_print_err_msg("syntax error near unexpected token `>>'");
		else if (op[2] == '>')
			ft_print_err_msg("syntax error near unexpected token `>'");
	}
	else if (op[1] != first_op && op[2] && op[2] != first_op)
	{
		if (first_op == '<')
			ft_print_err_msg("syntax error near unexpected token `>>'");
		else
			ft_print_err_msg("syntax error near unexpected token `<<'");
	}
	else if (first_op == '<')
		ft_print_err_msg("syntax error near unexpected token `>'");
	else
		ft_print_err_msg("syntax error near unexpected token `<'");
}

int	ft_get_content_cmd_table(t_cmd_table *cmd_table, t_token **tokens)
{
	int	err_num;

	while (*tokens && (*tokens)->type != TYPE_OPERATOR_PIPE)
	{
		if ((*tokens)->type == TYPE_WORD || (*tokens)->type == TYPE_WORD_QUOTES)
			ft_lst_add_back((void **)&cmd_table->arguments, \
				(void *)ft_token_pop(tokens), TYPE_TOKEN);
		else if ((*tokens)->type == TYPE_OPERATOR_ERROR)
		{
			ft_print_err_msg_op((*tokens)->chunks->content);
			return (2);
		}
		else
		{
			err_num = ft_set_io(cmd_table, tokens);
			if (err_num)
				return (err_num);
		}
	}
	return (0);
}

t_cmd_table	*ft_cmd_table_new(t_token **tokens, int *err_num)
{
	t_cmd_table	*res;

	if ((*tokens)->type == TYPE_OPERATOR_PIPE)
	{
		*err_num = 2;
		ft_print_err_msg("syntax error near unexpected token `|'");
		return (0);
	}
	res = (t_cmd_table *)ft_calloc(1, sizeof(t_cmd_table));
	if (res == 0)
	{
		*err_num = 1;
		return (0);
	}
	*err_num = ft_get_content_cmd_table(res, tokens);
	if (*err_num)
	{
		ft_cmd_tables_init(&res);
		return (0);
	}
	return (res);
}

int	ft_add_cmd_line(t_token **tokens)
{
	char	*str;

	while (*tokens == 0)
	{
		str = readline("> ");
		ft_lexer(tokens, str);
		free(str);
	}
	return (0);
}

int	ft_set_cmd_tables(t_token **tokens, t_cmd_table **cmd_tables)
{
	t_cmd_table	*temp_table;
	int			err_num;
	t_token		*temp;

	err_num = 0;
	temp_table = ft_cmd_table_new(tokens, &err_num);
	if (temp_table == 0)
		return (err_num);
	ft_lst_add_back((void **)cmd_tables, (void *)temp_table, TYPE_CMD_TABLE);
	while (*tokens)
	{
		if ((*tokens)->type == TYPE_OPERATOR_PIPE)
		{
			temp = ft_token_pop(tokens);
			ft_token_del(&temp, 1);
			if (*tokens == 0 && ft_add_cmd_line(tokens))
				return (1);
			temp_table = ft_cmd_table_new(tokens, &err_num);
			if (temp_table == 0)
				return (err_num);
			ft_lst_add_back(\
				(void **)cmd_tables, (void *)temp_table, TYPE_CMD_TABLE);
		}
	}
	return (0);
}

int	ft_parser(t_token **tokens, t_cmd_table **cmd_tables, t_env **envs)
{
	int	err_num;

	if (tokens == 0 || cmd_tables == 0 || envs == 0)
		return (1);
	*cmd_tables = 0;
	if (*tokens == 0)
		return (0);
	err_num = ft_assign_env_prefix(tokens, envs);
	if (err_num)
		return (err_num);
	if (*tokens == 0)
		return (0);
	err_num = ft_set_cmd_tables(tokens, cmd_tables);
	if (err_num)
		return (err_num);
	return (0);
}

//-----------------------------------------------------------------------------------------------------------------

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
				printf("  type  : %d\n", temp_io->type);
				printf("  files : ");
				ft_print_tokens(temp_io->file, 0);
				printf("\n");
				temp_io = temp_io->next;
			}
		}
		if (cmd_tables->output)
		{
			printf("output\n");
			temp_io = cmd_tables->output;
			while (temp_io)
			{
				printf("  type  : %d\n", temp_io->type);
				printf("  files : ");
				ft_print_tokens(temp_io->file, 0);
				printf("\n");
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
		if (local_envs == 0 || local_envs->value->chunks->content == 0)
			return ((char *)ft_calloc(1, sizeof(char)));
		return (ft_strdup(local_envs->value->chunks->content));
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

int	ft_dollar_handler(t_chunk *chunk, t_entity *local_envs)
{
	char	*res;
	char	*temp_res;
	char	*temp_str;
	size_t	i;

	if (chunk->content == 0 || \
		chunk->content[0] == '\'' || ft_strchr(chunk->content, '$') == 0)
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
	chunk->content = res;
	if (ft_strlen(chunk->content) == 0)
	{
		free(chunk->content);
		chunk->content = 0;
	}
	return (0);
}

int	ft_token_iteri(\
	t_token *tokens, t_entity *local_envs, int (*fp)(t_chunk *, t_entity *))
{
	t_chunk	*temp;
	int		err_num;

	while (tokens)
	{
		temp = tokens->chunks;
		while (temp)
		{
			err_num = fp(temp, local_envs);
			if (err_num)
				return (err_num);
			temp = temp->next;
		}
		tokens = tokens->next;
	}
	return (0);
}

int	ft_io_iteri(t_io *ios, t_entity *local_envs, int apply_heredoc, \
	int (*fp)(t_chunk *, t_entity *))
{
	int	err_num;

	while (ios)
	{
		if (ios->type == TYPE_OPERATOR_DLESS && apply_heredoc)
		{
			err_num = ft_token_iteri(ios->file, local_envs, fp);
			if (err_num)
				return (err_num);
		}
		ios = ios->next;
	}
	return (0);
}

int	ft_cmd_table_iteri(t_cmd_table *cmd_tables, t_env *envs, \
	int apply_heredoc, int (*fp)(t_chunk *, t_entity *))
{
	int			err_num;
	t_entity	*local_envs;

	local_envs = 0;
	if (envs)
		local_envs = envs->local;
	while (cmd_tables)
	{
		err_num = ft_token_iteri(cmd_tables->arguments, local_envs, fp);
		if (err_num)
			return (err_num);
		err_num = ft_io_iteri(cmd_tables->input, local_envs, apply_heredoc, fp);
		if (err_num)
			return (err_num);
		err_num = ft_io_iteri(\
			cmd_tables->output, local_envs, apply_heredoc, fp);
		if (err_num)
			return (err_num);
		cmd_tables = cmd_tables->next;
	}
	return (0);
}

int	ft_env_iteri(t_env *envs, int (*fp)(t_chunk *, t_entity *))
{
	int			err_num;
	t_entity	*temp_envs;
	t_entity	*local_envs;

	if (envs == 0)
		return (0);
	temp_envs = envs->temp;
	local_envs = envs->local;
	while (temp_envs)
	{
		err_num = ft_token_iteri(temp_envs->value, local_envs, fp);
		if (err_num)
			return (err_num);
		temp_envs = temp_envs->next;
	}
	return (0);
}

int	ft_remove_quotes(t_chunk *chunk, t_entity *local_envs)
{
	char	*temp_str;

	(void)local_envs;
	if (chunk->content == 0)
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

int	ft_join_chunks(t_chunk *chunk, t_entity *local_envs)
{
	t_chunk	*temp_next;
	char	*temp_content;

	(void)local_envs;
	if (chunk->next == 0)
		return (0);
	while (chunk->next)
	{
		temp_next = chunk->next->next;
		if (chunk->content == 0)
			chunk->content = (char *)ft_calloc(1, sizeof(char *));
		temp_content = ft_strjoin(chunk->content, chunk->next->content);
		if (temp_content == 0)
			return (1);
		free(chunk->content);
		free(chunk->next->content);
		free(chunk->next);
		chunk->content = temp_content;
		chunk->next = temp_next;
	}
	return (0);
}

int	ft_expander(t_cmd_table *cmd_tables, t_env *envs)
{
	int	err_num;

	if (cmd_tables == 0 && envs == 0)
		return (0);
	err_num = ft_cmd_table_iteri(cmd_tables, envs, 0, ft_dollar_handler);
	if (err_num)
		return (err_num);
	err_num = ft_cmd_table_iteri(cmd_tables, envs, 1, ft_remove_quotes);
	if (err_num)
		return (err_num);
	err_num = ft_cmd_table_iteri(cmd_tables, envs, 1, ft_join_chunks);
	if (err_num)
		return (err_num);
	err_num = ft_env_iteri(envs, ft_dollar_handler);
	if (err_num)
		return (err_num);
	err_num = ft_env_iteri(envs, ft_remove_quotes);
	if (err_num)
		return (err_num);
	err_num = ft_env_iteri(envs, ft_join_chunks);
	if (err_num)
		return (err_num);
	return (0);
}

t_entity	*ft_entity_pop(t_entity **entities)
{
	t_entity	*res;

	if (entities == 0 || *entities == 0)
		return (0);
	res = *entities;
	*entities = (*entities)->next;
	res->next = 0;
	return (res);
}

int	ft_save_env(t_env **envs, t_cmd_table *cmd_tables)
{
	t_entity	*temp_entity;

	if (envs == 0 || *envs == 0)
		return (0);
	if (cmd_tables == 0)
	{
		while ((*envs)->temp)
			ft_env_add_back(envs, ft_entity_pop(&(*envs)->temp), 0);
	}
	else
	{
		while ((*envs)->temp)
		{
			temp_entity = (*envs)->temp->next;
			free((*envs)->temp->name);
			ft_token_del(&((*envs)->temp->value), 1);
			(*envs)->temp = temp_entity;
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
	int			err_num;

	envs = 0;
	while (42)
	{
		str = readline("minishell : ");
		add_history(str);
		ft_lexer(&tokens, str);
		err_num = ft_parser(&tokens, &cmd_tables, &envs);
		if (err_num)
			return (err_num);
		ft_expander(cmd_tables, envs);
		// ft_print_tokens(tokens, 1);
		ft_save_env(&envs, cmd_tables);
		ft_print_envs(envs);
		ft_print_cmd_tables(cmd_tables);
		// ft_tokens_init(&tokens);
		ft_cmd_tables_init(&cmd_tables);
		free(str);
	}
	return (0);
}
