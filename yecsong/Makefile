# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/08 15:17:30 by yecsong           #+#    #+#              #
#    Updated: 2022/11/09 16:15:59 by yecsong          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

SRCS := read_line.c\

CC := cc

CFLAGS := -Werror -Wextra -Wall -lreadline

all : $(NAME)

$(NAME) :
		$(MAKE) -C ./libft all
		$(CC) $(CFLAGS) $(SRCS) ./libft/libft.a

re : fclean all

clean :
	rm -f *.o

fclean : clean
	rm -f $(NAME)

.PHONY : all clean fclean bonus re
