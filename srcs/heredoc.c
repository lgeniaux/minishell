/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 17:03:00 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/10 15:06:02 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc_path(int n)
{
	char	buf[32];
	char	nbuf[16];

	ft_strlcpy(buf, "/tmp/heredoc-", 32);
	ft_strlcat(buf, ft_itoa(n, nbuf), 32);
	return (ft_strdup(buf));
}

int	heredoc(const char *path, const char *stopword)
{
	char	*line;
	int		fd;
	int		ret;

	fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
		return (-1);
	ret = 1;
	while (ret)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, stopword))
		{
			free(line);
			break ;
		}
		ret = write(fd, line, ft_strlen(line)) >= 0;
		ret = ret && write(fd, "\n", 1) >= 0;
		free(line);
	}
	close(fd);
	if (!ret)
		return (-1);
	return (1);
}

int	command_process_heredocs(t_command *cmd, int n)
{
	t_input_redir *in;
	
	in = cmd->in_redirs;
	while (in)
	{
		if (in->is_heredoc)
		{
			in->heredoc_path = heredoc_path(n++);
			if (!in->heredoc_path)
				return (-1);
			if (heredoc(in->heredoc_path, in->path_or_delim) < 0)
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
