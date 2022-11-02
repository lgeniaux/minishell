/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 19:51:10 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/30 02:09:51 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_in_redirs(t_input_redir *in)
{
	int	fd;

	while (in)
	{
		if (in->is_heredoc)
			fd = open(in->heredoc_path, O_RDONLY);
		else
			fd = open(in->path_or_delim, O_RDONLY);
		if (fd < 0 || dup2(fd, 0) < 0)
		{
			perror(in->path_or_delim);
			return (-1);
		}
		close(fd);
		in = in->next;
	}
	return (0);
}

static int	setup_out_redirs(t_output_redir *out)
{
	int	fd;
	int	mode;

	while (out)
	{
		mode = O_WRONLY | O_CREAT;
		if (out->is_append)
			mode |= O_APPEND;
		else
			mode |= O_TRUNC;
		fd = open(out->path, mode, 0644);
		if (fd < 0 || dup2(fd, 1) < 0)
		{
			perror(out->path);
			return (-1);
		}
		close(fd);
		out = out->next;
	}
	return (0);
}

int setup_redirs(t_pipeline_cmd *cmd, int base_in, int base_out)
{
	if (dup2(base_in, 0) < 0
		|| dup2(base_out, 1) < 0)
	{
		perror("dup2");
		return (-1);
	}
	if (setup_in_redirs(cmd->in_redirs) < 0
		|| setup_out_redirs(cmd->out_redirs) < 0)
	{
		perror("Redir"); /* TODO */
		return (-1);
	}
	return (0);
}

pid_t fork_redir(t_pipeline_cmd *cmd, int base_in, int base_out)
{
	pid_t pid;

	pid = fork();
	if (0 == pid)
	{
		if (setup_redirs(cmd, base_in, base_out) < 0)
			exit(1);
		if (base_in != 0)
			close(base_in);
		if (base_out != 1)
			close(base_out);
	}
	return (pid);
}
