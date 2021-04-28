CC := gcc
CFLAGS := -Wall -g
CROSS_COMPILE := arm-none-linux-gnueabihf-
STRIP := strip

PROG = server
OBJS = server.o

all: $(PROG)

server: $(OBJS)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o $@ $^
	$(CROSS_COMPILE)$(STRIP) $@

%.o: %.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o $@ -c $^

clean:
	$(RM) -rf $(PROG) $(OBJS)