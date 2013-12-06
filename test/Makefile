TESTS = sdi_args dijkstra graph utils

OBJS = ../dijkstra.o ../graph.o ../sdi_args.o ../utils.o

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O3 -Wno-deprecated

CXXFLAGS := $(CXXFLAGS) -I ../

# Boost
#BOOSTPATH = /usr/local/boost-1.43
#CXXFLAGS := $(CXXFLAGS) -I $(BOOSTPATH)/include
#LDFLAGS := $(LDFLAGS) -L $(BOOSTPATH)/lib
LDFLAGS := $(LDFLAGS) -l boost_test_exec_monitor-mt
LDFLAGS := $(LDFLAGS) -l boost_unit_test_framework-mt
LDFLAGS := $(LDFLAGS) -l boost_program_options-mt

.PHONY: clean depend run

all: $(TESTS)

dijkstra: dijkstra.o $(OBJS)
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

graph: graph.o $(OBJS)
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

sdi_args: sdi_args.o $(OBJS)
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

utils: utils.o $(OBJS)
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

# run the tests
run:
	@for i in $(TESTS); do echo "Running" $$i; ./$$i; done

count:
	wc -l *.hpp *.cc

clean:
	rm -rf *~
	rm -rf *.o
	rm -rf $(TESTS)

depend:
	g++ -MM -I../ *.cc > dependencies

include dependencies
