TARGETS = sdi
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = client.o connection.o dijkstra.o event.o graph.o module.o	\
sdi_args.o stats.o utils.o utils_netgen.o mypoint.o teventqueue.o

#CXXFLAGS = -g -Wno-deprecated
CXXFLAGS = -O3 -Wno-deprecated

# Boost
#BOOSTPATH := ~/boost
#CXXFLAGS := $(CXXFLAGS) -I $(BOOSTPATH)/include
#:LDFLAGS := $(LDFLAGS) -L $(BOOSTPATH)/lib
#LDFLAGS := $(LDFLAGS) -l boost_program_options

CXXFLAGS := $(CXXFLAGS) -I /usr/include/boost

#LDFLAGS := $(LDFLAGS) -L $(BOOSTPATH)/lib
LDFLAGS := $(LDFLAGS) -L /lib 
LDFLAGS := $(LDFLAGS) -l boost_program_options -lboost_system -lstdc++ -lm





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
