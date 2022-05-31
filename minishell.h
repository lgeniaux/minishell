/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 22:28:03 by alavaud           #+#    #+#             */
/*   Updated: 2022/05/31 16:05:26 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>

typedef struct	s_env
{
	char *name;
	char *val;
	struct s_env *next;
}	t_env;

typedef struct	s_shellenv
{
	t_env *envlist;
	int envcount;
} t_shellenv;

# define TOKEN_STRING 1

typedef struct s_token
{
	int	type;
	union
	{
		char	*str;
	};
}	t_token;

/* == Parsing == */

/* = Tokenizer = */
int	next_token(const char **head);
const char	*skip_spaces(const char *head);

/* == FT Libs */

/* = ctype = */
int	ft_isspace(int ch);
int	ft_islower(int ch);
int	ft_isupper(int ch);
int	ft_isalpha(int ch);

#endif
