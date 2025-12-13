CXX      := g++
CXXFLAGS := -std=c++17 -I../include -Wall -Wextra -O2
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS  := -L.. -lg-lib -Wl,-rpath,@loader_path/..
else
	LDFLAGS  := -L.. -lg-lib -Wl,-rpath,'$$ORIGIN/..'
endif

SOURCES := $(wildcard *.cpp)
EXECUTABLES := $(SOURCES:.cpp=)

all: $(EXECUTABLES)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

test: all
	@for test in $(EXECUTABLES); do \
		echo "Running $$test..."; \
		LD_LIBRARY_PATH=..:$$LD_LIBRARY_PATH ./$$test; \
	done

clean:
	rm -f $(EXECUTABLES)

.PHONY: all test clean
