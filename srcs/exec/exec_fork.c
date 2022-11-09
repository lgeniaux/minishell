/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:29:12 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/09 17:52:12 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_putstr_fd(const char *s, int fd)
{
	size_t	i;

	i = 0;
	while (s[i])
		++i;
	write(fd, s, i);
}

static void print_error(const char *msg, const char *msg2, int errnum)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	if (msg2)
		ft_putstr_fd(msg2, 2);
	else
		ft_putstr_fd(strerror(errnum), 2);
	ft_putstr_fd("\n", 2);
}

static void	exec_cmd(t_pipeline_cmd *cmd)
{
	struct stat	st;
	int	code;

	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	if (run_builtin(cmd, &code) >= 0)
		exit(code);
	if (!cmd->path)
	{
		print_error(cmd->argv[0], "command not found", 0);
		exit(127);
	}
	if (access(cmd->path, X_OK))
	{
		print_error(cmd->path, NULL, errno);
		exit(127);
	}
	if (stat(cmd->path, &st) >= 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			print_error(cmd->path, "is a directory", 0);
			exit(126);
		}
		execve(cmd->path, cmd->argv, g_minishell.env);
	}
	print_error(cmd->path, "cannot execute binary file", 0);
	exit(126);
}

pid_t	exec_pipeline_cmd(t_pipeline_cmd *cmd,
	int base_in, int base_out, int next_pipe)
{
	pid_t	pid;

	pid = fork_redir(cmd, base_in, base_out);
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (next_pipe >= 0)
			close(next_pipe);
		exec_cmd(cmd);
		exit(126);
	}
	return (pid);
}
