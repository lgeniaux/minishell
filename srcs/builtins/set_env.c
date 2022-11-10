/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:26:19 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/10 21:51:54 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_var(char *var)
{
	if (!is_valid_ident(var))
	{
		printf("minishell: export: `%s': not a valid identifier\n", var);
		return (-1);
	}
	return (1);
}

void	dump_vars(char **env)
{
	int	i;
	int	sep;

	i = 0;
	sort_vars(env);
	while (env[i])
	{
		sep = 0;
		while (env[i][sep])
		{
			if (env[i][sep] == '=')
				break ;
			++sep;
		}
		printf("declare -x %.*s", sep, env[i]);
		if (env[i][sep])
		{
			printf("=\"%s\"", env[i] + sep + 1);
		}
		printf("\n");
		++i;
	}
}

int	ft_set_env_utils(char *var, char **copy, int pos, int i)
{
	pos = ft_find_env(g_minishell.env, var, i);
	if (pos >= 0)
	{
		if (var[i] == '=')
		{
			free(g_minishell.env[pos]);
			g_minishell.env[pos] = var;
		}
		else
			free(var);
	}
	else
	{
		copy = ft_append_env(g_minishell.env, var);
		if (!copy)
			return (0);
		free(g_minishell.env);
		g_minishell.env = copy;
	}
	return (1);
}

int	ft_set_env(char *var)
{
	int		i;
	int		pos;
	char	**copy;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		++i;
	}
	if (export_var(var) == -1)
		return (-1);
	pos = ft_find_env(g_minishell.env, var, i);
	if (!ft_set_env_utils(var, copy, pos, i))
		return (0);
	return (0);
}

void	set_pwd(char *oldpwd)
{
	char	*tmp_oldpwd;
	char	*tmp_pwd;
	char	*path;

	path = getcwd(NULL, 0);
	tmp_oldpwd = ft_strjoin("OLDPWD=", oldpwd);
	tmp_pwd = ft_strjoin("PWD=", path);
	if (!tmp_oldpwd || !tmp_pwd)
	{
		free(tmp_oldpwd);
		free(tmp_pwd);
		free(path);
		return ;
	}
	ft_set_env(tmp_oldpwd);
	ft_set_env(tmp_pwd);
	free(path);
}

int	ft_set_env_kv(const char *key, const char *value)
{
	size_t	len;
	char	*var;

	len = ft_strlen(key) + 1 + ft_strlen(value) + 1;
	var = malloc(len);
	if (!var)
		return (-1);
	ft_strlcpy(var, key, len);
	ft_strlcat(var, "=", len);
	ft_strlcat(var, value, len);
	if (ft_set_env(var) < 0)
	{
		free(var);
		return (-1);
	}
	return (0);
}
