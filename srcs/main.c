/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:18:01 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/12 17:08:42 by alavaud          ###   ########lyon.fr   */
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

int	process_line(char *line)
{
	t_piped_command_group	pgroup;
	int						rv;

	ft_memset(&pgroup, 0, sizeof(pgroup));
	rv = -1;
	if (pgroup_parse(&pgroup, line) == 1)
	{
		if (pgroup_resolve(&pgroup) >= 0)
		{
			if (process_heredocs(&pgroup) < 0)
				printf("Could not process heredocs\n");
			else
				rv = exec_pipeline(&pgroup);
		}
	}
	heredoc_cleanup(&pgroup);
	pgroup_destroy(&pgroup);
	return (rv);
}

char	*msh_loop(int tty)
{
	char	*line;

	set_tty_mode(TTY_INTERACTIVE);
	if (tty)
		line = readline("GLaDOS> ");
	else
		line = readline("");
	if (line && *line)
	{
		set_tty_mode(TTY_EXEC);
		add_history(line);
		if (process_line(line) < 0)
			g_minishell.last_code = 1;
	}
	return (line);
}

int	main(int argc, char *argv[], char *envp[])
{
	char			*line;
	int				tty;

	tty = (isatty(0) && isatty(1));
	if (msh_init(&g_minishell, argc, argv, envp) < 0)
		return (1);
	while (!g_minishell.should_exit)
	{
		line = msh_loop(tty);
		if (!line)
			break ;
		free(line);
	}
	if (tty && !g_minishell.should_exit)
		g_minishell.exit_code = g_minishell.last_code;
	if (tty && !line)
		display_exit();
	clear_history();
	env_free(g_minishell.env);
	return (g_minishell.exit_code);
}
