# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/03 12:32:52 by hnagashi          #+#    #+#              #
#    Updated: 2025/04/04 09:57:21 by hiroki           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = philo

SRCS = main.c ft_atoi.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -v $(CFLAGS) $(OBJS) -o $(NAME) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(OBJS) $(B_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: all

.PHONY: all clean fclean