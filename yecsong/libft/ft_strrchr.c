/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 12:14:39 by yecsong           #+#    #+#             */
/*   Updated: 2022/03/29 17:19:01 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*temp;
	int		i;

	temp = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			temp = (char *)&s[i];
		i++;
	}
	if (temp)
		return (temp);
	if ((char)c == '\0')
		return ((char *)(s + ft_strlen(s)));
	return (NULL);
}
