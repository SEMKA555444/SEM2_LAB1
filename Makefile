CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2

SRCS = main.cpp \
       data_structures/Array.cpp \
       data_structures/OneLinkedList.cpp \
       data_structures/DoublyLinkedList.cpp \
       data_structures/Stack.cpp \
       data_structures/Queue.cpp \
       data_structures/CompleteBinaryTree.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = dbms

all: $(TARGET) rust

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

rust:
	cd rust && cargo build --release
	cp rust/target/release/dbms_rust ./dbms_rust 2>/dev/null || true

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe dbms_rust dbms_rust.exe *.data
	cd rust 2>/dev/null && cargo clean 2>/dev/null || true

test: $(TARGET)
	@echo "=== Running C++ Tests ==="
	./$(TARGET) --file test.data --query "SPUSH mystack 42"
	./$(TARGET) --file test.data --query "STOP mystack"
	./$(TARGET) --file test.data --query "QPUSH myqueue 100"
	./$(TARGET) --file test.data --query "QPOP myqueue"
	./$(TARGET) --file test.data --query "TINSERT mytree 10"
	./$(TARGET) --file test.data --query "TINSERT mytree 20"
	./$(TARGET) --file test.data --query "TINSERT mytree 30"
	./$(TARGET) --file test.data --query "TISCOMPLETE mytree"
	./$(TARGET) --file test.data --query "ISMEMBER mytree 20"
	./$(TARGET) --file test.data --query "PRINT mytree"
	@rm -f test.data
	@echo "=== Tests completed successfully ==="

.PHONY: all rust clean test
