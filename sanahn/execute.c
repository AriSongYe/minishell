/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:04:08 by yecsong           #+#    #+#             */
/*   Updated: 2022/11/30 00:13:53 by yecsong          ###   ########.fr       */
/*   Updated: 2022/11/29 19:58:00 by sanahn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*read_line(int fd, char *store, int *end);
char	*ft_mystrjoin(char *s1, char *s2);
int		ft_mystrchr(const char *s, int *end);
char	**get_cmd_info(t_cmd **cmd, char **envp);

int	ft_mystrlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

size_t	ft_mystrlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	if (src == NULL)
		return (0);
	src_len = ft_mystrlen(src);
	if (dstsize == 0)
		return (src_len);
	while (src[i] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

char	*ft_mystrdup(const char *s1, int size)
{
	char	*ptr;
	int		i;

	i = 0;
	ptr = (char *)malloc(sizeof(char) * (size + 1));
	if (!ptr)
		return (NULL);
	while (i < size && s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
char	*get_next_line(int fd)
{
	static char	*store;
	char		*line;
	char		*temp;
	int			end;

	end = 0;
	temp = read_line(fd, store, &end);
	if (!temp || end == 0)
	{
		free(temp);
		store = NULL;
		return (NULL);
	}
	line = ft_mystrdup(temp, end);
	store = ft_mystrdup(temp + end, ft_mystrlen (temp + end));
	free(temp);
	return (line);
}

char	*read_line(int fd, char *store, int *end)
{
	char	*buff;
	int		n;

	if (store && ft_mystrchr(store, end))
		return (store);
	if (store)
		*end = ft_mystrlen(store);
	buff = (char *)malloc(sizeof(char) * (10 + 1));
	while (1)
	{
		n = read(fd, buff, 10);
		if (n == 0 || n == -1)
			break ;
		buff[n] = '\0';
		store = ft_mystrjoin(store, buff);
		if (!store || ft_mystrchr(store, end))
			break ;
		*end += n;
	}
	free(buff);
	return (store);
}

char	*ft_mystrjoin(char *s1, char *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*ptr;

	s1_len = 0;
	if (s1)
		s1_len = ft_mystrlen(s1);
	s2_len = ft_mystrlen(s2);
	ptr = (char *)malloc(sizeof(char) *(s1_len + s2_len + 1));
	if (!ptr)
		return (NULL);
	ft_mystrlcpy(ptr, s1, s1_len + 1);
	ft_mystrlcpy(ptr + s1_len, s2, s2_len + 1);
	free(s1);
	return (ptr);
}

int	ft_mystrchr(const char *s, int *end)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
		{
			*end = i + 1;
			return (1);
		}
		i++;
	}
	return (0);
}
char	*read_fd(int fd)
{
	char	*temp;
	char	*temp2;
	char	*store;

	store = "";
	while (1)
	{
		temp = get_next_line(fd);
		if (!temp)
			break ;
		temp2 = ft_strjoin(store, temp);
		if (store[0] != '\0')
			free(store);
		store = temp2;
		free(temp);
	}
	return (store);
}

void	child_exec(t_cmd *cmd, char **envp, int pipe_fd[2])
{
	if (cmd->next)
	{
		close(pipe_fd[READ]);
		dup2(pipe_fd[WRITE], 1);
		close(pipe_fd[WRITE]);
	}
	if (access(cmd->cmd_info[0], F_OK))
		exit(127);
	else if (access(cmd->cmd_info[0], X_OK))
		exit(126);
	execve(cmd->cmd_info[0], cmd->cmd_info, envp);
}

void	parent_exec(t_cmd *cmd, int pipe_fd[2])
{
	int	status;
	
	(void)cmd;
	(void)pipe_fd[2];
	status = 0;
	if (cmd->next)
	{
		close(pipe_fd[WRITE]);
		dup2(pipe_fd[READ], 0);
		close(pipe_fd[READ]);
	}
	/*
	else
	{
		int temp_fd;
		char	*temp;
		temp_fd = open("a.txt", O_RDWR | O_CREAT, 0644);
		temp = read_fd(pipe_fd[0]);
		write(temp_fd, temp, ft_strlen(temp));
	}
	*/
	waitpid(0, &status, 0);
	if (status / 256 == 127)
		printf("minishell: command not found: %s\n", cmd->cmd_info[0]);
	else if (status / 256 == 126)
		write(2, "Permission : Denied\n", 25);

	printf("status = %d\n", status / 256);
}

int	link_pipe(t_cmd *cmd, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	status = 0;
	if (pipe(pipe_fd) == -1)
		return (1);
	pid = fork();
	if (pid == 0)
		child_exec(cmd, envp, pipe_fd);
	else if (pid > 0)
		parent_exec(cmd, pipe_fd);
	else if (pid < 0)
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	return (0);
}


int	execute_cmd(t_cmd **cmd, char **envp)
{
	t_cmd	*cmd_temp;
	int		std_input;

	std_input = dup(0);
	cmd_temp = *cmd;
	while (cmd_temp)
	{
		cmd_temp->cmd_info = get_cmd_info(&cmd_temp, envp);
		link_pipe(cmd_temp, envp);
		cmd_temp = cmd_temp->next;
	}
	dup2(std_input, 0);
	return (0);
}

char	**parsing_path(char **envp)
{
	char	**path;
	char	*temp;

	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5))
			break ;
		(*envp)++;
	}
	temp = ft_strdup(*envp + 5);
	path = ft_split(temp, ':');
	free(temp);
	return (path);
}

char	*join_path(char *cmd, char *path)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin("/", cmd);
	temp2 = temp;
	temp = ft_strjoin(path, temp2);
	free(temp2);
	return (temp);
}

char	*valid_cmd(char *cmd, char **path)
{
	char	*cmd_path;
	char	*temp;
	int		i;

	i = 0;
	if (!access(cmd, X_OK))
		return (cmd);
	while (path[i])
	{
		temp = join_path(cmd, path[i]);
		if (!access (temp, X_OK))
			break ;
		if (path [i++ + 1] != NULL)
			free(temp);
	}
	if (access (temp, X_OK))
		return (cmd);
	cmd_path = temp;
	return (cmd_path);
}

int	args_len(t_cmd **cmd)
{
	t_token	*args;
	int		i;

	i = 0;
	args = (*cmd)->syntax->args;
	while (args)
	{
		args = args->next;
		i++;
	}
	return (i);

}

char	**get_cmd_info(t_cmd **cmd, char **envp)
{
	char	**path;
	char	**cmd_info;
	int		i;
	t_token	*args;

	i = args_len(cmd);
	args = (*cmd)->syntax->args;
	cmd_info = (char **)calloc(i + 2, sizeof(char *));
	i = 1;
	while (args)
	{
		cmd_info[i] = ft_strdup(args->chunks->content);
		args = args->next;
		i++;
	}
	path = parsing_path(envp);
	cmd_info[0] = valid_cmd((*cmd)->syntax->cmd->chunks->content, path);
	free(path);
	return (cmd_info);
}
