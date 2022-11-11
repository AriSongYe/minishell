# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yecsong <yecsong@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/01 11:39:28 by yecsong           #+#    #+#              #
#    Updated: 2022/05/18 14:06:21 by yecsong          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := libft.a

SRCS := ft_memchr.c\
	ft_strchr.c\
	ft_strchr.c\
	ft_strrchr.c\
	ft_atoi.c\
	ft_memcmp.c\
	ft_strdup.c\
	ft_strtrim.c\
	ft_bzero.c\
	ft_memcpy.c\
	ft_striteri.c\
	ft_substr.c\
	ft_calloc.c\
	ft_memmove.c\
	ft_strjoin.c\
	ft_tolower.c\
	ft_isalnum.c\
	ft_memset.c\
	ft_strlcat.c\
	ft_toupper.c\
	ft_isalpha.c\
	ft_putchar_fd.c\
	ft_strlcpy.c\
	ft_isascii.c\
	ft_putendl_fd.c\
	ft_strlen.c\
	ft_isdigit.c\
	ft_putnbr_fd.c\
	ft_strmapi.c\
	ft_isprint.c\
	ft_putstr_fd.c\
	ft_strncmp.c\
	ft_itoa.c\
	ft_split.c\
	ft_strnstr.c

SRCS_BONUS := ft_lstnew.c\
	ft_lstadd_front.c\
	ft_lstadd_back.c\
	ft_lstsize.c\
	ft_lstdelone.c\
	ft_lstclear.c\
	ft_lstiter.c\
	ft_lstmap.c\
	ft_lstlast.c

OBJS = $(SRCS:%.c=%.o)

OBJS_BONUS = $(SRCS_BONUS:%.c=%.o)


ifdef BONUS_CHECK
OBJS_FILES = $(OBJS) $(OBJS_BONUS)
else
OBJS_FILES = $(OBJS)
endif

AR := ar

ARFLAGS := src

CC := cc

CFLAGS := -Werror -Wextra -Wall

all : $(NAME)

$(NAME) : $(OBJS_FILES)
	$(AR) $(ARFLAGS) $@ $?

bonus :
	make BONUS_CHECK=1 all

re :
	make fclean
	make all

clean :
	rm -f *.o

fclean : clean
	rm -f $(NAME)
.PHONY : all clean fclean bonus re
