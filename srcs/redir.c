/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 23:36:02 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/06 18:00:09 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* TODO replace this */
char *strndup(const char *, size_t);

#define ft_strndup strndup

void	*append_redir_out(t_command *cmd, int toktype, const char *pos, int len)
{
	t_output_redir *r;
	t_output_redir *tmp;

	r = malloc(sizeof(*r));
	if (!r)
		return (NULL);
	r->is_append = (toktype == TOKEN_APPEND);
	r->next = NULL;
	r->path = ft_strndup(pos, len);
	if (!cmd->out_redirs)
	{
		cmd->out_redirs = r;
	}
	else
	{
		tmp = cmd->out_redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = r;	
	}
	return (r);
}

void	*append_redir_in(t_command *cmd, int toktype, const char *pos, int len)
{
	t_input_redir *r;
	t_input_redir *tmp;

	r = malloc(sizeof(*r));
	if (!r)
		return (NULL);
	r->is_heredoc = (toktype == TOKEN_HEREDOC);
	r->next = NULL;
	r->path_or_delim = ft_strndup(pos, len);
	r->heredoc_path = NULL;
	if (!cmd->in_redirs)
	{
		cmd->in_redirs = r;
	}
	else
	{
		tmp = cmd->in_redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = r;
	}
	return (r);
}

void	*command_append_redir(t_command *cmd, int toktype, const char *pos, int len)
{
	if (toktype == TOKEN_APPEND || toktype == TOKEN_REDIR_OUT)
		return (append_redir_out(cmd, toktype, pos, len));
	else if (toktype == TOKEN_HEREDOC || toktype == TOKEN_REDIR_IN)
		return (append_redir_in(cmd, toktype, pos, len));
	return (NULL);
}

int parse_redir(t_command *cmd, int type, char **head)
{
    t_token	tok;
    char	*ptr;
    int		n;

    ptr = *head;
    n = next_token(&ptr, &tok);
    if (n <= 0)
        return (n);
    if (tok.type != TOKEN_TEXT)
    {
        printf("Parse error\n");
        return (-1);
    }
	if (!command_append_redir(cmd, type, tok.str, tok.len))
		return (-1);
    *head = ptr;
    return (1);
}
