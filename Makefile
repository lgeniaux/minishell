SRCS = srcs/ft_ctype.c srcs/main.c srcs/tokenize.c srcs/tokenize_util.c srcs/pgroup.c srcs/redir.c \
	srcs/ft_strchr.c srcs/command.c srcs/ft_itoa.c srcs/heredoc.c srcs/resolve.c srcs/env.c srcs/path.c srcs/ft_split.c \
	srcs/exec_prepare.c srcs/debug.c srcs/exec_run.c srcs/pgroup_resolve.c srcs/builtin.c srcs/export.c srcs/unset.c \
	srcs/fork.c
OBJS = $(patsubst srcs/%.c, .objs/%.o, $(SRCS))
NAME = minishell
CFLAGS += -I incs

all: .objs $(NAME)

.objs:
	mkdir -pv .objs

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $^ -lreadline

.objs/%.o: srcs/%.c incs/minishell.h
	$(CC) -c -o $@ $< $(CFLAGS)

fclean:
	rm -rf .objs minishell

re: fclean all

.PHONY: all fclean re
