/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:42 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/07 14:17:58 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_spaces(char *head)
{
	while (ft_isspace(*head))
		++head;
	return (head);
}

void	init_token(t_token *tok, int type, char *str, int len)
{
	tok->type = type;
	tok->len = len;
	tok->str = str;
}

int	is_valid_text(int ch)
{
	return (!ft_isspace(ch) && !ft_strchr("><|", ch));
}

int	token_expect(char **head, t_token *tok, int expected_type)
{
	int	n;

	n = next_token(head, tok);
	if (n < 0)
		return (-1);
	if (n == 0)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (tok->type != expected_type)
	{
		printf("minishell: syntax error near unexpected token `%.*s'\n",
			tok->len, tok->str);
		return (-1);
	}
	return (0);
}

int	token_unexpect(char **head, t_token *tok, int unexpected_type)
{
	int	n;

	n = next_token(head, tok);
	if (n < 0)
		return (-1);
	if (n == 0)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (tok->type == unexpected_type)
	{
		printf("minishell: syntax error near unexpected token `%.*s'\n",
			tok->len, tok->str);
		return (-1);
	}
	return (0);
}
