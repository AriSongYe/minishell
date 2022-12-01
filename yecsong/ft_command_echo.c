/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_echo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 11:38:11 by yecsong           #+#    #+#             */
/*   Updated: 2022/12/01 20:26:43 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../sanahn/minishell.h"
#include "../sanahn/libft/libft.h"
void    ft_command_echo(char **cmd)

{
    int     is_n;
	int		i;
	
	i = 0;
	is_n = 1;
	if (cmd[1][i] != ('-'))
		is_n = 0;
	i++;
	while (cmd[1][i])
		if (cmd[1][i++] != 'n')
			is_n = 0;
	i = 1;
	if (is_n)
		i++;
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (is_n)
		return ;
	else
		printf("\n");
}

void    ft_command_pwd()
{
    char    *path;

    path = getcwd(NULL, 0);
    if (!path)
        return ;
    printf("%s\n", path);
    free(path);
}

void    ft_command_unset()
{
}
long long ft_atoll(const char *str)
{
	long long	sign;
	size_t		i;
	size_t		num;

	num = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (sign * num);
}

void    ft_command_exit(char **cmd)
{
    long long exitcode;
    int	i;

	i = 0;
    printf("exit\n");
	if (cmd[1] && cmd[2])
	{
		printf("minishell: exit: too many arguments\n");
		exit(1);
	}
	while (cmd[1][i])
	{
		if (cmd[1][i] != '-' && cmd[1][i] != '+' && (cmd[1][i] < '0' || cmd[1][i] > '9'))
		{
			printf("minishell: exit: %s: numeric argument required", cmd[1]);
			exit(255);
		}
		i++;
	}
	exitcode = ft_atoll(cmd[1]) % 256;
    exit(exitcode);
}

int	main()
{
	char	*temp[7];
	temp[0] = "echo";
	temp[1] = "+254";
	temp[2] = NULL;
	ft_command_pwd();
}
