/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:18:01 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 10:27:54 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "termios.h"

t_msh	g_minishell;

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
		if (pipeline_exec(pipeline) >= 0)
		{
			code = pipeline_wait_status(pipeline);
			g_minishell.last_code = code;
			ret = 0;
		}
	}
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

int	main(int argc, char *argv[], char *envp[])
{
	char			*line;
	struct termios	tm;

	if (msh_init(&g_minishell, envp) < 0)
		return (1);
	tcgetattr(0, &tm);
	while (!g_minishell.should_exit)
	{
		set_tty_mode(&tm, TTY_INTERACTIVE);
		line = readline("GLaDOS> ");
		if (!line)
			break ;
		if (*line)
		{
			set_tty_mode(&tm, TTY_EXEC);
			add_history(line);
			process_line(line);
		}
		free(line);
	}
	clear_history();
	env_free(g_minishell.env);
	return (g_minishell.exit_code);
}
