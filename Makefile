CXX = g++
CFLAGS = -mssse3 -std=c++0x -O3
INCS = -I$(RTE_SDK)/$(RTE_TARGET)/include
LIBDIR = -L$(RTE_SDK)/$(RTE_TARGET)/lib
LDFLAGS = -ldpdk -lpthread -ldl

WARNING = -Wall
DEBUGON = -g

SRCDIR = src
OBJDIR = obj
BINDIR = build

SRCS := $(shell find $(SRCDIR) -name '*.cc')
SRCDIRS = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
OBJS := $(patsubst $(SRCDIRS)/%.cc,$(OBJDIR)/%.o,$(SRCS))

default: $(BINDIR)/micronf

$(BINDIR)/micronf: $(OBJS)
	$(CXX) $(DEBUGON) $(CFLAGS) $(INCS) $(OBJS) $(LDFLAGS) $(LIBDIR) -o $@

$(OBJDIR)/%.o: $(SRCDIRS)/%.cc
		$(CXX) -c $(CFLAGS) $(INCS) $< -o $@
	
clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(BINDIR)/*
