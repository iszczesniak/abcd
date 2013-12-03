TARGETS = sdi
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = client.o connection.o dijkstra.o event.o graph.o module.o	\
sdi_args.o stats.o utils.o utils_netgen.o

#CXXFLAGS = -g -Wno-deprecated -std=c++11
CXXFLAGS = -O1 -Wno-deprecated -std=c++11

# Boost
#BOOSTPATH := /usr/local/boost-1.43
#CXXFLAGS := $(CXXFLAGS) -I $(BOOSTPATH)/include
#LDFLAGS := $(LDFLAGS) -L $(BOOSTPATH)/lib
LDFLAGS := $(LDFLAGS) -l boost_program_options-mt

# Boost
LDFLAGS := $(LDFLAGS) -l boost_graph-mt

all: $(TARGETS)

sdi: $(OBJS)

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
