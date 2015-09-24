
CPPFLAGS := $(shell root-config --cflags)
CXXFLAGS := -std=c++0x -g
LDFLAGS  := $(shell root-config --libs) -L $(ROOTSYS/lib) -lRooFit -lRooFitCore

TGTS := main

EXES := ${TGTS}

SRCS := $(wildcard *.cpp)
DEPS := $(SRCS:.cpp=.d)
OBJS := $(SRCS:.cpp=.o)

-include ${DEPS}

main: ${OBJS} 
	${CXX} ${CXXFLAGS} -o $@ ${OBJS}  ${LDFLAGS}

${OBJS}: %.o: %.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $<

HEADERS := $(wildcard *.h)

dict.cc: ${HEADERS} 
	rootcling -f $@ -c -p $^

clean:
	-rm -f ${EXES} *.d *.o *.pcm *~
