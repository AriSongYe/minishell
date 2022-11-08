/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 16:49:59 by yecsong           #+#    #+#             */
/*   Updated: 2022/04/19 12:06:52 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	assign_n(char *ptr, int i, int n);

char	*ft_itoa(int n)
{
	char	*ptr;
	int		i;
	int		temp;

	i = 1;
	if (n < 0)
		i++;
	temp = n;
	while (temp != 0)
	{
		temp /= 10;
		i++;
	}
	if (n != 0)
		i -= 1;
	ptr = (char *)malloc(sizeof(char) * (i + 1));
	if (!(ptr))
		return (NULL);
	ptr[i] = '\0';
	if (n < 0)
		ptr[0] = '-';
	if (n == 0)
		ptr[0] = '0';
	assign_n(ptr, i, n);
	return (ptr);
}

static void	assign_n(char *ptr, int i, int n)
{
	while (--i >= 0 && n != 0)
	{
		if (n > 0)
			ptr[i] = (n % 10) + '0';
		else if (n < 0)
			ptr[i] = -1 * (n % 10) + '0';
		n = n / 10;
	}
}
