/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_prepare.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 22:47:57 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/08 14:25:41 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_relative(const char *path)
{
	while (*path)
	{
		if (*path == '/')
			return (1);
		++path;
	}
	return (0);
}

static int	prepare_pipeline_command(t_pipeline_cmd *pcmd, t_command *cmd)
{
	pcmd->argv = cmd->argv;
	pcmd->path = NULL;
	pcmd->in_redirs = cmd->in_redirs;
	pcmd->out_redirs = cmd->out_redirs;
	pcmd->relative = 0;
	if (pcmd->argv)
	{
		if (pcmd->argv[0])
		{
			pcmd->relative = is_relative(pcmd->argv[0]);
			if (pcmd->relative)
				pcmd->path = ft_strdup(pcmd->argv[0]);
			else
				pcmd->path = find_path(pcmd->argv[0]);
		}
	}
	return (0);
}

t_pipeline	*pipeline_create(t_piped_command_group *pgroup)
{
	t_pipeline	*pipeline;

	pipeline = ft_calloc(1, sizeof(*pipeline));
	if (pipeline)
	{
		pipeline->pgroup = pgroup;
	}
	return (pipeline);
}

void	pipeline_dispose(t_pipeline *pipeline)
{
	t_pipeline_cmd	*cmd;
	t_pipeline_cmd	*tmp;

	if (pipeline)
	{
		cmd = pipeline->cmds;
		while (cmd)
		{
			free(cmd->path);
			tmp = cmd->next;
			free(cmd);
			cmd = tmp;
		}
	}
	free(pipeline);
}

int	pipeline_prepare(t_pipeline *pipeline)
{
	t_pipeline_cmd				*pcmd;
	t_pipeline_cmd				*last;
	t_command					*cmd;
	const t_piped_command_group	*pgroup = pipeline->pgroup;

	cmd = pgroup->cmds;
	last = NULL;
	while (cmd)
	{
		pcmd = malloc(sizeof(*pcmd));
		if (!pcmd || prepare_pipeline_command(pcmd, cmd) < 0)
		{
			free(pcmd);
			return (-1);
		}
		pcmd->next = NULL;
		if (last)
			last->next = pcmd;
		if (!pipeline->cmds)
			pipeline->cmds = pcmd;
		last = pcmd;
		cmd = cmd->next;
	}
	return (0);
}
