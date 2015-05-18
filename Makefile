include Makefile.$(shell hostname -s)

RUN := zlp-condense
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard include/*.h)
OBJECTS := $(SOURCES:.cpp=.o)

CFLAGS := -Iinclude -I$(shell pwd)/external/rapidjson/include $(CFLAGS_EXTRA)
LDFLAGS := -lcfitsio -lbz2 $(LDFLAGS_EXTRA)
COMMON := -g -O0 -Wall -Wextra -std=c++11 $(COMMON_EXTRA)

all: $(RUN)

$(RUN): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(COMMON)

%.o: %.cpp $(HEADERS)
	$(CXX) -c $< -o $@ $(CFLAGS) $(COMMON)

.PHONY: clean

clean:
	@rm -f $(OBJECTS)
	@rm -rf $(RUN)
