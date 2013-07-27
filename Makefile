TARGETS = first
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

<<<<<<< HEAD
OBJS = dijkstra.o graph.o utils_netgen.o
=======
OBJS = graph.o utils.o utils_netgen.o
>>>>>>> f6050d1dff71cc4d72398d8da98e9e6daa23e4ec

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O3 -Wno-deprecated

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
