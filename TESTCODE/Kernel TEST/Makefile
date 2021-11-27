CC = gcc
CFLAGS  = -g -pthread -Wall 
 
TARGET = test

all: $(TARGET)
  
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
