# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/08 15:17:30 by yecsong           #+#    #+#              #
#    Updated: 2022/11/08 15:20:33 by yecsong          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

SRCS := read_line.c\

CC := cc

CFLAGS := -Werror -Wextra -Wall -lreadline

all : $(NAME)

$(NAME) :
		$(CC) $(CFLAGS) $(SRCS)

re : fclean all

clean :
	rm -f *.o

fclean : clean
	rm -f $(NAME)

.PHONY : all clean fclean bonus re
