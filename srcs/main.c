/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:18:01 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 17:41:49 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_msh	g_minishell;

void	pipeline_propagate_signal(t_pipeline *pipeline, int sig)
{
	t_pipeline_cmd	*cmd;

	cmd = pipeline->cmds;
	while (cmd)
	{
		if (cmd->pid > 0)
		{
			kill(cmd->pid, sig);
		}
		cmd = cmd->next;
	}
}

int	exec_pipeline(t_piped_command_group *pgroup)
{
	t_pipeline	*pipeline;
	int			code;
	int			ret;

	pipeline = pipeline_create(pgroup);
	if (!pipeline)
		return (-1);
	ret = -1;
	if (pipeline_prepare(pipeline) >= 0)
	{
		g_minishell.pipeline = pipeline;
		if (pipeline_exec(pipeline) >= 0)
		{
			code = pipeline_wait_status(pipeline);
			g_minishell.last_code = code;
			ret = 0;
		}
	}
	g_minishell.pipeline = NULL;
	pipeline_dispose(pipeline);
	return (ret);
}

void	process_line(char *line)
{
	t_piped_command_group	pgroup;
	int						n;

	ft_memset(&pgroup, 0, sizeof(pgroup));
	n = pgroup_parse(&pgroup, line);
	if (n == 1)
	{
		pgroup_resolve(&pgroup);
		if (process_heredocs(&pgroup) < 0)
		{
			printf("Can't process heredocs\n");
			heredoc_cleanup(&pgroup);
			pgroup_destroy(&pgroup);
			return ;
		}
		exec_pipeline(&pgroup);
		heredoc_cleanup(&pgroup);
	}
	pgroup_destroy(&pgroup);
}

char	*msh_loop(struct termios *tm)
{
	char	*line;

	set_tty_mode(tm, TTY_INTERACTIVE);
	line = readline("GLaDOS> ");
	if (line && *line)
	{
		set_tty_mode(tm, TTY_EXEC);
		add_history(line);
		process_line(line);
	}
	return (line);
}

int	main(int argc, char *argv[], char *envp[])
{
	char			*line;
	struct termios	tm;

	(void)argc;
	(void)argv;
	if (msh_init(&g_minishell, envp) < 0)
		return (1);
	tcgetattr(0, &tm);
	while (!g_minishell.should_exit)
	{
		line = msh_loop(&tm);
		if (!line)
			break ;
		free(line);
	}
	if (!line)
		printf("%.*s\rGLaDOS> exit\n", \
		!!ft_getenv(g_minishell.env, "TERM", -1) * 4, "\033[A1");
	else
		printf("exit\n");
	clear_history();
	env_free(g_minishell.env);
	return (g_minishell.exit_code);
}
