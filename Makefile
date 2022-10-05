SRCS = srcs/ft_ctype.c srcs/parsing_demo.c srcs/tokenize.c srcs/tokenize_util.c srcs/pgroup.c srcs/redir.c \
	srcs/ft_strchr.c srcs/command.c
OBJS = $(patsubst srcs/%.c, .objs/%.o, $(SRCS))
NAME = minishell
CFLAGS += -I incs -fsanitize=address -g3

all: .objs $(NAME)

.objs:
	mkdir -pv .objs

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $^ -lreadline -fsanitize=address -g3

.objs/%.o: srcs/%.c incs/minishell.h
	$(CC) -c -o $@ $< $(CFLAGS)
