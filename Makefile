NAME = lem-in

SRCS = main.c parse.c vec.c priority_queue.c utils.c


CC = gcc

#FLAGS = -c -Wall -Wextra -Werror

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
		$(CC) $(OBJS) -o $(NAME)

clean : 
		rm -rf $(OBJS)

fclean : clean 
		rm -rf $(NAME)

re : fclean all

bonus : re

.PHONY:	all clean fclean reNAME = push_swap