/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 16:43:56 by yecsong           #+#    #+#             */
/*   Updated: 2022/04/15 11:33:03 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int value, size_t len)
{
	size_t			i;
	unsigned char	*temp;

	temp = (unsigned char *)dest;
	i = 0;
	while (i < len)
	{
		temp[i] = (unsigned char) value;
		i++;
	}
	return (temp);
}
