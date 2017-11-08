# Project directories
SRCDIR := src
INCDIR := include
BINDIR := bin
OBJDIR := obj

# Targets with main code inside
TARGETS := bin/simulate_web_server

# Required flags
CC := gcc
CCFLAGS := -Wall -lpthread -std=gnu11
INCLUDES := -I./$(INCDIR)/
#DEBUG := -D_DEBUG_
DEBUG :=
VERBOSE := -D_VERBOSE_
#LIBVAS := -lmvas

# Files to process
SRCFILES := $(shell find -name '*.c' -printf '%P ')
OBJFILES := $(basename $(subst $(SRCDIR),$(OBJDIR),$(SRCFILES)))

all: prepare $(TARGETS)

$(TARGETS): $(OBJFILES:%=%.o)
	$(CC) $(CCFLAGS) $^ -o $@ $(DEBUG) $(VERBOSE) $(LIBVAS)

$(OBJDIR)/%.o: $(basename $(subst $(OBJDIR),$(SRCDIR),$(OBJDIR)/%.o)).c
	$(CC) -o $@ -c $< $(INCLUDES) $(CCFLAGS) $(DEBUG) $(VERBOSE) $(LIBVAS)

.PHONY: clean, prepare

prepare:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
