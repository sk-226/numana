# Makefile

CC = gcc
CFLAGS = -I./inc
OBJDIR = obj
SRCDIR = src
INCDIR = inc
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
MAINOBJ = $(OBJDIR)/main.o

all: numana

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MAINOBJ): main.c
	$(CC) $(CFLAGS) -c main.c -o $(MAINOBJ)

numana: $(OBJECTS) $(MAINOBJ)
	$(CC) $(OBJECTS) $(MAINOBJ) -o numana -lm

clean:
	rm -f $(OBJDIR)/*.o numana
