RUN := zlp-condense
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

CFLAGS := -Ifits++
LDFLAGS := -Lfits++ -lfits++ -lcfitsio
COMMON := -g -O0

CXX := g++

CFLAGS :=
LDFLAGS := -lfits++ -lcfitsio
COMMON := -g -O0

include Makefile.$(shell hostname -s)

all: $(RUN)

$(RUN): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(COMMON)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(COMMON)

.PHONY: clean

clean:
	@rm -f $(OBJECTS)
	@rm -rf $(RUN)
