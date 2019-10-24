TARGET = main.out

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = \
	   include

SRCS = \
       src/main.c \
       src/socket.c

.PHONY: all clean

all: $(SRCS)
	$(CC) -Wall -Wextra -Werror $(addprefix -I, $(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)
