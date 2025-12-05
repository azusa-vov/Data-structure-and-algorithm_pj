# 编译器设置
CC = g++
# 编译选项：显示警告、使用 C++11 标准、包含调试信息
CFLAGS = -Wall -std=c++11 -g

# 目标文件列表
OBJS = main.o LinkedList.o CircularQueue.o BST.o UndoStack.o KMP.o

# 最终生成的可执行文件名
TARGET = loganalyzer

# 默认目标
all: $(TARGET)

# 链接步骤
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 编译各个源文件
main.o: main.cpp LinkedList.h CircularQueue.h BST.h UndoStack.h KMP.h
	$(CC) $(CFLAGS) -c main.cpp

LinkedList.o: LinkedList.cpp LinkedList.h LogEntry.h
	$(CC) $(CFLAGS) -c LinkedList.cpp

CircularQueue.o: CircularQueue.cpp CircularQueue.h LogEntry.h
	$(CC) $(CFLAGS) -c CircularQueue.cpp

BST.o: BST.cpp BST.h
	$(CC) $(CFLAGS) -c BST.cpp

UndoStack.o: UndoStack.cpp UndoStack.h LinkedList.h BST.h CircularQueue.h
	$(CC) $(CFLAGS) -c UndoStack.cpp

KMP.o: KMP.cpp KMP.h
	$(CC) $(CFLAGS) -c KMP.cpp

# 清理规则
clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标
.PHONY: all clean