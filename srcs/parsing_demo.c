#include "minishell.h"

int resolve_args(t_command *cmd, char **env)
{
	char	*s;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->argv[i])
	{
		s = resolve_string(env, cmd->argv[i]);
		if (!s)
			return (-1);
		if (s[0])
		{
			cmd->argv[j] = s;
			++j;
		}
		else
		{
			free(s);
		}
		++i;
	}
	cmd->argv[j] = NULL;
	return (0);
}

void dump_pgroup(t_piped_command_group *pgroup)
{
	t_command		*cmd;
	t_input_redir	*ir;
	t_output_redir	*or;
	int				i;

	i = 0;
	cmd = pgroup->cmds;
	while (cmd)
	{
		++i;
		cmd = cmd->next;
	}
	printf("# of cmds : %d\n", i);

	i = 0;
	cmd = pgroup->cmds;
	while (cmd)
	{
		printf("#### Command %d ####\n", i + 1);
		for (ir = cmd->in_redirs; ir; ir = ir->next)
			printf("%.*s %s (path: %s)\n", 1 + ir->is_heredoc, "<<", ir->path_or_delim, ir->heredoc_path);
		for (or = cmd->out_redirs; or; or = or->next)
			printf("%.*s %s\n", 1 + or->is_append, ">>", or->path);
		for (int j = 0; cmd->argv[j]; ++j)
			printf("Arg. %d = '%s'\n", j, cmd->argv[j]);
		printf("\n");

		cmd = cmd->next;
	}
}

void sigint_handler()
{
}

int	parse_line(t_piped_command_group **pgrp, char *line)
{
	*pgrp = malloc(sizeof(**pgrp));
	if (!pgrp[0])
		return (-1);
	ft_memset(*pgrp, 0, sizeof(**pgrp));
	if (pgroup_parse(line, *pgrp) != 1)
	{
		pgroup_free(*pgrp);
		return (-1);
	}
	return (1);
}

extern char **environ;

void process_line(char *line)
{
	t_piped_command_group	*grp;

	if (parse_line(&grp, line) == 1)
	{
		if (resolve_args(grp->cmds, environ) < 0)
		{
			
		}
		if (process_heredocs(grp) < 0)
		{
			printf("Can't process heredocs\n");
			heredoc_cleanup(grp);
			pgroup_free(grp);
			return ;
		}
		dump_pgroup(grp);
		heredoc_cleanup(grp);
		pgroup_free(grp);
	}
}

int main(int argc, char *argv[])
{
	char	*line;

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
	return 0;
}
