/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:11:47 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 14:15:47 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static void	ft_get_content_syntax(t_token **syntax_tokens, t_token **tokens)
{
	if ((*tokens)->next)
	{
		ft_token_add_back(syntax_tokens, ft_token_pop(tokens));
		ft_token_add_back(syntax_tokens, ft_token_pop(tokens));
	}
	else
		ft_token_add_back(syntax_tokens, ft_token_pop(tokens));
}

static void	ft_set_syntax_args(t_syntax *syntax, t_token **tokens)
{
	int		operno;

	if (syntax == 0 || tokens == 0)
		return ;
	while (*tokens && ft_get_operno(*tokens) != TYPE_OPER_PIPE)
	{
		operno = ft_get_operno(*tokens);
		if ((*tokens)->type == TYPE_TOKEN_WORD)
		{
			if (syntax->cmd == 0)
				ft_token_add_back(&(syntax->cmd), ft_token_pop(tokens));
			else
				ft_token_add_back(&(syntax->args), ft_token_pop(tokens));
		}
		else if (operno == -1)
			ft_token_add_back(&(syntax->error), ft_token_pop(tokens));
		else if (operno < 2)
			ft_get_content_syntax(&(syntax->input), tokens);
		else
			ft_get_content_syntax(&(syntax->output), tokens);
	}
}

t_syntax	*ft_syntax_new(t_token **tokens)
{
	t_syntax	*res;

	if (tokens == 0)
		return (0);
	res = (t_syntax *)ft_calloc(1, sizeof(t_syntax));
	if (res == 0)
		return (0);
	ft_set_syntax_args(res, tokens);
	return (res);
}
