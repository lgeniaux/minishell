/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 14:26:09 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/11 18:58:00 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ctrl+c handler
void	interactive_sig_handler(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void) sig;
}

void	exec_sig_handler(int sig)
{
	if (sig == SIGQUIT && g_minishell.pipeline)
	{
		pipeline_propagate_signal(g_minishell.pipeline, SIGQUIT);
	}
	else if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

// catching ctrl+c and redirect ctrl+\ to IGN signal
void	signals(void)
{
	signal(SIGINT, interactive_sig_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_exec(void)
{
	signal(SIGINT, exec_sig_handler);
	signal(SIGQUIT, exec_sig_handler);
}

int	set_tty_mode(int mode)
{
	struct termios tm;

	if (tcgetattr(0, &tm) < 0)
		return (-1);
	if (TTY_EXEC == mode)
	{
		tm.c_lflag |= ECHOCTL;
		signals_exec();
	}
	else if (TTY_INTERACTIVE == mode)
	{
		tm.c_lflag &= ~ECHOCTL;
		signals();
	}
	return (tcsetattr(0, TCSAFLUSH, &tm));
}
