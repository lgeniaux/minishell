/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 16:59:46 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/18 22:49:05 by alavaud          ###   ########lyon.fr   */
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

int	is_varchar(int ch, int i)
{
	if (ft_isalpha(ch) || ch == '_')
		return (1);
	if (i > 0 && ft_isnumber(ch))
		return (1);
	return (0);
}

char const	*resolve_variable(char **env, char const *str, char **resolved)
{
	int		len;
	char	*var;

	len = 1;
	while (is_varchar(str[len], len - 1))
		++len;
	var = ft_getenv(env, str + 1, len - 1);
	printf("Var '%.*s'\n", len - 1, str + 1);
	printf("Resolved to '%s'\n", var);
	if (var)
		*resolved = str_append(*resolved, var, strlen(var));
	else
		*resolved = str_append(*resolved, str, len);
	return (str + len);
}

char	*resolve_string(char **env, const char *str)
{
	char	*resolved;
	int		strmode;

	resolved = NULL;
	strmode = 0;
	while (*str)
	{
		if (*str == '$' && strmode != 1)
			str = resolve_variable(env, str, &resolved);
		else
		{
			if (*str == '\'' && strmode != 2)
				strmode = 1 - strmode;
			else if (*str == '"' && strmode != 1)
				strmode = 2 - strmode;
			else
				resolved = str_append(resolved, str, 1);
			++str;
		}
		if (!resolved)
			return (NULL);
	}
	if (!resolved)
		resolved = ft_strdup("");
	return (resolved);
}
