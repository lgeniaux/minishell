/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 14:29:37 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/01 14:30:21 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	command_process_heredocs(t_command *cmd, int n)
{
	t_input_redir	*in;

	in = cmd->in_redirs;
	while (in)
	{
		if (in->is_heredoc)
		{
			in->heredoc_path = heredoc_path(n++);
			if (!in->heredoc_path)
				return (-1);
			if (heredoc(in) < 0)
				return (-1);
		}
		in = in->next;
	}
	return (n);
}

int	process_heredocs(t_piped_command_group *pgroup)
{
	t_command	*cmd;
	int			n;
	int			r;

	n = 0;
	cmd = pgroup->cmds;
	while (cmd)
	{
		r = command_process_heredocs(cmd, n);
		if (r < 0)
			return (-1);
		n += r;
		cmd = cmd->next;
	}
	return (n);
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
