/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:43:42 by sanahn            #+#    #+#             */
/*   Updated: 2022/11/30 00:11:00 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

void	ft_check_leaks(void)
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_token	*tokens;
	t_cmd	*cmds;

	(void)argc;
	(void)argv;
	//(void)envp;
	tokens = 0;
	cmds = 0;
	ft_tokens_init(&tokens);
	while (42)
	{
		str = readline("minishell : ");
		if (str[0] == '\0')
		{
			free(str);
			continue;
		}
		add_history(str);
		// 토크나이즈
		// printf("-------------------------------------------\n");
		ft_tokenize(&tokens, str);
		// ft_print_tokens(tokens);
		// printf("--------------------------------------------\n");
		// 후처리
		// printf("--------------------------------------------\n");
		// 따옴표 처리, 환경 변수 가져오기, 청크 합치기
		ft_chunk_iteri(tokens, ft_dollar_handler);
		ft_chunk_iteri(tokens, ft_qutoes_handler);
		ft_chunk_iteri(tokens, ft_chunk_join);
		ft_del_empty_token(&tokens);
		// printf("--------------------------------------------\n");
		// 파이프를 기준으로 명령줄 나누기
		ft_set_cmd(&cmds, &tokens);
		ft_print_cmds(cmds);
		execute_cmd(&cmds, envp);
		free(str);
		ft_cmds_init(&cmds);
		// system("leaks minishell");
	}
	return (0);
}
