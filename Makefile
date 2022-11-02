SRCS = srcs/builtins/export.c srcs/builtins/unset.c srcs/exec/env.c srcs/exec/exec_prepare.c srcs/exec/exec_run.c srcs/exec/fork.c \
	srcs/exec/path.c srcs/ft/ft_ctype.c srcs/ft/ft_itoa.c srcs/ft/ft_split.c srcs/ft/ft_strchr.c srcs/parsing/command.c srcs/parsing/heredoc.c \
	srcs/parsing/pgroup.c srcs/parsing/pgroup_resolve.c srcs/parsing/redir.c srcs/parsing/resolve.c srcs/parsing/tokenize.c srcs/parsing/parsing_utils.c \
	srcs/main.c srcs/debug.c srcs/parsing/heredoc_parse.c
OBJS = $(patsubst %.c, %.o, $(SRCS))
NAME = minishell
CFLAGS += -I incs

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $^ -lreadline

%.o: %.c incs/minishell.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS) minishell

re: fclean all

.PHONY: all fclean re
