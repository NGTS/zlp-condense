CFITSIO=/usr/local
CFLAGS=-I$(CFITSIO)/include
LDFLAGS=-L$(CFITSIO)/lib -lcfitsio
COMMON=-g -Wall -Wextra -std=c++0x -stdlib=libc++
CXX=clang++
RUN=bin/condense

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(RUN)

$(RUN): $(OBJECTS)
	$(CXX) $^ -o $@ $(COMMON) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $< -o $@ -c $(COMMON) $(CFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f $(RUN)
