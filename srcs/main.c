#include "minishell.h"

t_msh	g_minishell;

void sigint_handler()
{
}

int exec_pipeline(t_piped_command_group *pgroup)
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

	ft_memset(&pgroup, 0, sizeof(pgroup));
	if (pgroup_parse(&pgroup, line) == 1)
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

int msh_update_shlvl(t_msh *msh)
{
	return (0);
}

int msh_check_path(t_msh *msh)
{
	return (0);
}

int msh_init(t_msh *msh, char **envp)
{
	msh->env = clone_env(envp);
	msh->last_code = 0;
	if (!msh->env)
		return (-1);
	msh_update_shlvl(msh);
	msh_check_path(msh);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;

	msh_init(&g_minishell, envp);
	signal(SIGINT, sigint_handler);
	while ((line = readline("GLaDOS> ")) != NULL)
	{
		if (*line)
		{
			add_history(line);
			process_line(line);
		}
		free(line);
	}
	clear_history();
	env_free(g_minishell.env);
	return 0;
}
