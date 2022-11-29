# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/08 15:17:30 by yecsong           #+#    #+#              #
#    Updated: 2022/11/29 17:11:48 by yecsong          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

SRCS := main.c unit_chunk.c unit_token.c unit_syntax.c unit_cmd.c \
	tokenize.c tokenize_metachar.c handler_dollar_qutoes.c utils.c \
	execute.c
OBJS := $(SRCS:%.c=%.o)

CC := cc

CFLAGS := -Werror -Wextra -Wall

.PHONY : all clean fclean re

all : $(NAME)

$(NAME) : $(OBJS)
	@make -C ./libft all
	$(CC) $(CFLAGS) -o $@ $^ ./libft/libft.a -lreadline

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	@make -C ./libft clean
	rm -f $(OBJS)

fclean :
	@make clean
	@make -C ./libft fclean
	rm -f $(NAME)

re :
	@make fclean
	@make all
