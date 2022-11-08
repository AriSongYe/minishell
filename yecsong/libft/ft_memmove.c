/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 12:25:01 by yecsong           #+#    #+#             */
/*   Updated: 2022/03/18 12:04:11 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*tmp_dst;
	const unsigned char	*tmp_src;

	tmp_dst = (unsigned char *)dst;
	tmp_src = (unsigned char *)src;
	if ((dst == NULL && src == NULL) || len == 0)
		return (dst);
	if (dst > src)
	{
		while (len-- > 0)
			*(tmp_dst + len) = *(tmp_src + len);
	}
	else
	{
		while (len-- > 0)
			*tmp_dst++ = *tmp_src++;
	}
	return (dst);
}
