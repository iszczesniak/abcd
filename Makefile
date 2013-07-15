TARGETS = test
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = graph.o

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O3 -Wno-deprecated

# Boost
#BOOSTPATH := /usr/local/boost-1.43
#CXXFLAGS := $(CXXFLAGS) -I $(BOOSTPATH)/include
#LDFLAGS := $(LDFLAGS) -L $(BOOSTPATH)/lib
LDFLAGS := $(LDFLAGS) -l boost_filesystem-mt
LDFLAGS := $(LDFLAGS) -l boost_graph-mt
LDFLAGS := $(LDFLAGS) -l boost_program_options-mt
LDFLAGS := $(LDFLAGS) -l boost_regex-mt
LDFLAGS := $(LDFLAGS) -l boost_serialization-mt

# GNU Scientific Library
LDFLAGS := $(LDFLAGS) -l gslcblas -l gsl

all: $(TARGETS)

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
