/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:33:52 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/02 18:09:55 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_options(int argc, char *argv[], int *nl)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc && argv[i][0] == '-')
	{
		j = 1;
		while (argv[i][j])
		{
			if (argv[i][j] == 'n')
				*nl = 0;
			else
				break ;
			++j;
		}
		if (argv[i][j])
			break ;
		++i;
	}
	return (i);
}

int	builtin_echo(int argc, char *argv[])
{
	int	i;
	int	nl;

	nl = 1;
	i = parse_options(argc, argv, &nl);
	while (i < argc)
	{
		printf("%s", argv[i]);
		if ((i + 1) < argc)
			printf(" ");
		++i;
	}
	printf("%.*s", nl, "\n");
	return (0);
}
