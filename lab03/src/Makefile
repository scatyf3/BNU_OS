CC := gcc
CFLAGS := -Wall -Wextra

# 目标文件
TARGETS := producer consumer

# 默认目标
all: $(TARGETS)

# producer规则
producer: producer.o shared_memory.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

producer.o: producer.c shared_memory.h
	$(CC) $(CFLAGS) -c $<

# consumer规则
consumer: consumer.o shared_memory.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

consumer.o: consumer.c shared_memory.h
	$(CC) $(CFLAGS) -c $<

# shared_memory规则
shared_memory.o: shared_memory.c shared_memory.h
	$(CC) $(CFLAGS) -c $<

# 清理生成的文件
clean:
	rm -f $(TARGETS) *.o
