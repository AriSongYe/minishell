/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:38:56 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/22 17:41:53 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define TYPE_CHUNK     0
# define TYPE_TOKEN     1
# define TYPE_CMD_TABLE 2
# define TYPE_CMD       3

# define TYPE_WORD           0
# define TYPE_METACHAR_BLANK 1
# define TYPE_METACHAR_OPER  2

# define TYPE_TOKEN_WORD     0
# define TYPE_TOKEN_METACHAR 1

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
	t_chunk			*chunks;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd_table
{
	t_token	*command;
	t_token	*arguments;
	t_token	*input;
	t_token	*output;
	t_token	*error;
}	t_cmd_table;

typedef struct s_cmd
{
	t_cmd_table		*table;
	struct s_cmd	*next;
}	t_cmd;

#endif
