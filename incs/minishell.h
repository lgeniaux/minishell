/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 22:28:03 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/12 17:24:03 by alavaud          ###   ########lyon.fr   */
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
# include <sys/param.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <termcap.h>
# include <ctype.h>

# define TOKEN_TEXT			1
# define TOKEN_APPEND		2
# define TOKEN_REDIR_OUT	3
# define TOKEN_REDIR_IN		4
# define TOKEN_HEREDOC		5
# define TOKEN_PIPE 		6

/* Bonus */
# define TOKEN_AND	 		7
# define TOKEN_OR	 		8

# define TTY_EXEC			1
# define TTY_INTERACTIVE	2

typedef struct s_token
{
	int		type;
	char	*str;
	int		len;
}	t_token;

typedef struct s_io
{
	int	savedin;
	int	savedout;
}	t_io;

/* = PARSING = */
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
	struct s_pipeline		*pipeline;
	char					**argv;
	char					*path;
	pid_t					pid;
	t_input_redir			*in_redirs;
	t_output_redir			*out_redirs;
	int						builtin_status;
	int						relative;
	struct s_pipeline_cmd	*next;
}	t_pipeline_cmd;

typedef struct s_pipeline
{
	t_piped_command_group	*pgroup;
	t_pipeline_cmd			*cmds;
	char					**env;
}	t_pipeline;

typedef struct s_msh
{
	char		pwd[MAXPATHLEN];
	const char	*arg0;
	char		**env;
	int			last_code;
	char		status_buf[16];
	int			should_exit;
	int			exit_code;
	t_pipeline	*pipeline;
}	t_msh;

extern t_msh	g_minishell;

void		msh_exit(int code);

/* path.c */
char		*find_path(const char *cmd);

/* == Parsing == */

/* = Tokenizer = */

/**
 * @brief Extracts the first token found at *head.
 * Updates *head to point
 * at the end of the parsed token.
 * 
 * @param head A pointer to the current character of the string.
 * @param tok The extracted token data
 * @return int -1 if an error occured (invalid token), 0 if no more tokens
 *  can be extracted, 1 if a token has been successfuly extracted
 */
int			next_token(char **head, t_token *tok);

char		*skip_spaces(char *head);
void		init_token(t_token *tok, int type, char *str, int len);
int			is_valid_text(int ch);

/* == FT Libs */

int			ft_atoi(const char *s);
char		**ft_split(char const *s, char c);
void		*ft_calloc(size_t count, size_t size);
int			ft_isdigit(int c);
void		*ft_memset(void *b, int c, size_t len);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s1);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlcat(char *s1, const char *s2, size_t n);
size_t		ft_strlcpy(char	*dst, const char	*src, size_t dstsize);
size_t		ft_strlen(const char *str);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(const char *s1, const	char *s2);
char		*ft_strcat(char *dest, const char *src);
char		*ft_strndup(const char *s, size_t n);
char		*str_append(char *base, const char *s, int len);
char		**clone_env(char **envp);

/* = ctype = */
int			ft_isspace(int ch);
int			ft_islower(int ch);
int			ft_isupper(int ch);
int			ft_isalpha(int ch);
int			ft_isprint(int ch);

char		*ft_strchr(const char *s, int c);

char		*ft_itoa(int value, char *str);

/* pgroup.c */
void		pgroup_destroy(t_piped_command_group *pgroup);

/**
 * @brief Parses the command starting at *head into pgroup
 * 
 * @param pgroup The pipeline where the parsed command is added.
 * @param head A pointer to the read head.
 * @return int A negative value is returned if an error occured,
 * otherwise 0 is returned.
 */
int			pgroup_parse_command(t_piped_command_group *pgroup, char **head);

int			pgroup_parse(t_piped_command_group *pgroup, char *line);

/* pgroup_resolve.c */
int			pgroup_resolve(t_piped_command_group *pgroup);

/* command.c */

/**
 * @brief 
 * 
 * -1 = invalid command
 * 0 = good command
 * 
 * @param cmd 
 * @param head 
 * @return int 
 */
int			command_parse(t_command *cmd, char **head);

void		command_free(t_command *cmd);

char		**create_clean_env(char **env);

