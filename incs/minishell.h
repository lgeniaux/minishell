/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 22:28:03 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/30 02:10:11 by alavaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H	

# include <stdlib.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <ctype.h>
# define ft_calloc calloc
# define ft_memset memset
# define ft_strcmp strcmp
# define ft_strncmp strncmp
# define ft_isnumber isnumber
# define ft_isdigit isdigit
# define ft_strndup strndup
# define ft_strlen strlen
# define ft_strlcat strlcat
# define ft_strlcpy strlcpy
# define ft_strdup strdup
# define ft_strcat strcat

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

typedef struct s_msh
{
	char **env;
	int last_code;
} t_msh;

/* = PARSING = */
/**
 * This contains the only functions and structures you'll ever need to exec
 * the command line input.
 * 
 * Example command: rev < test -test < test2 -lol | grep tset | rev | cat >> output > output2 >> real_output
 */

typedef struct s_output_redir
{
	int						is_append;
	char					*path;
	struct s_output_redir	*next;
}	t_output_redir;

typedef struct s_input_redir
{
	int						is_heredoc;
	char					*path_or_delim;
	char					*heredoc_path;
	int						interpret_vars;
	struct s_input_redir	*next;
}	t_input_redir;

typedef struct s_command
{
	char				**argv;
	t_output_redir		*out_redirs;
	t_input_redir		*in_redirs;
	struct s_command	*next;
}	t_command;

typedef struct s_piped_command_group
{
	char		*raw_line;
	t_command	*cmds;
}	t_piped_command_group;

typedef struct s_pipeline_cmd
{
	char					**argv;
	char					*path;
	pid_t					pid;
	t_input_redir			*in_redirs;
	t_output_redir			*out_redirs;
	struct s_pipeline_cmd	*next;
}	t_pipeline_cmd;

typedef struct s_pipeline
{
	t_piped_command_group	*pgroup;
	t_pipeline_cmd			*cmds;
}	t_pipeline;

extern t_msh g_minishell;

/* path.c */
char	*find_path(const char *cmd);

/* == Parsing == */

/* = Tokenizer = */
int		next_token(char **head, t_token *tok);
char	*skip_spaces(char *head);
void	init_token(t_token *tok, int type, char *str, int len);
int	is_valid_text(int ch);

/* == FT Libs */

char	**ft_split(char const *s, char c);

/* = ctype = */
int		ft_isspace(int ch);
int		ft_islower(int ch);
int		ft_isupper(int ch);
int		ft_isalpha(int ch);
int		ft_isprint(int ch);

char	*ft_strchr(const char *s, int c);

char	*ft_itoa(int value, char *str);

/* pgroup.c */
void	pgroup_destroy(t_piped_command_group *pgroup);
int		pgroup_parse_command(t_piped_command_group *pgroup, char **head);
int		pgroup_parse(t_piped_command_group *pgroup, char *line);

/* pgroup_resolve.c */
void	pgroup_resolve(t_piped_command_group *pgroup);

/* command.c */
int		command_parse(t_command *cmd, char **head);
void	command_free(t_command *cmd);

/* redir.c */
int	parse_redir(t_command *cmd, int type, char **head);

int	prompt(const char *prompt, t_piped_command_group **pgroup);

/* heredoc.c */
char	*heredoc_path(int n);
int		heredoc(t_input_redir *redir);
int		process_heredocs(t_piped_command_group *pgroup);
void	heredoc_cleanup(t_piped_command_group *pgroup);

/* resolve.c */
char	*resolve_vars(char *cmdline, char **env);
char	*append_var(char **resolved, char *cmdline, char **env);
char	*str_append(char *base, const char *s, int len);
int		varlen(const char *line);

/* env.c */
void	env_free(char **env);
int		ft_find_env(char **env, const char *name, int len);
char	*ft_getenv(char **env, const char *name, int len);

/* exec_prepare.c */
t_pipeline	*pipeline_create(t_piped_command_group *pgroup);
void		pipeline_dispose(t_pipeline *pipeline);

int	pipeline_prepare(t_pipeline *pipeline);

/* exec_run.c */
int	pipeline_exec(t_pipeline *pipeline);
int	pipeline_wait_status(t_pipeline *pipeline);

/* debug.c */
void	dump_pgroup(t_piped_command_group *pgroup);

/* builtin.c */

/* builtins */
int builtin_export(int argc, char *argv[]);
int	builtin_unset(int argc, char *argv[]);

/* fork.c */
pid_t fork_redir(t_pipeline_cmd *cmd, int base_in, int base_out);
int setup_redirs(t_pipeline_cmd *cmd, int base_in, int base_out);

#endif
