/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 12:14:13 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/23 04:54:51 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*copy_rev(char *dest, const char *src, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		dest[i] = src[len - i - 1];
		++i;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_itoa(int value, char *str)
{
	char			buf[32];
	unsigned int	u;
	int				i;

	i = 0;
	u = (unsigned int)value;
	if (value < 0)
		u = -u;
	i = 0;
	while (0 == i || u > 0)
	{
		buf[i++] = '0' + (u % 10);
		u /= 10;
	}
	if (value < 0)
		buf[i++] = '-';
	return (copy_rev(str, buf, i));
}
