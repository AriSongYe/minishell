# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sanahn <sanahn@student.42seoul.kr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/08 15:17:30 by yecsong           #+#    #+#              #
#    Updated: 2022/11/08 20:37:09 by sanahn           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

SRCS := parse.c
OBJS := $(SRCS:%.c=%.o)

CC := cc

CFLAGS := -Werror -Wextra -Wall

.PHONY : all clean fclean re

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -lreadline -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(OBJS)

fclean :
	@make clean
	rm -f $(NAME)

re :
	@make fclean
	@make all
