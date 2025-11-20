CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

TARGET = gnl

SRCS = ./main.c ./get_next_line.c ./get_next_line_utils.c ./get_next_line_bonus.c ./get_next_line_utils_bonus.c

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean 
	$(MAKE) all

d: re
	gdb gnl


.PHONY: all clean fclean re
