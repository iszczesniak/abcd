TARGETS = sdi
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = client.o cdijkstra.o connection.o event.o module.o routing.o	\
sdi_args.o simulation.o stats.o utils.o utils_netgen.o mypoint.o	\
teventqueue.o traffic.o

CXXFLAGS := $(CXXFLAGS) -g
CXXFLAGS := $(CXXFLAGS) -std=c++11

# Use the C++ linker
LINK.o = $(LINK.cc)

ifdef BOOST_ROOT
	CXXFLAGS := $(CXXFLAGS) -I $(BOOST_ROOT)/include
	LDFLAGS := $(LDFLAGS) -L $(BOOST_ROOT)/lib
endif

LDFLAGS := $(LDFLAGS) -l boost_program_options

all: $(TARGETS)

sdi: $(OBJS)

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
	g++ -MM *.cc > dependencies

include dependencies
