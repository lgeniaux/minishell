/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:43:03 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/07 17:15:39 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_set_env(char *var)
{
    int     i;
    int	    pos;
    char    **copy;

    i = 0;
    while (var[i])
    {
        if (var[i] == '=')
            break ;
        ++i;
    }
    pos = ft_find_env(g_minishell.env, var, i);
    if (pos >= 0)
    {
        if (var[i] == '=')
        {
            free(g_minishell.env[pos]);
            g_minishell.env[pos] = var;
        }
        else
        {
            free(var);
        }
    }
    else
    {
        copy = ft_append_env(g_minishell.env, var);
        if (!copy)
            return (1);
        free(g_minishell.env);
        g_minishell.env = copy;
    }
    return (0);
}
