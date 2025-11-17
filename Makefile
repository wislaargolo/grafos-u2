# Makefile para compilar os testes em tests/ e colocar executáveis em build/
# Uso:
#   make           # compila todos os testes
#   make q7        # compila apenas tests/q7.cpp -> build/q7
#   make clean

CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 -I.

SRC_DIR := tests
BUILD_DIR := build
EXTRA_SOURCES ?=

TEST_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
TEST_BINS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%,$(TEST_SRCS))
TEST_NAMES := $(notdir $(basename $(TEST_SRCS)))

.PHONY: all clean $(TEST_NAMES)

all: $(TEST_BINS)

$(BUILD_DIR)/%: $(SRC_DIR)/%.cpp $(EXTRA_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(EXTRA_SOURCES) $< -o $@
	@echo "Built $@"

# permite executar `make q7` (vai depender de build/q7)
$(TEST_NAMES): %: $(BUILD_DIR)/%

clean:
	rm -rf $(BUILD_DIR)
