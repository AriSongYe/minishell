/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 14:52:52 by yecsong           #+#    #+#             */
/*   Updated: 2022/04/19 15:26:24 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;
	size_t	start;
	size_t	end;

	if (set == NULL)
		return (ft_strdup(s1));
	if (s1 == NULL)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] != '\0' && ft_strchr(set, s1[start]))
		start++;
	while (end > 0 && ft_strchr(set, s1[end - 1]))
		end--;
	if (start >= end)
		return (ft_strdup(""));
	ptr = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1 + start, end - start + 1);
	return (ptr);
}
