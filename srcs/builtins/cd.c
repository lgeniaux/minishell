/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:30:29 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/10 21:47:29 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_error_handler(char *oldpwd, char *path)
{
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
	return (0);
}

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
		path = argv[1];
	oldpwd = getcwd(NULL, 0);
	if (cd_error_handler(oldpwd, path))
		return (1);
	/* TODO */
	set_pwd(oldpwd);
	if (chdir(path) < 0)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		free(oldpwd);
		return (1);
	}
	// TODO
	// ft_set_env_kv("OLDPWD", oldpwd);
	free(oldpwd);
	return (0);
}
