/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 18:47:32 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/10 18:47:55 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_append(char *base, const char *s, int len)
{
	int		size;
	char	*buf;

	size = len + 1;
	if (base)
		size += ft_strlen(base);
	buf = malloc(size);
	if (buf)
	{
		*buf = '\0';
		if (base)
			ft_strlcat(buf, base, size);
		ft_strlcat(buf, s, size);
	}
	free(base);
	return (buf);
}
