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
MVAS_ENABLED := -D_MVAS_
LIBVAS := -lmvas

# Files to process
SRCFILES := $(shell find -name '*.c' -printf '%P ')
OBJFILES := $(basename $(subst $(SRCDIR),$(OBJDIR),$(SRCFILES)))

all: prepare mvas

nothing: prepare $(TARGETS)

mvas: $(OBJFILES:%=%.o)
	$(CC) $(CCFLAGS) $^ -o $(TARGETS) $(DEBUG) $(VERBOSE) $(MVAS_ENABLED) $(LIBVAS)

$(TARGETS): $(OBJFILES:%=%.o)
	$(CC) $(CCFLAGS) $^ -o $@ $(DEBUG) $(VERBOSE)

$(OBJDIR)/%.o: $(basename $(subst $(OBJDIR),$(SRCDIR),$(OBJDIR)/%.o)).c
	$(CC) -o $@ -c $< $(INCLUDES) $(CCFLAGS) $(DEBUG) $(VERBOSE) $(LIBVAS) $(MVAS_ENABLED)

.PHONY: clean, prepare

prepare:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
