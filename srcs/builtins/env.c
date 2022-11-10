/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 19:06:37 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/10 19:19:40 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(int argc, char *argv[])
{
	int	i;
	int	j;

	(void)argc;
	(void)argv;
	i = 0;
	while (g_minishell.env[i])
	{
		j = 0;
		while (g_minishell.env[i][j] && g_minishell.env[i][j] != '=')
			++j;
		if (g_minishell.env[i][j] == '=' && g_minishell.env[i][j + 1])
			printf("%s\n", g_minishell.env[i]);
		++i;
	}
	return (0);
}
