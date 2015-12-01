TESTS = cdijkstra graph edge_disjoint_ksp routing sdi_args utils 

OTHER = connected

OBJS = ../cdijkstra.o ../edksp.o ../mypoint.o ../routing.o	\
../sdi_args.o ../teventqueue.o ../utils.o ../utils_netgen.o

CXXFLAGS = -g -Wno-deprecated -std=c++11
#CXXFLAGS = -O3 -Wno-deprecated -std=c++11

CXXFLAGS := $(CXXFLAGS) -I ../

# Boost
BOOSTPATH = ~/boost
CXXFLAGS := $(CXXFLAGS) -I $(BOOSTPATH)/include
LDFLAGS := $(LDFLAGS) -L $(BOOSTPATH)/lib
LDFLAGS := $(LDFLAGS) -l boost_test_exec_monitor
LDFLAGS := $(LDFLAGS) -l boost_unit_test_framework
LDFLAGS := $(LDFLAGS) -l boost_program_options

.PHONY: clean depend run

all: $(TESTS) $(OTHER)

connected: connected.o $(OBJS)
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

cdijkstra: cdijkstra.o $(OBJS)
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

graph: graph.o $(OBJS)
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

edge_disjoint_ksp: edge_disjoint_ksp.o $(OBJS)
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

routing: routing.o $(OBJS)
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
	rm -rf $(TESTS) $(OTHER)

depend:
	g++ -MM -I../ *.cc > dependencies

include dependencies
