/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 22:28:03 by alavaud           #+#    #+#             */
/*   Updated: 2022/07/29 14:24:36 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>

extern int printf(char const *, ...);

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

# define TOKEN_TEXT			1
# define TOKEN_APPEND		2
# define TOKEN_REDIR_OUT	3
# define TOKEN_REDIR_IN		4
# define TOKEN_HEREDOC		5
# define TOKEN_PIPE 		6

/* Bonus */
# define TOKEN_AND	 		7
# define TOKEN_OR	 		8

typedef struct s_token
{
	int		type;
	char	*str;
	int		len;
}	t_token;

/* == Parsing == */

/* = Tokenizer = */
int	next_token(const char **head, t_token *tok);
const char	*skip_spaces(const char *head);

/* == FT Libs */

/* = ctype = */
int	ft_isspace(int ch);
int	ft_islower(int ch);
int	ft_isupper(int ch);
int	ft_isalpha(int ch);

/* = PARSING = */
/**
 * This contains the only functions and structures you'll ever need to exec
 * the command line input.
 * 
 * Example command: rev < test -test < test2 -lol | grep tset | rev | cat >> output > output2 >> real_output
 */
struct output_redir
{
	int is_append;
	char *path;
	struct output_redir *next;
};

struct input_redir
{
	int is_heredoc;
	char *path_or_delim;
	struct input_redir *next;
};

struct command
{
	char **argv;
	int argc;
	
	struct output_redir *out_redirs;
	struct input_redir *in_redirs;
};

struct piped_command_group
{
	int ncmds;
	struct command *cmds;
};

int prompt(struct piped_command_group **pgroup);

#endif
