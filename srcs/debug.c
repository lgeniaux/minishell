/**
 * @file debug.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-19
 * 
 * @copyright Copyright (c) 2022
 * 
 * PLEASE DELETE ME (LATER)!
 */
#include "minishell.h"

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
		if (cmd->argv)
		{
			for (int j = 0; cmd->argv[j]; ++j)
				printf("Arg. %d = '%s'\n", j, cmd->argv[j]);
		}
		printf("\n");

		cmd = cmd->next;
	}
}
