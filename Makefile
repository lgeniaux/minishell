SRCS = srcs/ft_ctype.c srcs/parsing_demo.c srcs/tokenize.c srcs/tokenize_util.c srcs/pgroup.c srcs/redir.c \
	srcs/ft_strchr.c srcs/command.c srcs/ft_itoa.c srcs/heredoc.c
OBJS = $(patsubst srcs/%.c, .objs/%.o, $(SRCS))
NAME = minishell
CFLAGS += -I incs -g3 -fsanitize=address

all: .objs $(NAME)

.objs:
	mkdir -pv .objs

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $^ -lreadline -g3 -fsanitize=address

.objs/%.o: srcs/%.c incs/minishell.h
	$(CC) -c -o $@ $< $(CFLAGS)

fclean:
	rm -rf .objs minishell

re: fclean all

.PHONY: all fclean re