LIBDIR := lib
OBJDIR := obj
SRCDIR := src
INCDIR := include

TARGETS := lib/libmvas

CCFLAGS := -Wall -std=gnu99
CC := gcc
AR := ar
LIBFLAGS := rcs
INCLUDE := -I./$(INCDIR)/
DEBUG :=
#DEBUG := -D_DEBUG_

SRCFILES := $(shell find -name '*.c' -printf '%P ')
OBJFILES := $(basename $(subst $(SRCDIR),$(OBJDIR),$(SRCFILES)))

all: prepare $(TARGETS)

$(TARGETS): $(OBJFILES:%=%.o)
	$(AR) $(LIBFLAGS) $@ $^

$(OBJDIR)/%.o: $(basename $(subst $(OBJDIR),$(SRCDIR),$(OBJDIR)/%.o)).c
	$(CC) -o $@ -c $< $(INCLUDE) $(CCFLAGS) $(DEBUG)

.PHONY: clean, prepare

prepare:
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(LIBDIR)/*
