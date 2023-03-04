CC=clang
CXX=clang++
LLVM_CONFIG=llvm-config

CFLAGS=$(shell $(LLVM_CONFIG) --cxxflags) -fPIC
LDFLAGS=$(shell $(LLVM_CONFIG) --ldflags)
LIBS=$(shell $(LLVM_CONFIG) --libs core irreader)

SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=out/taiga

all: run clean

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE) tests/test1.tai tests/out

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY: clean