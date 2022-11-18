/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:43:25 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/18 17:13:08 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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
			printf("%12s[%zu] -> ", temp->content, ft_strlen(temp->content));
			temp = temp->next;
		}
		printf("(null)\n");
		tokens = tokens->next;
	}
}

void	ft_print_cmds(t_cmd *cmds)
{
	printf("--------------------------------------------\n");
	while (cmds)
	{
		printf("--------------------------------------------\n");
		printf("cmd\n");
		ft_print_tokens(cmds->syntax->cmd);
		printf("\nargs\n");
		ft_print_tokens(cmds->syntax->args);
		printf("\ninput\n");
		ft_print_tokens(cmds->syntax->input);
		printf("\noutput\n");
		ft_print_tokens(cmds->syntax->output);
		printf("\nerror\n");
		ft_print_tokens(cmds->syntax->error);
		cmds = cmds->next;
		printf("--------------------------------------------\n");
	}
	printf("--------------------------------------------\n");
}

int	ft_get_operno(t_token *token)
{
	char	*content;

	if (token->type != 2)
		return (-1);
	content = token->chunks->content;
	if (ft_strlen(content) == 1)
	{
		if (*(content) == '|')
			return (TYPE_OPER_PIPE);
		if (*(content) == '<')
			return (TYPE_OPER_REDIRECT_IN);
		if (*(content) == '>')
			return (TYPE_OPER_REDIRECT_OUT);
	}
	if (ft_strlen(content) == 2)
	{
		if (content[0] == '<' && content[1] == '<')
			return (TYPE_OPER_HEREDOC);
		if (content[0] == '>' && content[1] == '>')
			return (TYPE_OPER_APPEND);
	}
	return (-1);
}

t_token	*ft_token_pop(t_token **tokens)
{
	t_token	*res;

	if (tokens == 0)
		return (0);
	res = *tokens;
	*tokens = res->next;
	res->next = 0;
	return (res);
}

int ft_excute_cmd(t_cmd **cmd)
{
	char	*cmd_str;

	cmd_str = (*cmd)->syntax->cmd->chunks->content;
	if (ft_strncmp(cmd_str, "cd", 3) == 0)
		ft_commamd_cd(cmd);
	else if (ft_strncmp(cmd_str, "echo", 5) == 0)
		ft_command_echo(cmd);
	else if (ft_strncmp(cmd_str, "pwd", 4) == 0)
		ft_command_pwd(cmd);
	else if (ft_strncmp(cmd_str, "export", 7) == 0)
		ft_command_export(cmd);
	else if (ft_strncmp(cmd_str, "unset", 6) == 0)
		ft_command_unset(cmd);
	else if (ft_strncmp(cmd_str, "env", 4) == 0)
		ft_command_env(cmd);
	else if (ft_strncmp(cmd_str, "exit", 5) == 0)
		ft_command_exit(cmd);
	else
		ft_command_others(cmd);
}
