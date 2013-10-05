TARGETS = first
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = dijkstra.o graph.o utils.o utils_netgen.o

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O1 -Wno-deprecated -std=c++11

# Boost
LDFLAGS := $(LDFLAGS) -l boost_graph-mt

all: $(TARGETS)

first: $(OBJS)

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
