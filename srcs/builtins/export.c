/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:43:03 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/07 17:14:27 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_append_env(char **env, char *var)
{
	char	**copy;
	int		i;

	i = 0;
	while (env[i])
		++i;
	copy = ft_calloc(i + 2, sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (env[i])
	{
		copy[i] = env[i];
		++i;
	}
	copy[i] = var;
	return (copy);
}

static int	is_valid_ident(const char *v)
{
	int	i;

	i = 0;
	while (v[i] && v[i] != '=')
	{
		if (!ft_isalpha(v[i]) && (i == 0 || !ft_isdigit(v[i])))
			return (0);
		++i;
	}
	if (!i)
		return (0);
	return (1);
}

static int	export_var(char *var)
{
	if (!is_valid_ident(var))
	{
		printf("minishell: export: `%s': not a valid identifier\n", var);
		return (-1);
	}
	return (0);
}

static int	compare_keys(const char *a, const char *b)
{
	int	i;
	int	j;

	i = 0;
	while (a[i] && b[i])
	{
		if (b[i] == '=' || a[i] == '=')
			break ;
		++i;
	}
	return (ft_strncmp(a, b, i));
}

static void	sort_vars(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (compare_keys(env[i], env[j]) > 0)
			{
				tmp = env[j];
				env[j] = env[i];
				env[i] = tmp;
			}
			++j;
		}
		++i;
	}
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
