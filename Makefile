CFLAGS=
LDFLAGS=
COMMON=-g -Wall -Wextra
CXX=clang++
RUN=condense

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(RUN)

condense: $(OBJECTS)
	$(CXX) $^ -o $@ $(COMMON) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $< -o $@ -c $(COMMON) $(CFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f $(RUN)
