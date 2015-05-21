# Makefile for compiling the Omni-optimizer source code

CC=gcc

LD=gcc

RM=rm -f

CFLAGS=-Wall -ansi -pedantic -O3

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

MAIN=omniopt

all:$(MAIN)

$(MAIN):$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(MAIN) -lm

%.o: %.c OmniOptOptimizer.h rand.h pdef.h
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJS)
