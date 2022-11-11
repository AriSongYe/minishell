/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:49:32 by yecsong           #+#    #+#             */
/*   Updated: 2022/04/19 17:58:10 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*temp;

	if (!lst || !f)
		return (NULL);
	temp = ft_lstnew(f(lst->content));
	if (!temp)
	{
		ft_lstclear(&temp, del);
		return (NULL);
	}
	temp->next = ft_lstmap(lst->next, f, del);
	return (temp);
}
