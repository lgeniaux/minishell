/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:18:01 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 20:04:04 by alavaud          ###   ########lyon.fr   */
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
		n = process_heredocs(&pgroup);
		if (n < 0)
		{
			printf("Can't process heredocs\n");
			heredoc_cleanup(&pgroup);
			pgroup_destroy(&pgroup);
			return ;
		}
		if (n == 1)
			exec_pipeline(&pgroup);
		else
			g_minishell.last_code = 1;
		heredoc_cleanup(&pgroup);
	}
	pgroup_destroy(&pgroup);
}

char	*msh_loop(void)
{
	char	*line;

	set_tty_mode(TTY_INTERACTIVE);
	line = readline("GLaDOS> ");
	if (line && *line)
	{
		set_tty_mode(TTY_EXEC);
		add_history(line);
		process_line(line);
	}
	return (line);
}

int	main(int argc, char *argv[], char *envp[])
{
	char			*line;

	(void)argc;
	(void)argv;
	if (msh_init(&g_minishell, envp) < 0)
		return (1);
	while (!g_minishell.should_exit)
	{
		line = msh_loop();
		if (!line)
			break ;
		free(line);
	}
	if (isatty(0))
	{
		if (!line)
			printf("%.*s\rGLaDOS> exit\n", \
			!!ft_getenv(g_minishell.env, "TERM", -1) * 4, "\033[A1");
		else
			printf("exit\n");
	}
	clear_history();
	env_free(g_minishell.env);
	return (g_minishell.exit_code);
}
