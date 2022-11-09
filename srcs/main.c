/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:18:01 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/09 22:02:04 by alavaud          ###   ########lyon.fr   */
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

static int ft_atoi(const char *s)
{
	int	n;
	
	n = 0;
	while (ft_isdigit(*s))
		n = n * 10 + (*s++ - '0');
	return (n);
}

int	msh_get_shlvl(t_msh *msh)
{
	char	*shlvl;
	int		i;

	shlvl = ft_getenv(msh->env, "SHLVL", -1);
	if (shlvl)
	{
		i = 0;
		while (ft_isdigit(shlvl[i]))
			++i;
		if (i > 0 && !shlvl[i])
			return (ft_atoi(shlvl));
	}
	return (0);
}

int	msh_update_shlvl(t_msh *msh)
{
	char *buf;
	int	n;
	int lvl;
	int	rv;

	buf = malloc(32);
	if (!buf)
		return (-1);
	lvl = msh_get_shlvl(msh) + 1;
	n = ft_strlcpy(buf, "SHLVL=", 32);
	ft_itoa(lvl, buf + n);
	rv = ft_set_env(buf);
	if (rv < 0)
		free(buf);
	return (rv);
}

int	msh_check_path(t_msh *msh)
{
	char	*path;
	int		rv;

	path = ft_getenv(msh->env, "PATH", -1);
	if (!path)
	{
		path = ft_strdup("PATH=/usr/local/bin:/bin:/usr/bin:.");
		if (!path)
			return (-1);
		rv = ft_set_env(path);
		if (rv < 0)
			free(path);
		return (rv);
	}
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
	ft_set_env_kv("OLDPWD", "");
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
