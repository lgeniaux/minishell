/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 02:44:22 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/12 13:05:28 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_init(t_msh *msh, char **envp)
{
	if (!getcwd(msh->pwd, MAXPATHLEN))
	{
		printf("shell-init: error retrieving current directory: getcwd: "
			"cannot access parent directories: %s\n", strerror(errno));
		return (-1);
	}
	msh->env = clone_env(envp);
	msh->pipeline = NULL;
	msh->last_code = 0;
	msh->should_exit = 0;
	msh->exit_code = 0;
	if (!msh->env)
		return (-1);
	tgetent(NULL, ft_getenv(msh->env, "TERM", -1));
	msh_update_shlvl(msh);
	msh_check_path(msh);
	ft_set_env_kv("PWD", msh->pwd);
	ft_set_env("OLDPWD");
	return (0);
}

static int	msh_get_shlvl(t_msh *msh)
{
	char	*shlvl;
	int		i;

	shlvl = ft_getenv(msh->env, "SHLVL", -1);
	if (shlvl)
	{
		i = 0;
		while (ft_isdigit(shlvl[i]))
			++i;
		if (i > 0 && !shlvl[i])
			return (ft_atoi(shlvl));
	}
	return (0);
}

int	msh_update_shlvl(t_msh *msh)
{
	char	buf[16];
	int		lvl;

	lvl = msh_get_shlvl(msh) + 1;
	return (ft_set_env_kv("SHLVL", ft_itoa(lvl, buf)));
}

int	msh_check_path(t_msh *msh)
{
	if (!ft_getenv(msh->env, "PATH", -1))
		return (ft_set_env_kv("PATH", "/usr/local/bin:/bin:/usr/bin:."));
	return (0);
}
