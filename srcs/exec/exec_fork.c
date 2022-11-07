/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:29:12 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/07 15:33:51 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cmd(t_pipeline_cmd *cmd)
{
	int	i;
	int	code;

	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	if (run_builtin(cmd, &code) >= 0)
		exit(code);
	if (!cmd->path)
	{
		dprintf(2, "command not found: %s\n", cmd->argv[0]);
		exit(127);
	}
	/* TODO temporary fix (pass pipe information so we can close it in the child) */
	i = 3;
	while (i < 1024)
	{
		close(i++);
	}
	/* Tries to close the pipe here */
	/*	if (p >= 0)
			close(p);*/
	if (execve(cmd->path, cmd->argv, g_minishell.env) < 0)
	{
		perror("execve");
	}
}

pid_t	exec_pipeline_cmd(t_pipeline_cmd *cmd,
	int base_in, int base_out)
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
		exec_cmd(cmd);
		exit(126);
	}
	return (pid);
}
