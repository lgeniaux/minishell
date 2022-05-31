/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 15:35:08 by alavaud           #+#    #+#             */
/*   Updated: 2022/05/31 16:05:37 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	get_text_token(const char **head, char *buf, int *len)
{
	const char	*p;
	int			strmode;

	p = *head;
	strmode = 0;
	while (*p)
	{
		if (*p == '"' && strmode != 2)
			strmode = 1 - strmode;
		else if (*p == '\'' && strmode != 1)
			strmode = 2 - strmode;
		else if (!is_valid_text(*p) && !strmode)
			break ;
		if (buf)
			*buf++ = *p;
		++p;
	}
	if (strmode)
	{
		printf("Unterminated string!\n");
		return (-1);
	}
	if (len)
		*len = (int)(p - *head);
	return (1);
}

int	parse_pipe_token(const char **head)
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
		*head = p + 1;
		return (1);
	}
	printf("Token: PIPE\n");
	*head = p;
	return (1);
}

int	parse_chevron_token(const char **head)
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
		printf("Token: APPEND\n");
		*head = p + 1;
		return (1);
	}
	printf("Token: REDIR\n");
	*head = p;
	return (1);
}

int	next_token(const char **head)
{
	const char	*p;
	char		*buf;
	int			len;

	*head = skip_spaces(*head);
	if (**head == '\0')
		return (0);
	if (is_valid_text(**head))
	{
		p = *head;
		if (get_text_token(&p, NULL, &len) < 0)
			return (-1);
		buf = malloc(len);
		get_text_token(head, buf, NULL);
		printf("Token: %.*s\n", len, buf);
		free(buf);
		*head = p + len;
		return (1);
	}
	else if (**head == '>')
		return (parse_chevron_token(head));
	else if (**head == '|')
		return (parse_pipe_token(head));
	return (0);
}
