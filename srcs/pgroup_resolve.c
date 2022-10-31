/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pgroup_resolve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 15:30:11 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/28 17:37:14 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	resolve_args(t_command *cmd, char **env)
{
	char	*s;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!cmd->argv)
		return (0);
	while (cmd->argv[i])
	{
		s = resolve_vars(cmd->argv[i], env);
		/* TOOD leaks */
		if (s)
		{
			free(cmd->argv[j]);
			cmd->argv[j] = s;
			++j;
		}
		++i;
	}
	i = j;
	while (cmd->argv[i])
		free(cmd->argv[i++]);
	cmd->argv[j] = NULL;
	return (0);
}

static int	is_quoted(const char *str)
{
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			return (1);
		++str;
	}
	return (0);
}

int	pgroup_resolve_cmd(t_command *cmd)
{
	t_input_redir	*in;
	t_output_redir	*out;
	char			*tmp;

	resolve_args(cmd, g_minishell.env);
	in = cmd->in_redirs;
	while (in)
	{
		tmp = resolve_vars(in->path_or_delim, g_minishell.env);
		if (!tmp)
			return (-1);
		in->interpret_vars = !is_quoted(in->path_or_delim);
		free(in->path_or_delim);
		in->path_or_delim = tmp;
		in = in->next;
	}
	out = cmd->out_redirs;
	while (out)
	{
		tmp = resolve_vars(out->path, g_minishell.env);
		if (!tmp)
			return (-1);
		free(out->path);
		out->path = tmp;
		out = out->next;
	}
	return (0);
}

void	pgroup_resolve(t_piped_command_group *pgroup)
{
	t_command	*cmd;

	cmd = pgroup->cmds;
	while (cmd)
	{
		pgroup_resolve_cmd(cmd);
		cmd = cmd->next;
	}
}