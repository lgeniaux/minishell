/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 18:49:24 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/12 16:43:44 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	outc(int ch)
{
	return (write(1, &ch, 1));
}

void	display_exit(void)
{
	char	*str;

	str = tgetstr("UP", NULL);
	tputs(tgoto(str, 1, 0), 0, outc);
	printf("\rGLaDOS> exit\n");
}

void	msh_exit(int code)
{
	g_minishell.exit_code = code;
	g_minishell.should_exit = 1;
}

static void	free_envp(int i, char **clone)
{
	while (i-- > 0)
	{
		free(clone[i]);
	}
	free(clone);
}

char	**clone_env(char **envp)
{
	char	**clone;
	int		i;

	i = 0;
	while (envp[i])
		++i;
	clone = ft_calloc(i + 1, sizeof(char *));
	if (clone)
	{
		i = 0;
		while (envp[i])
		{
			clone[i] = ft_strdup(envp[i]);
			if (!clone[i])
			{
				free_envp(i, clone);
				return (NULL);
			}
			++i;
		}
	}
	return (clone);
}
