CC = gcc
CFLAGS = -Wall -Wextra
INCLUDES = -I/usr/includes/SDL2
LIBS = -lSDL2
SOURCE = snake.c snakeFunc.c
OBJS = $(SOURCE:.c=.o)
MAIN = snake

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
