/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:30:29 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/02 18:56:58 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(int argc, char *argv[])
{
	char	*path;
	char	*oldpwd;

	if (argc == 1)
	{
		path = ft_getenv(g_minishell.env, "HOME", -1);
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
	{
		path = argv[1];
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		printf("minishell: cd: %s\n", strerror(errno));
		return (1);
	}
	if (chdir(path) < 0)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		free(oldpwd);
		return (1);
	}
	/* TODO set OLDPWD to oldpwd */
	free(oldpwd);
	return (0);
}
