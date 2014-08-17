CFITSIO=/usr/local
BOOST=/usr/local
CFLAGS=-I$(CFITSIO)/include -I$(BOOST)/include
LDFLAGS=-L$(CFITSIO)/lib -lcfitsio
COMMON=-g -O0 -Wall -Wextra -Wno-deprecated-writable-strings
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
