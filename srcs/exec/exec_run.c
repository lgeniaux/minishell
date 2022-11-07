/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 23:05:24 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/07 13:59:31 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_io
{
	int	savedin;
	int	savedout;
}	t_io;

static int save_io(t_io *io)
{
	io->savedin = dup(0);
	if (io->savedin < 0)
		return (-1);
	io->savedout = dup(1);
	if (io->savedout < 0)
	{
		close(io->savedin);
		return (-1);
	}
	return (0);
}

static int restore_io(t_io *io)
{
	int rv;

	rv = 0;
	if (dup2(io->savedin, 0) < 0 || dup2(io->savedout, 1) < 0)
		rv = -1;
	if (rv < 0)
		perror("could not restore io");
	close(io->savedout);
	close(io->savedin);
	return (rv);
}

static int builtin_id(const char *arg0)
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

static int builtin_dispatch_id(int id, int argc, char *argv[])
{
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
	return (-1);
}

static int builtin_dispatch(int id, int argc, char *argv[], t_pipeline_cmd *cmd)
{
	t_io	io;
	int rv;

	save_io(&io);
	/* TODO error handling */
	setup_redirs(cmd, 0, 1);
	rv = builtin_dispatch_id(id, argc, argv);
	restore_io(&io);
	return (rv);
}

int run_builtin(t_pipeline_cmd *cmd, int out)
{
	int argc;
	int id;
	int code;

	if (cmd->argv && cmd->argv[0])
	{
		id = builtin_id(cmd->argv[0]);
		if (id < 0)
			return (-1);
		argc = 0;
		while (cmd->argv[argc])
			++argc;
		builtin_dispatch(id, argc, cmd->argv, cmd);
		cmd->pid = -1;
		return (0);
	}
	return (-1);
}

int	pipeline_exec(t_pipeline *pipeline)
{
	t_pipeline_cmd *cmd;
	int				p[2];
	int				base_in;
	int				base_out;
	int				last_pipe;

	cmd = pipeline->cmds;
	if (cmd && !cmd->next && run_builtin(cmd, 1) >= 0)
		return (0);
	last_pipe = -1;
	while (cmd)
	{
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
		cmd->pid = exec_pipeline_cmd(cmd, base_in, base_out);
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
		code = 0; /* TODO */
		if (cmd->pid > 0)
		{
			if (waitpid(cmd->pid, &status, 0) >= 0)
			{
				/* TODO */
				if (WIFSIGNALED(status))
					code = 128 + WTERMSIG(status), printf("[%d] SIG %d\n", cmd->pid, WTERMSIG(status));
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
