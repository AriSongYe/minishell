/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 10:16:01 by yecsong           #+#    #+#             */
/*   Updated: 2022/04/19 15:16:06 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*ptr;
	unsigned int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	(ptr = (char *)malloc(sizeof(char) * ft_strlen(s) + 1));
	if (!(ptr))
		return (NULL);
	ptr[ft_strlen(s)] = '\0';
	while (s[i])
	{
		ptr[i] = (*f)(i, s[i]);
		i++;
	}
	return (ptr);
}
