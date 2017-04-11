CXX = g++
CFLAGS = -mssse3 -std=c++0x 
INCS = -I$(RTE_SDK)/$(RTE_TARGET)/include -I./src
LIBDIR = -L$(RTE_SDK)/$(RTE_TARGET)/lib
LDFLAGS = -lprotobuf -lpthread -ldl -Wl,--whole-archive -ldpdk -ldl -libverbs -Wl,--no-whole-archive

WARNING = -Wall
DEBUGON = -g
OPT_DEFAULT = -O3
OPT_DEBUG = -O0

SRCDIR = src
OBJDIR = obj
BINDIR = build
DEBUGDIR = debug

SRCS := $(shell find $(SRCDIR) -name '*.cc')
SRCDIRS = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
OBJS := $(patsubst $(SRCDIRS)/%.cc,$(OBJDIR)/%.o,$(SRCS))

default: $(BINDIR)/micronf

debug: $(DEBUGDIR)/micronf

$(BINDIR)/micronf: $(OBJS)
	$(CXX) $(CFLAGS) $(OPT_DEFAULT) $(INCS) $(OBJS) $(LDFLAGS) $(LIBDIR) -o $@

$(DEBUGDIR)/micronf: $(OBJS)
	$(CXX) $(DEBUGON) $(CFLAGS) $(OPT_DEBUG) $(INCS) $(OBJS) $(LDFLAGS) $(LIBDIR) -o $@

$(OBJDIR)/%.o: $(SRCDIRS)/%.cc
		$(CXX) -c $(CFLAGS) $(INCS) $< -o $@
	
clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(BINDIR)/*
