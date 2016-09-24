TARGETS = abcd
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = cli_args.o client.o cdijkstra.o connection.o edksp.o routing.o	\
stats.o utils.o utils_netgen.o mypoint.o teventqueue.o traffic.o	\
yenksp.o

CXXFLAGS := $(CXXFLAGS) -g
CXXFLAGS := $(CXXFLAGS) -std=c++11
CXXFLAGS := $(CXXFLAGS) -I include

# Use the C++ linker
LINK.o = $(LINK.cc)

ifdef BOOST_ROOT
	CXXFLAGS := $(CXXFLAGS) -I $(BOOST_ROOT)/include
	LDFLAGS := $(LDFLAGS) -L $(BOOST_ROOT)/lib
endif

LDFLAGS := $(LDFLAGS) -l boost_program_options
LDFLAGS := $(LDFLAGS) -l boost_timer
LDFLAGS := $(LDFLAGS) -l boost_system

all: $(TARGETS)

abcd: $(OBJS)

try: graph.o try.o

.PHONY: clean count depend test

clean:
	rm -f *~
	rm -f $(OBJS)
	rm -f $(TARGET_OBJS)
	rm -f $(TARGETS)

count:
	wc -l *.hpp *.cc

depend:
	g++ $(CXXFLAGS) -MM *.cc > dependencies

include dependencies
