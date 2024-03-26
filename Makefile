CC := gcc
LD := gcc

CFLAGS := -Wall -O2
LDFLAGS :=

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
EXES := $(SRCS:.c=)

all: $(EXES)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXES) signal.csv

.PHONY: all clean
