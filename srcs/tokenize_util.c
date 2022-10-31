/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:42 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/24 18:23:09 by alavaud          ###   ########lyon.fr   */
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
