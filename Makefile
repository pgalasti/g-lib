UNAME_S := $(shell uname -s)

LIB_NAME := g-lib

CXX      := g++
CXXFLAGS := -std=c++17 -fPIC -Iinclude -Wall -Wextra -O2

ifeq ($(UNAME_S),Darwin)          # macOS
    SHARED_EXT := dylib
    LDFLAGS := -shared
else                               # Linux
    SHARED_EXT := so
    LDFLAGS := -shared
endif

TARGET := lib$(LIB_NAME).$(SHARED_EXT)

SRC_DIR  := src
INC_DIR  := include

PREFIX      := /usr/local
INSTALL_LIB := $(PREFIX)/lib
INSTALL_INC := $(PREFIX)/include/$(LIB_NAME)

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(TARGET)
	mkdir -p $(INSTALL_LIB)
	mkdir -p $(INSTALL_INC)
	install -m 0755 $(TARGET) $(INSTALL_LIB)/$(TARGET)
	cp -r $(INC_DIR)/* $(INSTALL_INC)/

	@echo "Installed $(TARGET) into $(INSTALL_LIB)"
	@echo "Installed headers into $(INSTALL_INC)"

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean install

