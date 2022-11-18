/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:38:56 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 15:32:35 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define TYPE_TOKEN_WORD     0
# define TYPE_TOKEN_OPERATOR 1
# define TYPE_TOKEN_METACHAR 2

# define TYPE_OPER_REDIRECT_IN  0
# define TYPE_OPER_HEREDOC      1
# define TYPE_OPER_REDIRECT_OUT 2
# define TYPE_OPER_APPEND       3
# define TYPE_OPER_PIPE         4

# include <stdio.h>
# include <stdlib.h>

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

typedef struct s_syntax
{
	t_token	*cmd;
	t_token	*args;
	t_token	*input;
	t_token	*output;
	t_token	*error;
}	t_syntax;

typedef struct s_cmd
{
	t_syntax		*syntax;
	struct s_cmd	*next;
}	t_cmd;

char		*ft_strndup(const char *s1, size_t size);
void		ft_print_tokens(t_token *tokens);
void		ft_print_cmds(t_cmd *cmds);
int			ft_get_operno(t_token *token);
t_token		*ft_token_pop(t_token **tokens);

t_chunk		*ft_chunk_new(char *content);
int			ft_chunk_add_back(t_token *token, t_chunk *new);
t_chunk		*ft_get_last_chunk(t_token *token);
int			ft_chunk_join(t_chunk *chunk);
int			ft_chunk_iteri(t_token *tokens, int (*fp)(t_chunk *));

t_token		*ft_token_new(char *content, int type);
int			ft_token_add_back(t_token **tokens, t_token *new);
t_token		*ft_get_last_token(t_token *tokens);
void		ft_token_del(t_token *token);
void		ft_tokens_init(t_token **tokens);

int			ft_is_metachar(const char c);
int			ft_get_prev_token_type(const char *str, size_t i);
int			ft_tokenize_metachar(t_token **tokens, const char *str, size_t *i);
void		ft_tokenize(t_token **tokens, const char *str);

int			ft_dollar_handler(t_chunk *chunk);
int			ft_qutoes_handler(t_chunk *chunk);
int			ft_del_empty_token(t_token **tokens);

t_syntax	*ft_syntax_new(t_token **tokens);
int			ft_set_cmd(t_cmd **cmds, t_token **tokens);
void		ft_cmds_init(t_cmd **cmds);
#endif
