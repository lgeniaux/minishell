/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:18:01 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/09 19:42:36 by alavaud          ###   ########.fr       */
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
		/* TODO */
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

char	**clone_env(char **envp)
{
	char	**clone;
	int		i;

	i = 0;
	while (envp[i])
		++i;
	clone = ft_calloc(i + 1, sizeof(char *));
	if (clone)
	{
		i = 0;
		while (envp[i])
		{
			clone[i] = ft_strdup(envp[i]);
			if (!clone[i])
			{
				while (i-- > 0)
				{
					free(clone[i]);
				}
				free(clone);
				return (NULL);
			}
			++i;
		}
	}
	return (clone);
}

int	msh_update_shlvl(t_msh *msh)
{
	char	*shlvl;

	shlvl = ft_getenv(msh->env, "SHLVL", -1);
	if (shlvl)
	{
		printf("Shell level is : %s\n", shlvl);
	}
	return (0);
}

int	msh_check_path(t_msh *msh)
{
	return (0);
}

int msh_init(t_msh *msh, char **envp)
{
	msh->env = clone_env(envp);
	msh->last_code = 0;
	msh->should_exit = 0;
	msh->exit_code = 0;
	if (!msh->env)
		return (-1);
	msh_update_shlvl(msh);
	msh_check_path(msh);
	return (0);
}

void	msh_exit(int code)
{
	g_minishell.exit_code = code;
	g_minishell.should_exit = 1;
}

int	main(int argc, char *argv[], char *envp[])
{
	char			*line;
	struct termios	t;

	msh_init(&g_minishell, envp);
	tcgetattr(0, &t);
	while (!g_minishell.should_exit)
	{
		signals();
    	t.c_lflag &= ~ECHOCTL;
		tcsetattr(0, TCSANOW, &t);
		line = readline("GLaDOS> ");
		if (!line)
			break ;
		if (*line)
		{
			signals_exec();
			t.c_lflag |= ECHOCTL;
			tcsetattr(0, TCSANOW, &t);
			add_history(line);
			process_line(line);
		}
		free(line);
	}
	clear_history();
	env_free(g_minishell.env);
	return (g_minishell.exit_code);
}
