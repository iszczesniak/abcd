TARGETS = test
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = graph.o test.o

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O3 -Wno-deprecated

# Boost
LDFLAGS := $(LDFLAGS) -l boost_graph-mt

all: $(TARGETS)

test: $(OBJS)

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