/* redir.c */
/**
 * @brief 
 * 
 * @param cmd 
 * @param type 
 * @param head 
 * @return int Returns a negative value upon error, returns 0 otherwise.
 */
int			parse_redir(t_command *cmd, int type, char **head);

int			token_expect(char **head, t_token *tok, int expected_type);
int			token_unexpect(char **head, t_token *tok, int unexpected_type);

int			prompt(const char *prompt, t_piped_command_group **pgroup);

/* heredoc.c */
char		*heredoc_path(int n);
int			heredoc(t_input_redir *redir);
int			process_heredocs(t_piped_command_group *pgroup);
void		heredoc_cleanup(t_piped_command_group *pgroup);

/* resolve.c */
char		*resolve_vars(char *cmdline, char **env);
void		resolve_args(t_command *cmd, char **env);
char		*append_var(char **resolved, char *cmdline, char **env);
char		*str_append(char *base, const char *s, int len);
int			varlen(const char *line);

/* env.c */
void		env_free(char **env);
int			ft_find_env(char **env, const char *name, int len);
char		*ft_getenv(char **env, const char *name, int len);

/* exec_prepare.c */

/**
 * @brief Creates a new command pipeline from a pgroup
 * 
 * @param pgroup 
 * @return t_pipeline* 
 */
t_pipeline	*pipeline_create(t_piped_command_group *pgroup);

/**
 * @brief Disposes the pipeline and frees everything
 * (including the pipeline itself)
 * 
 * @param pipeline 
 */
void		pipeline_dispose(t_pipeline *pipeline);

/**
 * @brief Prepares the pipeline for execution (path lookup and copying data)
 * 
 * @param pipeline 
 * @return int 
 */
int			pipeline_prepare(t_pipeline *pipeline);
int			run_builtin(t_pipeline_cmd *cmd, int *out);

/* exec_run.c */

/**
 * @brief Executes the pipeline in sub-processes
 * 
 * @param pipeline 
 * @return int 
 */
int			pipeline_exec(t_pipeline *pipeline);

/**
 * @brief Wait for every sub-process to terminate 
 * and returns the exit status of the last in the pipeline.
 * 
 * @param pipeline 
 * @return int 
 */
int			pipeline_wait_status(t_pipeline *pipeline);

/* exec_fork.c */
pid_t		exec_pipeline_cmd(t_pipeline_cmd *cmd,
				int pipe_in, int pipe_out, int next_pipe);

void		pipeline_propagate_signal(t_pipeline *pipeline, int sig);

/* debug.c */
void		dump_pgroup(t_piped_command_group *pgroup);

/* builtin.c */

/* builtins */
int			is_valid_ident(const char *v);
void		print_error(const char *msg, const char *msg2, int errnum);
void		ft_putstr_fd(const char *s, int fd);

int			builtin_export(int argc, char *argv[]);
int			builtin_unset(int argc, char *argv[]);
int			builtin_echo(int argc, char *argv[]);
int			builtin_pwd(int argc, char *argv[]);
int			builtin_cd(int argc, char *argv[]);
int			builtin_env(int argc, char *argv[]);
int			builtin_exit(int argc, char *argv[]);

/* fork.c */
pid_t		fork_redir(t_pipeline_cmd *cmd, int base_in, int base_out);
int			setup_redirs(t_pipeline_cmd *cmd, int base_in, int base_out);
char		**ft_append_env(char **env, char *var);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strjoin(char const *s1, char const *s2);

int			ft_set_env(const char *var);
int			ft_set_env_kv(const char *key, const char *value);

void		signals(void);
void		signals_exec(void);
char		**ft_append_env(char **env, char *var);
void		sort_vars(char **env);
void		rl_replace_line(const char *text, int clear_undo);
int			msh_init(t_msh *msh, int argc, char *argv[], char **envp);
void		msh_exit(int code);
int			msh_update_shlvl(t_msh *msh);
int			msh_check_path(t_msh *msh);
void		set_pwd(char *oldpwd);
void		display_exit(void);

/**
 * @brief Set the tty mode according to `mode'
 * 
 * @param tm The base termios structure
 * @param mode Either (TTY_INTERACTIVE or TTY_EXEC)
 * @return int same as tcsetattr
 */
int			set_tty_mode(int mode);

int			save_io(t_io *io);
int			restore_io(t_io *io);

#endif
