CC := gcc

CFLAGS := -std=c99

HDRS := src/vm.h

SRCS := src/main.c src/parser.c

OBJS := $(SRCS:.c=.o)

EXEC := marsh

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

$(OBJS): $(@:.o=.c) $(HDRS) Makefile
	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

install:
	cp $(EXEC) /usr/bin

.PHONY: all install
