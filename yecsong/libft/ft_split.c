/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:58:23 by yecsong           #+#    #+#             */
/*   Updated: 2022/04/19 12:08:19 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_section(char const *s, char c);
static void		fill_word(char **ptr, char const *s, char c, size_t i);
static char		**malloc_error(char **ptr);
static char		**count_word(char **ptr, char const *s, char c, size_t i);

char	**ft_split(char const *s, char c)
{
	char	**ptr;

	if (!s)
		return (NULL);
	ptr = (char **)malloc(sizeof(char *) * (count_section(s, c) + 1));
	if (!(ptr))
		return (NULL);
	if (!(count_word(ptr, s, c, 0)))
		return (malloc_error(ptr));
	fill_word(ptr, s, c, 0);
	return (ptr);
}

static char	**malloc_error(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (NULL);
}

static void	fill_word(char **ptr, char const *s, char c, size_t i)
{
	size_t	j;
	size_t	k;

	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			k = 0;
			while (s[i] != c && s[i])
			{
				ptr[j][k] = s[i];
				i++;
				k++;
			}
			ptr[j][k] = '\0';
			j++;
		}
		else
			i++;
	}
	ptr[j] = NULL;
}

static size_t	count_section(char const *s, char c)
{
	size_t	i;
	size_t	cnt;

	cnt = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			cnt++;
			while (s[i] != c && s[i])
				i++;
		}
		else
			i++;
	}
	return (cnt);
}

static char	**count_word(char **ptr, char const *s, char c, size_t i)
{
	size_t	j;
	size_t	word_cnt;

	j = 0;
	while (s[i])
	{
		word_cnt = 0;
		if (s[i] != c)
		{
			while (s[i] != c && s[i])
			{
				word_cnt++;
				i++;
			}
			ptr[j] = (char *)malloc(sizeof(char) * word_cnt + 1);
			if (!(ptr[j]))
				return (NULL);
			j++;
		}
		else
			i++;
	}
	return (ptr);
}
