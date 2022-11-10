/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:26:19 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/10 18:16:31 by lgeniaux         ###   ########.fr       */
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
	export_var(var);
	pos = ft_find_env(g_minishell.env, var, i);
	if (!ft_set_env_utils(var, copy, pos, i))
		return (0);
	return (0);
}

void	set_oldpwd(char *oldpwd)
{
	char	*tmp;

	tmp = ft_strjoin("OLDPWD=", oldpwd);
	ft_set_env(tmp);
}
