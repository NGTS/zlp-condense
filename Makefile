include Makefile.$(shell hostname -s)

RUN := zlp-condense
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

CFLAGS := -Ifits++
LDFLAGS := -Lfits++ -lfits++ -lcfitsio
COMMON := -g -O0

CXX := g++

CFLAGS := -I$(FITSPPDIR) -Iinclude
LDFLAGS := -L$(FITSPPDIR) -lfits++ -lcfitsio -lbz2
COMMON := -g -O0 -Wall -Wextra


all: $(RUN)

$(RUN): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(COMMON)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(COMMON)

.PHONY: clean

clean:
	@rm -f $(OBJECTS)
	@rm -rf $(RUN)
