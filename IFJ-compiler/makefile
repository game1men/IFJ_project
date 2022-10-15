EXEC = compiler
CC = gcc
CFLAGS = -g -std=c11 -Wall -Wextra -pedantic
#LDLIBS = -lm

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

.PHONY: clean all

all: $(EXEC)

$(EXEC): $(SRCS)

clean:
	rm -f $(EXEC) $(OBJS) $(EXEC)