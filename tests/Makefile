CXX      := g++
CXXFLAGS := -std=c++17 -I../include -Wall -Wextra -O2
LDFLAGS  := -L.. -lg-lib -Wl,-rpath=..

SOURCES := $(wildcard *.cpp)
EXECUTABLES := $(SOURCES:.cpp=)

all: $(EXECUTABLES)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

test: all
	@for test in $(EXECUTABLES); do \
		echo "Running $$test..."; \
		./$$test; \
	done

clean:
	rm -f $(EXECUTABLES)

.PHONY: all test clean
