NAME        := minishell
CC        := gcc
FLAGS    := -Wall -Wextra -Werror -I~/.brew/opt/readline/include -L ~/.brew/opt/readline/lib -lreadline -I incs
READLINE_F  := 

SRCS        :=      srcs/builtins/builtin_export.c \
                          srcs/builtins/cd.c \
                          srcs/builtins/echo.c \
                          srcs/builtins/env.c \
                          srcs/builtins/exit.c \
                          srcs/builtins/export.c \
                          srcs/builtins/pwd.c \
                          srcs/builtins/set_env.c \
                          srcs/builtins/unset.c \
                          srcs/debug.c \
                          srcs/exec/env.c \
                          srcs/exec/exec_fork.c \
                          srcs/exec/exec_functions.c \
                          srcs/exec/exec_prepare.c \
                          srcs/exec/exec_run.c \
                          srcs/exec/fork.c \
                          srcs/exec/path.c \
                          srcs/ft/ft_atoi.c \
                          srcs/ft/ft_calloc.c \
                          srcs/ft/ft_ctype.c \
                          srcs/ft/ft_isdigit.c \
                          srcs/ft/ft_isnumber.c \
                          srcs/ft/ft_itoa.c \
                          srcs/ft/ft_memset.c \
                          srcs/ft/ft_split.c \
                          srcs/ft/ft_str_append.c \
                          srcs/ft/ft_strcat.c \
                          srcs/ft/ft_strchr.c \
                          srcs/ft/ft_strcmp.c \
                          srcs/ft/ft_strdup.c \
                          srcs/ft/ft_strjoin.c \
                          srcs/ft/ft_strlcat.c \
                          srcs/ft/ft_strlcpy.c \
                          srcs/ft/ft_strlen.c \
                          srcs/ft/ft_strncmp.c \
                          srcs/ft/ft_strndup.c \
                          srcs/ft/main_utils.c \
                          srcs/parsing/command.c \
                          srcs/parsing/heredoc.c \
                          srcs/parsing/parsing_utils.c \
                          srcs/parsing/pgroup.c \
                          srcs/parsing/pgroup_resolve.c \
                          srcs/parsing/redir.c \
                          srcs/parsing/resolve.c \
                          srcs/parsing/tokenize.c \
                          srcs/shell_init.c \
						  srcs/main.c \
						  srcs/parsing/heredoc_parse.c \
						  srcs/exec/sig.c \

                          
OBJS        := $(SRCS:.c=.o)

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
			${CC} ${FLAGS} -o ${NAME} ${OBJS}

all:		${NAME}

bonus:		all

clean:
			@ ${RM} $(OBJS)

fclean:		clean
			@ ${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re


