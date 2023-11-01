CC=gcc
CFLAGS=-I.
DEPS = shared_memory.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

producer: producer.o shared_memory.o
	$(CC) -o producer producer.o shared_memory.o

consumer: consumer.o shared_memory.o
	$(CC) -o consumer consumer.o shared_memory.o

clean:
	rm -f *.o shared_memory
