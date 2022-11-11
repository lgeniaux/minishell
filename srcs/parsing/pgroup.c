/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pgroup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 19:28:33 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 13:12:03 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pgroup_add_cmd(t_piped_command_group *pgroup, t_command *cmd)
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

void	pgroup_destroy(t_piped_command_group *pgroup)
{
	t_command	*cmd;
	t_command	*tmp;

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
	return (0);
}

int	pgroup_parse(t_piped_command_group *pgroup, char *line)
{
	t_token	tok;
	int		n;
	char	*ptr;

	ptr = line;
	n = next_token(&ptr, &tok);
	if (n <= 0)
		return (n);
	n = pgroup_parse_command(pgroup, &line);
	while (n >= 0)
	{
		n = next_token(&line, &tok);
		if (n == 0)
			return (1);
		if (n < 0 || tok.type != TOKEN_PIPE)
			return (-1);
		n = pgroup_parse_command(pgroup, &line);
		if (n < 0)
			break ;
	}
	if (n < 0)
		return (-1);
	return (1);
}
