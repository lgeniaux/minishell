/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 22:28:03 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/05 17:05:12 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <readline/readline.h>

#define ft_strndup strndup
#define ft_strlen strlen
#define ft_strlcat strlcat
#define ft_strlcpy strlcpy
#define ft_memset memset
#define ft_strcmp strcmp


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
int		next_token(char **head, t_token *tok);
char	*skip_spaces(char *head);

/* == FT Libs */

/* = ctype = */
int	ft_isspace(int ch);
int	ft_islower(int ch);
int	ft_isupper(int ch);
int	ft_isalpha(int ch);
int	ft_isprint(int ch);

char	*ft_strchr(const char *s, int c);

char	*ft_itoa(int value, char *str);

/* = PARSING = */
/**
 * This contains the only functions and structures you'll ever need to exec
 * the command line input.
 * 
 * Example command: rev < test -test < test2 -lol | grep tset | rev | cat >> output > output2 >> real_output
 */

typedef struct s_output_redir
{
	int is_append;
	char *path;
	struct s_output_redir *next;
} t_output_redir;

typedef struct s_input_redir
{
	int is_heredoc;
	char *path_or_delim;
	struct s_input_redir *next;
} t_input_redir;

typedef struct s_command
{
	char **argv;
	t_output_redir *out_redirs;
	t_input_redir *in_redirs;
	struct s_command	*next;
} t_command;

typedef struct s_piped_command_group
{
	char		*raw_line;
	t_command	*cmds;
} t_piped_command_group;

void	pgroup_add_cmd(t_piped_command_group *pgroup, t_command *cmd);
void	pgroup_free(t_piped_command_group *pgroup);
int		pgroup_parse_command(t_piped_command_group *pgroup, char **head);
int		pgroup_parse(char *line, t_piped_command_group *pgroup);

/* command.c */
int		command_parse(t_command *cmd, char **head);
void	command_free(t_command *cmd);

int parse_redir(t_command *cmd, int type, char **head);

int prompt(const char *prompt, t_piped_command_group **pgroup);

/* heredoc.c */
void	heredoc_path(char buf[32], int n);
int		heredoc(const char *path, const char *stopword);

#endif
