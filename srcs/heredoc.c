/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 17:03:00 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/05 18:01:24 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void heredoc_path(char buf[32], int n)
{
	char	nbuf[16];

	ft_strlcpy(buf, "/tmp/heredoc-", 32);
	ft_strlcat(buf, ft_itoa(n, nbuf), 32);
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
