/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pgroup_resolve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 15:30:11 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/12 13:22:28 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	dequote_inplace(char *str)
{
	int	strmode;
	int	i;
	int	j;

	strmode = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' && strmode != 2)
			strmode = 1 - strmode;
		else if (str[i] == '"' && strmode != 1)
			strmode = 2 - strmode;
		else
			str[j++] = str[i];
		++i;
	}
	str[j] = '\0';
	return (strmode);
}

static int	resolve_in_redirs(t_command *cmd)
{
	t_input_redir	*in;
	char			*tmp;

	in = cmd->in_redirs;
	while (in)
	{
		if (!in->is_heredoc)
		{
			tmp = resolve_vars(in->path_or_delim, g_minishell.env);
			if (!tmp)
				return (-1);
			free(in->path_or_delim);
			in->path_or_delim = tmp;
		}
		else
			dequote_inplace(in->path_or_delim);
		in->interpret_vars = !is_quoted(in->path_or_delim);
		in = in->next;
	}
	return (0);
}

static int	resolve_out_redirs(t_command *cmd)
{
	t_output_redir	*out;
	char			*tmp;

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

int	pgroup_resolve(t_piped_command_group *pgroup)
{
	t_command	*cmd;

	cmd = pgroup->cmds;
	while (cmd)
	{
		resolve_args(cmd, g_minishell.env);
		if (resolve_in_redirs(cmd) < 0)
			return (-1);
		if (resolve_out_redirs(cmd) < 0)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}
