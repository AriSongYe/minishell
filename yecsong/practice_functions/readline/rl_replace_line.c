/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_replace_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 15:58:08 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/07 18:03:15 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

int	main()
{
	char	*str;

	while (1)
	{
		str = readline("test = ");
		if (!str)
			break ;
		if (!rl_on_new_line())
			printf("rl_on_new_line is active\n");
		rl_replace_line("replace\n", 0);
		rl_redisplay();
//		printf("%s\n", str);
		free(str);
	}
}
