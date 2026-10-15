CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -g

TARGET = test_rbtree
OBJS = main.c rbtree.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET)