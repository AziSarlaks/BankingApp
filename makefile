CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0` -lsqlite3
OBJS = main.o database.o encoder.o
TARGET = bank_app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

main.o: main.c database.h
	$(CC) $(CFLAGS) -c main.c

database.o: database.c database.h
	$(CC) $(CFLAGS) -c database.c

encoder.o: encoder.c encoder.h
	$(CC) $(CFLAGS) -c encoder.c

clean:
	rm -f $(OBJS) $(TARGET) clients.db

.PHONY: all clean