CC = gcc
CFLAGS = -g -Wall -std=c99 -pthread

TARGET = knapsack

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

debug:
	$(CC) -D DEBUG $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
