/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 23:05:24 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 03:05:33 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_id(const char *arg0)
{
	if (!ft_strcmp(arg0, "echo"))
		return (0);
	else if (!ft_strcmp(arg0, "export"))
		return (1);
	else if (!ft_strcmp(arg0, "unset"))
		return (2);
	else if (!ft_strcmp(arg0, "cd"))
		return (3);
	else if (!ft_strcmp(arg0, "pwd"))
		return (4);
	else if (!ft_strcmp(arg0, "env"))
		return (5);
	else if (!ft_strcmp(arg0, "exit"))
		return (6);
	return (-1);
}

static int	builtin_dispatch_id(int id, int argc, char *argv[], int *found)
{
	*found = 1;
	if (0 == id)
		return (builtin_echo(argc, argv));
	else if (1 == id)
		return (builtin_export(argc, argv));
	else if (2 == id)
		return (builtin_unset(argc, argv));
	else if (3 == id)
		return (builtin_cd(argc, argv));
	else if (4 == id)
		return (builtin_pwd(argc, argv));
	else if (5 == id)
		return (builtin_env(argc, argv));
	else if (6 == id)
		return (builtin_exit(argc, argv));
	*found = 0;
	return (-1);
}

static int	builtin_dispatch(int id, t_pipeline_cmd *cmd, int *out)
{
	t_io	io;
	int		rv;
	int		found;
	int		argc;

	argc = 0;
	while (cmd->argv[argc])
		++argc;
	save_io(&io);
	setup_redirs(cmd, 0, 1);
	*out = builtin_dispatch_id(id, argc, cmd->argv, &found);
	if (!found)
		rv = -1;
	restore_io(&io);
	return (rv);
}

int	run_builtin(t_pipeline_cmd *cmd, int *out)
{
	int	id;

	if (cmd->argv && cmd->argv[0])
	{
		id = builtin_id(cmd->argv[0]);
		if (id < 0)
			return (-1);
		cmd->pid = -1;
		if (builtin_dispatch(id, cmd, out) < 0)
			return (-1);
		return (0);
	}
	return (-1);
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
		{
			perror("pipe");
		}
		base_in = 0;
		base_out = 1;
		if (last_pipe >= 0)
			base_in = last_pipe;
		if (cmd->next)
			base_out = p[1];
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
				if (WIFSIGNALED(status))
				{
					code = 128 + WTERMSIG(status);
				}
				else if (WIFEXITED(status))
					code = WEXITSTATUS(status);
			}
			else
				perror("waitpid");
		}
		cmd = cmd->next;
	}
	return (code);
}
