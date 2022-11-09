/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 15:35:08 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/04 13:56:34 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_text_token(char *p, int *len)
{
	char	*start;
	int		strmode;

	start = p;
	strmode = 0;
	while (*p)
	{
		if (*p == '"' && strmode != 1)
			strmode = 2 - strmode;
		else if (*p == '\'' && strmode != 2)
			strmode = 1 - strmode;
		else if (!is_valid_text(*p) && !strmode)
			break ;
		++p;
	}
	if (strmode)
	{
		printf("minishell: unexpected EOF while looking for matching `%c'\n", "'\""[strmode - 1]);
		return (-1);
	}
	if (len)
		*len = (int)(p - start);
	return (0);
}

static int	parse_pipe_token(char **head, t_token *tok)
{
	char	*p;

	p = *head + 1;
	if (*p == '|')
	{
		if (p[1] == '|')
		{
			printf("Parse error\n");
			return (-1);
		}
		init_token(tok, TOKEN_AND, *head, 2);
		*head = p + 1;
		return (1);
	}
	init_token(tok, TOKEN_PIPE, *head, 1);
	*head = p;
	return (1);
}

static int	parse_chevron_right_token(char **head, t_token *tok)
{
	char	*p;

	p = *head + 1;
	if (*p == '>')
	{
		if (p[1] == '>')
		{
			printf("Parse error\n");
			return (-1);
		}
		init_token(tok, TOKEN_APPEND, *head, 2);
		*head = p + 1;
		return (1);
	}
	init_token(tok, TOKEN_REDIR_OUT, *head, 1);
	*head = p;
	return (1);
}

static int	parse_chevron_left_token(char **head, t_token *tok)
{
	char	*p;

	p = *head + 1;
	if (*p == '<')
	{
		if (p[1] == '<')
		{
			printf("Parse error\n");
			return (-1);
		}
		init_token(tok, TOKEN_HEREDOC, *head, 2);
		*head = p + 1;
		return (1);
	}
	init_token(tok, TOKEN_REDIR_IN, *head, 1);
	*head = p;
	return (1);
}

int	next_token(char **head, t_token *tok)
{
	char	*p;
	char	*buf;
	int		len;

	*head = skip_spaces(*head);
	if (**head == '\0')
		return (0);
	if (is_valid_text(**head))
	{
		if (get_text_token(*head, &len) < 0)
		{
			printf("minishell: syntax error: unexpected end of file\n");
			return (-1);
		}
		init_token(tok, TOKEN_TEXT, *head, len);
		*head += len;
		return (1);
	}
	else if (**head == '>')
		return (parse_chevron_right_token(head, tok));
	else if (**head == '<')
		return (parse_chevron_left_token(head, tok));
	else if (**head == '|')
		return (parse_pipe_token(head, tok));
	return (-1);
}
