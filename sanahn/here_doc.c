/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 20:02:59 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/29 20:59:04 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parsing_heredoc(char *limiter)
{
	char	*store;
	char	*temp;
	char	*temp2;

	store = "";
	while (1)
	{
		temp = get_next_line(0);
		if (!ft_strncmp(limiter, temp, ft_strlen(limiter) + 1))
			break ;
		temp2 = ft_strjoin(store, temp);
		if (store[0] != '\0')
			free(store);
		store = temp2;
		free(temp);
	}
	free(temp);
	return (store);
}

void	create_temp_file(char *limiter)
{
	int		temp_fd;
	char	*temp;

	temp_fd = open("/tmp/here_doc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	temp = parsing_heredoc(limiter);
	write(temp_fd, temp, ft_strlen(temp));
	if (temp[0] != '\0')
		free(temp);
	close(temp_fd);
}
