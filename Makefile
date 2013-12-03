TARGETS = sdi
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = client.o connection.o dijkstra.o event.o graph.o module.o	\
stats.o utils.o utils_netgen.o

#CXXFLAGS = -g -Wno-deprecated -std=c++11
CXXFLAGS = -O1 -Wno-deprecated -std=c++11

# Boost
LDFLAGS := $(LDFLAGS) -l boost_graph-mt

all: $(TARGETS)

first: $(OBJS)

second: $(OBJS)

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
