/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 11:55:34 by yecsong           #+#    #+#             */
/*   Updated: 2022/04/19 12:07:20 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	recur_put(int n, int fd);

void	ft_putnbr_fd(int n, int fd)
{
	int	i;

	i = 0;
	if (n == 0)
		ft_putchar_fd('0', fd);
	else if (n < 0)
		ft_putchar_fd('-', fd);
	recur_put(n, fd);
}

static void	recur_put(int n, int fd)
{
	int	temp;

	if (n != 0)
	{
		temp = n % 10;
		recur_put(n / 10, fd);
		if (temp >= 0)
			ft_putchar_fd('0' + temp, fd);
		else if (temp < 0)
			ft_putchar_fd((temp * -1) + '0', fd);
	}
}
