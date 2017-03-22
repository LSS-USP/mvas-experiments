# Project directories
SRCDIR := src
INCDIR := include
BINDIR := bin
OBJDIR := obj
LIBDIR := lib

PATHTOLIB := ../libmvas

# Targets with main code inside
#TARGETS := bin/hello_vas
TARGETS := bin/simpleHelloVas

# Required flags
CC := gcc
CCFLAGS := -Wall -lpthread -std=gnu99
INCLUDES := -I./$(INCDIR)/ -I./$(LIBDIR)/include/
LIBMVAS := -lmvas
#DEBUG := -D_DEBUG_
DEBUG :=

# Files to process
SRCFILES := $(shell find -name '*.c' -printf '%P ')
OBJFILES := $(basename $(subst $(SRCDIR),$(OBJDIR),$(SRCFILES)))

all: prepare $(TARGETS)

$(TARGETS): $(OBJFILES:%=%.o)
	$(CC) $(CCFLAGS) $^ -o $@ $(DEBUG) $(LIBMVAS)

$(OBJDIR)/%.o: $(basename $(subst $(OBJDIR),$(SRCDIR),$(OBJDIR)/%.o)).c
	$(CC) -o $@ -c $< $(INCLUDES) $(CCFLAGS) $(DEBUG) $(LIBMVAS)

.PHONY: clean, prepare

prepare:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(LIBDIR)/include

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
