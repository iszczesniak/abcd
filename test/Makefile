TESTS = arr_queue_test distro_test geometric_test nodistro_test	\
poisson_test polynomial_test serialization_test tabdistro_test	\
utils_test utils_sim_test test_arr_queue

PERFORM = test_adm test_rou

OBJS = ../analysis.o ../arguments.o ../arr_queue.o ../distro.o		\
../edge_probs.o ../generate.o ../graph.o ../graph_serialization.o	\
../nodistro.o ../packet.o ../poisson.o ../rand.o ../rou_order.o		\
../serializer.o ../simulation.o ../tabdistro.o ../utils_ana.o		\
../utils.o ../utils_netgen.o ../utils_sim.o ../test.o

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O3 -Wno-deprecated

CXXFLAGS := $(CXXFLAGS) -I ../

# Boost
#BOOSTPATH = /usr/local/boost-1.43
#CXXFLAGS := $(CXXFLAGS) -I $(BOOSTPATH)/include
#LDFLAGS := $(LDFLAGS) -L $(BOOSTPATH)/lib
LDFLAGS := $(LDFLAGS) -l boost_regex-mt
LDFLAGS := $(LDFLAGS) -l boost_program_options-mt
LDFLAGS := $(LDFLAGS) -l boost_graph-mt
LDFLAGS := $(LDFLAGS) -l boost_serialization-mt

# GNU Scientific Library
LDFLAGS := $(LDFLAGS) -l gslcblas -l gsl

.PHONY: clean depend run

all: $(TESTS)

arr_queue_test: ../arr_queue.o ../nodistro.o ../poisson.o
distro_test: ../nodistro.o ../poisson.o
geometric_test: ../geometric.o
nodistro_test: ../nodistro.o
poisson_test: ../poisson.o
polynomial_test: $(OBJS)
serialization_test: $(OBJS)
tabdistro_test: ../distro.o ../nodistro.o ../poisson.o ../tabdistro.o
utils_test: $(OBJS)
utils_sim_test: $(OBJS)
test_1: $(OBJS)
test_2: $(OBJS)

# These guys produce files that are submitted to SVN
test_adm: $(OBJS)
test_arr_queue: ../arr_queue.o ../poisson.o ../geometric.o
test_rou: $(OBJS)

# run the tests
run:
	@for i in $(TESTS); do echo "Running" $$i; ./$$i; done

# run the performance evaluation
perform:
	@for i in $(PERFORM); do echo "Running" $$i; ./$$i; done

count:
	wc -l *.hpp *.cc

clean:
	rm -rf *~
	rm -rf *.o
	rm -rf $(TESTS)

depend:
	g++ -MM -I../ *.cc > dependencies

include dependencies
