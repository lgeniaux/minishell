/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:59:39 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/11 02:22:13 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (ft_set_env(var));
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
			if (export_var(argv[i]) < 0)
				rv = 1;
			++i;
		}
	}
	else
		dump_vars(g_minishell.env);
	return (rv);
}
