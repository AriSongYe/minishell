/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:32:47 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/08 21:13:31 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSING_H

#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_token
{
	char			*token;
	struct s_token	*next;
}	t_token;

typedef struct s_info
{
	int		i;
	int		start;
	char	*str;
	t_token	*token;
}	t_info;

#endif
