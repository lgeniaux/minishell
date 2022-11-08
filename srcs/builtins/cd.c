/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:30:29 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/08 13:53:24 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	size_t	j;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	i = 0;
	j = -1;
	str = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[++j])
		str[i++] = s2[j];
	str[i] = '\0';
	return (str);
}


int	builtin_cd(int argc, char *argv[])
{
	char	*path;
	char	*oldpwd;
	char	*oldpwd_export;
	
	if (argc == 1)
	{
		path = ft_getenv(g_minishell.env, "HOME", -1);
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
	{
		path = argv[1];
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		printf("minishell: cd: %s\n", strerror(errno));
		return (1);
	}
	if (chdir(path) < 0)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		free(oldpwd);
		return (1);
	}
	oldpwd_export = malloc(ft_strlen(oldpwd) + 8);
	if (!oldpwd_export)
	{
		printf("minishell: cd: %s\n", strerror(errno));
		free(oldpwd);
		return (1);
	}
	free(oldpwd);
	return (0);
}
