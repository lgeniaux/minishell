/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 19:12:20 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/07 15:46:55 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	atoi_safe(const char *s, int *value)
{
	int		sign;
	long	n;

	sign = 0;
	n = 0;
	while (ft_isspace(*s))
		++s;
	if (*s == '-' || *s == '+')
		sign = (*s++ == '-');
	if (ft_isdigit(*s))
	{
		while (ft_isdigit(*s))
		{
			n = 10 * n + (*s++ - '0');
			if ((sign && n > 2147483648)
				|| (!sign && n > 2147483647))
			{
				return (-1);
			}
		}
		if (sign)
			*value = -n;
		else
			*value = n;
		return (0);
	}
	return (-1);
}

int	builtin_exit(int argc, char *argv[])
{
	int	code;

	code = 0;
	if (argc > 2)
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else if (argc > 1)
	{
		if (atoi_safe(argv[1], &code) < 0)
		{
			printf("minishell: exit: numeric argument required\n");
			code = -1;
		}
	}
	msh_exit(code);
	return (code);
}
