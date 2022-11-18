/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:15:32 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 14:57:14 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static t_cmd	*ft_cmd_new(t_syntax *content)
{
	t_cmd	*res;

	if (content == 0)
		return (0);
	res = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (res == 0)
		return (0);
	res->syntax = content;
	res->next = 0;
	return (res);
}

static int	ft_cmd_add_back(t_cmd **cmds, t_cmd *new)
{
	t_cmd	*last;

	if (cmds == 0)
		return (1);
	if (*cmds == 0)
	{
		*cmds = new;
		return (0);
	}
	last = *cmds;
	while (last->next)
		last = last->next;
	last->next = new;
	return (0);
}

int	ft_set_cmd(t_cmd **cmds, t_token **tokens)
{
	t_token	*temp;

	if (cmds == 0 || tokens == 0)
		return (0);
	if (ft_cmd_add_back(cmds, ft_cmd_new(ft_syntax_new(tokens))))
		return (1);
	while (*tokens)
	{
		if (ft_get_operno(*tokens) == TYPE_OPER_PIPE)
		{
			temp = (*tokens)->next;
			ft_token_del(*tokens);
			*tokens = temp;
			if (ft_cmd_add_back(cmds, ft_cmd_new(ft_syntax_new(tokens))))
				return (1);
		}
	}
	return (0);
}

void	ft_cmds_init(t_cmd **cmds)
{
	t_cmd	*temp;

	if (cmds == 0)
		return ;
	while (*cmds)
	{
		temp = (*cmds)->next;
		ft_tokens_init(&((*cmds)->syntax->cmd));
		ft_tokens_init(&((*cmds)->syntax->args));
		ft_tokens_init(&((*cmds)->syntax->input));
		ft_tokens_init(&((*cmds)->syntax->output));
		ft_tokens_init(&((*cmds)->syntax->error));
		free((*cmds)->syntax);
		free(*cmds);
		*cmds = temp;
	}
	*cmds = 0;
}
