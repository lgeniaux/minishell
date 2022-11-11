/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:30:29 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 02:28:09 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*concat_path(const char *a, const char *b)
{
	int		len;
	char	*out;

	len = ft_strlen(a) + ft_strlen(b) + 2;
	out = malloc(len);
	if (!out)
		return (NULL);
	ft_strlcpy(out, a, len);
	ft_strlcat(out, "/", len);
	ft_strlcat(out, b, len);
	return (out);
}

static int update_pwd(char *path, char *oldpwd)
{
	char	*pwd;
	int		rv;

	pwd = getcwd(NULL, 0);
	rv = 0;
	if (!pwd)
	{
		rv = 1;
		printf("cd: error retrieving current directory: getcwd: cannot access parent directories: %s\n", strerror(errno));
		pwd = concat_path(oldpwd, path);
		if (!pwd)
			return (1);
	}
	ft_set_env_kv("PWD", pwd);
	ft_set_env_kv("OLDPWD", oldpwd);
	ft_strlcpy(g_minishell.pwd, pwd, MAXPATHLEN);
	free(pwd);
	return (rv);
}

int	builtin_cd(int argc, char *argv[])
{
	char	*path;
	char	*oldpwd;

	if (argc == 2)
	{
		path = argv[1];
	}
	else
	{
		path = ft_getenv(g_minishell.env, "HOME", -1);
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
	}
	oldpwd = g_minishell.pwd;
	if (chdir(path) < 0)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	update_pwd(path, oldpwd);
	return (0);
}
