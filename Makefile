include Makefile.$(shell hostname -s)

RUN := zlp-condense
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard include/*.h)
OBJECTS := $(SOURCES:.cpp=.o)

CXX := g++

CFLAGS := -I$(FITSPPDIR) -Iinclude -I$(shell pwd)/external/rapidjson/include
LDFLAGS := -L$(FITSPPDIR) -lfits++ -lcfitsio -lbz2
COMMON := -g -O0 -Wall -Wextra -std=c++11

all: $(RUN)

$(RUN): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(COMMON)

%.o: %.cpp $(HEADERS)
	$(CXX) -c $< -o $@ $(CFLAGS) $(COMMON)

.PHONY: clean

clean:
	@rm -f $(OBJECTS)
	@rm -rf $(RUN)
