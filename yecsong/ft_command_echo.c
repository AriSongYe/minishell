/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_echo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:38:11 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/29 21:37:55 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../sanahn/minishell.h"
#include "../sanahn/libft/libft.h"
void    ft_command_echo(t_cmd *cmd)
{
    t_chunk *chunks;
    int     is_n;

    is_n = 0;
    chunks = cmd->syntax->args->chunks;
    if (!ft_strncmp(chunks->content, "-n", 3))
    {
        chunks = chunks->next;
        is_n = 1;
    }
    while (chunks)
    {
        printf("%s", chunks->content);
        chunks = chunks->next;
    }
    if (is_n)
        return ;
    printf("\n");
}

void    ft_command_pwd()
{
    char    *path;

    path = getcwd(NULL, 0);
    // NULL, 0을 넣으면 알아서 사이즈 만큼 동적할당을 해준다, 대신 메모리 해제를 제 때 해줘야 한다.
    if (!path)
        return ;
    printf("%s\n", path);
    free(path);
}

void    ft_command_unset(t_cmd *cmd, char **envp)
{
    char    *env;

    env = getenv("a");
    printf("%s\n", env);
    int i;
    i = 0;
    while (envp[i])
    {
        if (envp[i][0] == 'a')
            envp[i] = "a=bcd";
        printf("%s\n", envp[i]);
        i++;
    }
}

void    ft_command_exit(t_cmd *cmd)
{
    int exitcode;
    
    exitcode = 0;
    printf("exit\n");
    exit(exitcode);
}
