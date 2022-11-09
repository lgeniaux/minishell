/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 16:30:12 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/08 14:25:20 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static char	*search_pathlist(char **pathlist, const char *cmd)
{
	char	*bin;
	int		i;

	bin = NULL;
	i = 0;
	while (pathlist[i])
	{
		bin = (char *)ft_calloc(sizeof(char),
				(ft_strlen(pathlist[i]) + 1 + ft_strlen(cmd) + 1));
		if (!bin)
			break ;
		ft_strcat(bin, pathlist[i]);
		ft_strcat(bin, "/");
		ft_strcat(bin, cmd);
		if (!access(bin, X_OK))
			break ;
		i++;
		free(bin);
		bin = NULL;
	}
	return (bin);
}

char	*find_path(const char *cmd)
{
	char	*path;
	char	**pathlist;
	char	*bin;
	int		i;

	if (!cmd[0])
		return (NULL);
	path = ft_getenv(g_minishell.env, "PATH", -1);
	if (!path)
		return (NULL);
	pathlist = ft_split(path, ':');
	if (!pathlist)
		return (NULL);
	bin = search_pathlist(pathlist, cmd);
	free_array(pathlist);
	return (bin);
}
