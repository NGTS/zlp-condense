include Makefile.$(shell hostname -s)

CFLAGS=-I$(CFITSIO)/include -I$(BOOST) -I$(TCLAP)/include
LDFLAGS=-L$(CFITSIO)/lib -lcfitsio
COMMON=-Wall -Wextra -Wno-deprecated-writable-strings -pthread

ifdef RELEASE
	COMMON += -O2
else
	COMMON += -O0 -g 
endif

CXX=clang++
RUN=bin/condense

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(RUN)

$(RUN): $(OBJECTS)
	$(CXX) $^ -o $@ $(COMMON) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $< -o $@ -c $(COMMON) $(CFLAGS) -Iinclude
 
clean:
	rm -f $(OBJECTS)
	rm -f $(RUN)
