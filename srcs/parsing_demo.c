#include "minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <string.h>

#define ft_memset memset

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
			printf("%.*s %s\n", 1 + ir->is_heredoc, "<<", ir->path_or_delim);
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

void process_line(char *line)
{
	t_piped_command_group	*grp;

	if (parse_line(&grp, line) == 1)
	{
		dump_pgroup(grp);
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
