/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 15:35:08 by alavaud           #+#    #+#             */
/*   Updated: 2022/09/27 15:43:44 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void init_token(t_token *tok, int type, char *str, int len)
{
	tok->type = type;
	tok->len = len;
	tok->str = str;
}

int	is_valid_text(int ch)
{
	if (ft_isalpha(ch))
		return (1);
	if ((char)ch == '$' || (char)ch == '-' || (char)ch == '?')
		return (1);
	if ((char)ch == '"' || (char)ch == '\'')
		return (1);
	return (0);
}

int	get_text_token(const char *p, int *len)
{
	const char	*start;
	int			strmode;

	start = p;
	strmode = 0;
	while (*p)
	{
		if (*p == '"' && strmode != 2)
			strmode = 1 - strmode;
		else if (*p == '\'' && strmode != 1)
			strmode = 2 - strmode;
		else if (!is_valid_text(*p) && !strmode)
			break ;
		++p;
	}
	if (strmode)
	{
		printf("Unterminated string!\n");
		return (-1);
	}
	if (len)
		*len = (int)(p - start);
	return (1);
}

int	parse_pipe_token(const char **head, t_token *tok)
{
	const char	*p;

	p = *head + 1;
	if (*p == '|')
	{
		if (p[1] == '|')
		{
			printf("Parse error\n");
			return (-1);
		}
		printf("Token: AND\n");
		init_token(tok, TOKEN_AND, (char *)*head, 2);
		*head = p + 1;
		return (1);
	}
	init_token(tok, TOKEN_PIPE, (char *)*head, 1);
	*head = p;
	return (1);
}

int	parse_chevron_token(const char **head, t_token *tok)
{
	const char	*p;

	p = *head + 1;
	if (*p == '>')
	{
		if (p[1] == '>')
		{
			printf("Parse error\n");
			return (-1);
		}
		init_token(tok, TOKEN_APPEND, (char *)*head, 2);
		*head = p + 1;
		return (1);
	}
	init_token(tok, TOKEN_REDIR_OUT, (char *)*head, 1);
	*head = p;
	return (1);
}

int	next_token(const char **head, t_token *tok)
{
	const char	*p;
	char		*buf;
	int			len;

	*head = skip_spaces(*head);
	if (**head == '\0')
		return (0);
	if (is_valid_text(**head))
	{
		if (get_text_token(*head, &len) < 0)
			return (-1);
		init_token(tok, TOKEN_TEXT, (char *)*head, len);
		*head += len;
		return (1);
	}
	else if (**head == '>')
		return (parse_chevron_token(head, tok));
	else if (**head == '|')
		return (parse_pipe_token(head, tok));
	return (0);
}
