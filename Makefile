TARGET   = server.out
TARGET_C = client.out

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = \
	   include

SRCS = \
       src/main.c \
       src/socket.c
SRCS_C = \
       src/main_2.c \
       src/socket.c

.PHONY: all clean

all: $(SRCS) $(SRCS_C)
	$(CC) -Wall -Wextra -Werror $(addprefix -I, $(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS)
	$(CC) -Wall -Wextra -Werror $(addprefix -I, $(HDRS)) -o $(TARGET_C) $(CFLAGS) $(SRCS_C)

clean:
	rm -rf $(TARGET)
	rm -rf $(TARGET_C)