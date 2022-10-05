/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pgroup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 19:28:33 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/05 16:30:44 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pgroup_add_cmd(t_piped_command_group *pgroup, t_command *cmd)
{
	t_command	*head;

	cmd->next = NULL;
	head = pgroup->cmds;
	if (!head)
	{
		pgroup->cmds = cmd;
	}
	else
	{
		while (head->next)
			head = head->next;
		head->next = cmd;
	}
}

void	pgroup_free(t_piped_command_group *pgroup)
{
	t_command	*cmd;
	t_command	*tmp;
	int	i;

	if (pgroup)
	{
		cmd = pgroup->cmds;
		while (cmd)
		{
			tmp = cmd->next;
			command_free(cmd);
			free(cmd);
			cmd = tmp;
		}
		free(pgroup->raw_line);
	}
	free(pgroup);
}

int	pgroup_parse_command(t_piped_command_group *pgroup, char **head)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (-1);
	if (command_parse(cmd, head) < 0)
	{
		free(cmd);
		return (-1);
	}
	pgroup_add_cmd(pgroup, cmd);
	return (1);
}

int	pgroup_parse(char *line, t_piped_command_group *pgroup)
{
	t_token	tok;
	int		n;

	n = pgroup_parse_command(pgroup, &line);
	if (n <= 0)
		return (-1);
	while (1)
	{
		n = next_token(&line, &tok);
		if (n == 0)
			return (1);
		if (n < 0 || tok.type != TOKEN_PIPE)
			return (-1);
		n = pgroup_parse_command(pgroup, &line);
		if (n < 0)
			break;
	}
	return (1);
}
