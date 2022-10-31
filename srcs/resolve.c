/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 16:59:46 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/30 13:50:53 by alavaud          ###   ########.fr       */
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

int	varlen(const char *line)
{
	int	i;

	i = 1;
	while (line[i])
	{
		if (i > 1 && line[1] == '?')
			break ;
		if (!ft_isalpha(line[i]) && line[i] != '_'
			&& (i > 1 && !ft_isdigit(line[i])))
			break ;
		++i;
	}
	return (i);
}

char	*append_var(char **resolved, char *cmdline, char **env)
{
	int		len;
	char	*var;

	len = varlen(cmdline);
	var = NULL;
	if (len > 1)
		var = ft_getenv(env, cmdline + 1, len - 1);
	if (var)
		*resolved = str_append(*resolved, var, ft_strlen(var));
	return (cmdline + len);
}

char	*resolve_vars(char *cmdline, char **env)
{
	int		strmode;
	char	*resolved;
	int		i;

	strmode = 0;
	resolved = NULL;
	while (*cmdline)
	{
		if (*cmdline == '$' && strmode != 1)
			cmdline = append_var(&resolved, cmdline, env);
		else
		{
			i = 0;
			if (*cmdline == '"' && strmode != 1)
				strmode = 2 - strmode;
			else if (*cmdline == '\'' && strmode != 2)
				strmode = 1 - strmode;
			else
				++i;
			resolved = str_append(resolved, cmdline++, i);
		}
		if (!resolved)
			break ;
	}
	return (resolved);
}
