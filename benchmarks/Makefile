CXX      := g++
CXXFLAGS := -std=c++23 -I../include -Wall -Wextra -O2
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

bench: all
	@for bench in $(EXECUTABLES); do \
		echo "Running $$bench..."; \
		LD_LIBRARY_PATH=..:$$LD_LIBRARY_PATH ./$$bench; \
		echo ""; \
	done

clean:
	rm -f $(EXECUTABLES)

.PHONY: all bench clean
