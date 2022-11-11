/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:57:41 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/11 12:43:25 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wifsignaled_status(int *status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	pipeline_wait_status(t_pipeline *pipeline)
{
	t_pipeline_cmd	*cmd;
	int				status;
	int				code;

	cmd = pipeline->cmds;
	status = 0;
	code = 0;
	while (cmd)
	{
		if (cmd->pid < 0)
			code = cmd->builtin_status;
		else
		{
			if (waitpid(cmd->pid, &status, 0) >= 0)
			{
				code = wifsignaled_status(&status);
			}
			else
				perror("waitpid");
		}
		cmd = cmd->next;
	}
	return (code);
}

int	pipeline_exec(t_pipeline *pipeline)
{
	t_pipeline_cmd	*cmd;
	int				p[2];
	int				base_in;
	int				base_out;
	int				last_pipe;

	cmd = pipeline->cmds;
	if (cmd && !cmd->next && run_builtin(cmd, &cmd->builtin_status) >= 0)
		return (0);
	last_pipe = -1;
	while (cmd)
	{
		p[0] = -1;
		if (cmd->next && pipe(p) < 0)
			perror("pipe");
		base_in = 0;
		base_out = 1;
		cmd->pid = exec_pipeline_cmd(cmd, base_in, base_out, p[0]);
		if (last_pipe >= 0)
			close(last_pipe);
		last_pipe = p[0];
		if (cmd->next)
			close(p[1]);
		cmd = cmd->next;
	}
	if (last_pipe >= 0)
		close(last_pipe);
	return (0);
}
