OBJECTS = main.o
CC=gcc
CFLAGS= -c
TARGET=MAIN

$(TARGET): $(OBJECTS)
	     $(CC) -o $(TARGET) $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(TARGET)

run:
	./MAIN

main.o: main.c
