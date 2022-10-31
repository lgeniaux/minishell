/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:02:43 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/30 02:05:21 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * These builins are spawned in their own processes.
 */

int builtin_echo(int argc, char *argv[], char *envp[])
{
	int nl;
	int i;

	(void)envp;
	nl = (argc > 1 && !strcmp(argv[1], "-n"));
	i = 1 + nl;
	while (i < argc)
	{
		printf("%s", argv[i]);
		i++;
		if (i != argc)
			printf(" ");
	}
	if (!nl)
		printf("\n");
	return 0;
}

static char	*ft_getenv2(char **env, const char *name)
{
	int	i;
	int j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && name[j] && env[i][j] != '=')
			++j;
		if (!name[j] && (env[i][j] == '=' || !env[i][j]))
			return (env[i] + j + (env[i][j] == '='));
		++i;
	}
	return (NULL);
}

int builtin_cd(int argc, char *argv[], char *envp[])
{
	char	*path;

	if (argc == 1)
	{
		path = ft_getenv2(envp, "HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return 1;
		}
	}
	else
	{
		path = argv[1];
	}
	if (chdir(path) < 0)
	{
		perror("minishell: cd");
		return 1;
	}
	return 0;
}

