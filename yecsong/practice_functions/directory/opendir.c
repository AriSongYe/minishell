/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opendir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 19:08:35 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/07 19:20:18 by yecsong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

int	main()
{
	DIR				*dir_info;
	struct dirent	*dir_entry;

	mkdir("test_A", 0755);
	mkdir("test_B", 0755);

	dir_info = opendir ("."); // 현재 디렉토리 열기
	if (dir_info)
	{
		while ((dir_entry = readdir(dir_info)))
		{
			printf("%s\n", dir_entry->d_name);
		}
		closedir(dir_info);
	}
}
