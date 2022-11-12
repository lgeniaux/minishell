# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/10 20:31:12 by lgeniaux          #+#    #+#              #
#    Updated: 2022/11/12 16:37:06 by lgeniaux         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRCS        :=	srcs/builtins/builtin_export.c \
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
				srcs/exec/sig.c
	
OBJS = $(patsubst %.c, %.o, $(SRCS))
NAME = minishell
CFLAGS = -I incs -Wall -Werror -Wextra -I ~/.brew/opt/readline/include 
LDFLAGS = -ltermcap -L ~/.brew/opt/readline/lib -lreadline

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

%.o: %.c Makefile incs/minishell.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS) minishell

re: fclean all

.PHONY: all fclean clean re
