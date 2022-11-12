/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 14:29:37 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/12 13:01:17 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	prepare_heredocs(t_piped_command_group *pgroup)
{
	t_command		*cmd;
	t_input_redir	*in;
	int				n;

	n = 0;
	cmd = pgroup->cmds;
	while (cmd)
	{
		in = cmd->in_redirs;
		while (in)
		{
			if (in->is_heredoc)
			{
				in->heredoc_path = heredoc_path(n++);
				if (!in->heredoc_path)
					return (-1);
			}
			in = in->next;
		}
		cmd = cmd->next;
	}
	return (n);
}

static int	forked_heredoc_process(t_command *cmd)
{
	t_input_redir	*in;

	set_tty_mode(TTY_INTERACTIVE);
	signal(SIGINT, SIG_DFL);
	while (cmd)
	{
		in = cmd->in_redirs;
		while (in)
		{
			if (in->is_heredoc)
			{
				if (heredoc(in) < 0)
					return (1);
			}
			in = in->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

static int	collect_heredocs(t_piped_command_group *pgroup)
{
	pid_t		pid;
	int			status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid)
	{
		if (waitpid(pid, &status, 0) < 0)
		{
			perror("waitpid");
			return (-1);
		}
		if (WIFSIGNALED(status) && WTERMSIG(status) == 2)
			return (0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			return (1);
	}
	else
		exit(forked_heredoc_process(pgroup->cmds));
	return (-1);
}

int	process_heredocs(t_piped_command_group *pgroup)
{
	int	rv;

	if (prepare_heredocs(pgroup) < 0)
		return (-1);
	rv = collect_heredocs(pgroup);
	set_tty_mode(TTY_EXEC);
	return (rv);
}

void	heredoc_cleanup(t_piped_command_group *pgroup)
{
	t_command		*cmd;
	t_input_redir	*in;

	cmd = pgroup->cmds;
	while (cmd)
	{
		in = cmd->in_redirs;
		while (in)
		{
			if (in->heredoc_path)
			{
				unlink(in->heredoc_path);
				free(in->heredoc_path);
			}
			in = in->next;
		}
		cmd = cmd->next;
	}
}
