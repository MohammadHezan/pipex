CC = @cc
CFLAGS = -Wall -Wextra -Werror
RM = @rm -rf
NAME = pipex
SRCS = srcs/Hpipex.c srcs/Hutils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@make -s -C libft
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@make -s clean -C libft

fclean: clean
	$(RM) $(NAME)
	@make -s fclean -C libft

re: fclean all

.PHONY: all clean fclean re
