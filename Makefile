CC      = gcc
NAME    = push_swap
CFLAGS  = -Wall -Wextra -Werror -Wformat -g -DDEBUG
INC     = ./includes
SRCS    = $(addprefix ./srcs/, push_swap.c parse_input.c setup_stack.c \
						partition.c utils.c stack_operation.c sort.c \
						divide.c divide_three_a.c divide_three_b.c \
						divide_two_a.c divide_two_b.c)

OBJS    = $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
