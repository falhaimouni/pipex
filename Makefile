CC = cc
CFLAGS = -Wall -Werror -Wextra
NAME = pipex

SRCS = pipex.c ft_split.c helper.c path.c utils.c utils1.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean:clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
