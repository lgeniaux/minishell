/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 14:26:09 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/11 10:24:01 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ctrl+c handler
void	sigint_handler(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void) sig;
}

void	sigint_handler_redisplay(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	(void) sig;
}

// catching ctrl+c and redirect ctrl+\ to IGN signal
void	signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_exec(void)
{
	signal(SIGINT, sigint_handler_redisplay);
	signal(SIGQUIT, SIG_IGN);
}

int	set_tty_mode(struct termios *tm, int mode)
{
	if (TTY_EXEC == mode)
	{
		tm->c_lflag |= ECHOCTL;
		signals_exec();
	}
	else if (TTY_INTERACTIVE == mode)
	{
		tm->c_lflag &= ~ECHOCTL;
		signals();
	}
	return (tcsetattr(0, TCSAFLUSH, tm));
}
