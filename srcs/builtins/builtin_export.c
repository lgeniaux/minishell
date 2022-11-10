/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:59:39 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/10 20:01:21 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			//if (s && export_var(s) < 0)
			if (s && ft_set_env(s) < 0)
			{
				free(s);
				rv = 1;
			}
			++i;
		}
	}
	else
		dump_vars(g_minishell.env);
	return (rv);
}
