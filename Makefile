CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
NAME = pipex
SRCS = srcs/Hpipex.c srcs/Hutils.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(NAME)

# Build pipex and link with libft
$(NAME): $(OBJS)
	@make -s -C libft
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -o $(NAME)

# Compile source to object
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove object files
clean:
	$(RM) $(OBJS)
	@make -s clean -C libft

# Remove objects and binary
fclean: clean
	$(RM) $(NAME)
	@make -s fclean -C libft

# Full rebuild
re: fclean all

.PHONY: all clean fclean re
