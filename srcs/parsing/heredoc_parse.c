/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 17:03:00 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 13:11:43 by lgeniaux         ###   ########.fr       */
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

static char	*interpret_vars(char *line, char **env)
{
	char	*resolved;

	resolved = ft_strdup("");
	while (*line && resolved)
	{
		if (*line == '$')
			line = append_var(&resolved, line, env);
		else
			resolved = str_append(resolved, line++, 1);
	}
	return (resolved);
}

static int	heredoc_process_line(int fd, char *line, t_input_redir *redir)
{
	char	*tmp;

	if (!line || !ft_strcmp(line, redir->path_or_delim))
		return (0);
	tmp = NULL;
	if (redir->interpret_vars)
	{
		tmp = interpret_vars(line, g_minishell.env);
		if (!tmp)
		{
			close(fd);
			return (-1);
		}
		line = tmp;
	}
	if (write(fd, line, ft_strlen(line)) < 0)
	{
		free(tmp);
		return (-1);
	}
	free(tmp);
	if (write(fd, "\n", 1) < 0)
		return (-1);
	return (1);
}

static int	heredoc_loop(int fd, t_input_redir *redir)
{
	char	*line;
	int		ret;

	ret = 1;
	while (1 == ret)
	{
		line = readline("> ");
		ret = heredoc_process_line(fd, line, redir);
		free(line);
	}
	if (ret < 0)
		return (-1);
	return (0);
}

int	heredoc(t_input_redir *redir)
{
	int		fd;
	int		ret;

	fd = open(redir->heredoc_path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
		return (-1);
	ret = heredoc_loop(fd, redir);
	close(fd);
	if (ret < 0)
		return (-1);
	return (1);
}
