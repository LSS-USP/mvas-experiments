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
LIBMVAS := lib/libmvas
#DEBUG := -D_DEBUG_
DEBUG :=

# Files to process
SRCFILES := $(shell find -name '*.c' -printf '%P ')
OBJFILES := $(basename $(subst $(SRCDIR),$(OBJDIR),$(SRCFILES)))

all: prepare $(TARGETS)

$(TARGETS): $(OBJFILES:%=%.o)
	$(CC) $(CCFLAGS) $^ $(LIBMVAS) -o $@ $(DEBUG)

$(OBJDIR)/%.o: $(basename $(subst $(OBJDIR),$(SRCDIR),$(OBJDIR)/%.o)).c
	$(CC) -o $@ -c $< $(INCLUDES) $(CCFLAGS) $(DEBUG)

.PHONY: clean, prepare

prepare:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(LIBDIR)/include
	$(MAKE) -C $(PATHTOLIB)
	cp $(PATHTOLIB)/lib/* lib/
	cp $(PATHTOLIB)/include/* lib/include

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
	rm -rf $(LIBDIR)/*
