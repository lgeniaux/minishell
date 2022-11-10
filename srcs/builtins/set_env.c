/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:26:19 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/10 14:38:28 by lgeniaux         ###   ########.fr       */
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

static void	dump_vars(char **env)
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
	if (pos >= 0)
	{
		if (var[i] == '=')
		{
			free(g_minishell.env[pos]);
			g_minishell.env[pos] = var;
		}
		else
		{
			free(var);
		}
	}
	else
	{
		copy = ft_append_env(g_minishell.env, var);
		if (!copy)
			return (-1);
		free(g_minishell.env);
		g_minishell.env = copy;
	}
	return (0);
}

void	set_oldpwd(char *oldpwd)
{
	char	*tmp;

	tmp = ft_strjoin("OLDPWD=", oldpwd);
	ft_set_env(tmp);
}

int	builtin_export(int argc, char *argv[])
{
	int		i;
	char	*s;
	int		rv;

	i = 1;
	rv = 0;
	if (argc > 1)
	{
		while (i < argc)
		{
			s = ft_strdup(argv[i]);
			if (!s)
				rv = 1;
			if (s && export_var(s) < 0)
			{
				free(s);
				rv = 1;
			}
			++i;
		}
	}
	else
	{
		dump_vars(g_minishell.env);
	}
	return (rv);
}
