/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:38:56 by sanahn            #+#    #+#             */
/*   Updated: 2022/12/01 14:06:05 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define TYPE_CHUNK     0
# define TYPE_TOKEN     1
# define TYPE_IO        2
# define TYPE_CMD_TABLE 3
# define TYPE_CMD       4

# define TYPE_ENTITY    5

# define TYPE_WORD              0
# define TYPE_WORD_QUTOES       1
# define TYPE_METACHAR_BLANK    2
# define TYPE_METACHAR_OPERATOR 3

# define TYPE_OPERATOR_PIPE   3
# define TYPE_OPERATOR_ERROR  4
# define TYPE_OPERATOR_LESS   5
# define TYPE_OPERATOR_DLESS  6
# define TYPE_OPERATOR_GREAT  7
# define TYPE_OPERATOR_DGREAT 8

# include <stdio.h>
# include <stdlib.h>
# include "libft/libft.h"

typedef struct s_chunk
{
	char			*content;
	struct s_chunk	*next;
}	t_chunk;

typedef struct s_token
{
	int				type;
	char			*content;
	t_chunk			*chunks;
	struct s_token	*next;
}	t_token;

typedef struct s_io
{
	int			fd;
	int			type;
	t_token		*file;
	struct s_io	*next;
}	t_io;

typedef struct s_cmd_table
{
	t_token				*arguments;
	t_io				*input;
	t_io				*output;
	t_token				*op_error;
	struct s_cmd_table	*next;
}	t_cmd_table;

typedef struct s_cmd
{
	char			**arguments;
	t_io			*input;
	t_io			*output;
	char			**op_error;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_entity
{
	char			*name;
	t_token			*value;
	struct s_entity	*next;
}	t_entity;

typedef struct s_env
{
	t_entity	*temp;
	t_entity	*local;
}	t_env;

#endif
