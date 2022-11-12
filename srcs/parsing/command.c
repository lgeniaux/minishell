/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:02:24 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/12 15:36:02 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**arglist_append(char **argv, char *arg)
{
	int		len;
	int		i;
	char	**args;

	len = 0;
	if (argv)
	{
		while (argv[len])
			++len;
	}
	args = malloc((len + 2) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (i < len)
	{
		args[i] = argv[i];
		++i;
	}
	args[i++] = arg;
	args[i] = NULL;
	return (args);
}

/**
 * @brief 
 * 
 * @param cmd 
 * @param tok 
 * @param head 
 * @return int Returns a negative value upon error, returns 0 otherwise.
 */
static int	process_token(t_command *cmd, t_token *tok, char **head)
{
	char	**argv;
	char	*s;

	if (tok->type == TOKEN_TEXT)
	{
		s = ft_strndup(tok->str, tok->len);
		if (!s)
			return (-1);
		argv = arglist_append(cmd->argv, s);
		if (!argv)
		{
			free(s);
			return (-1);
		}
		free(cmd->argv);
		cmd->argv = argv;
		return (0);
	}
	else if (tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_OUT
		|| tok->type == TOKEN_APPEND || tok->type == TOKEN_HEREDOC)
	{
		return (parse_redir(cmd, tok->type, head));
	}
	return (-1);
}

static void	command_free_redirs(t_command *cmd)
{
	t_input_redir	*ir;
	t_input_redir	*itmp;
	t_output_redir	*or;
	t_output_redir	*otmp;

	ir = cmd->in_redirs;
	while (ir)
	{
		itmp = ir->next;
		free(ir->path_or_delim);
		free(ir);
		ir = itmp;
	}
	or = cmd->out_redirs;
	while (or)
	{
		otmp = or->next;
		free(or->path);
		free(or);
		or = otmp;
	}
}

void	command_free(t_command *cmd)
{
	int				i;

	i = 0;
	command_free_redirs(cmd);
	if (cmd->argv)
	{
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv);
	}
	free(cmd);
}

int	command_parse(t_command *cmd, char **head)
{
	t_token		tok;
	int			n;
	char		*ptr;

	ptr = *head;
	ft_memset(cmd, 0, sizeof(*cmd));
	if (token_unexpect(&ptr, &tok, TOKEN_PIPE) < 0)
		return (-1);
	ptr = *head;
	n = 0;
	while (n >= 0)
	{
		n = next_token(&ptr, &tok);
		if (n <= 0 || tok.type == TOKEN_PIPE)
			break ;
		n = process_token(cmd, &tok, &ptr);
		if (n < 0)
			break ;
		*head = ptr;
	}
	if (n < 0)
		return (-1);
	return (0);
}
