CC = gcc
CFLAGS  = -g -pthread
 
TARGET = test

all: $(TARGET)
  
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
