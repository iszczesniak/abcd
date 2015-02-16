TARGETS = sdi
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = client.o connection.o dijkstra.o event.o graph.o module.o	\
sdi_args.o stats.o utils.o utils_netgen.o mypoint.o teventqueue.o

#CXXFLAGS = -std=c++11 -g -Wno-deprecated
CXXFLAGS = -std=c++11 -O3 -Wno-deprecated

LDFLAGS := -l boost_program_options

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
