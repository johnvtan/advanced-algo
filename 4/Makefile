PROBLEM := p4
TARGET := $(PROBLEM).out
SRCS := $(PROBLEM).cpp
CC := g++
INCLUDES := ../includes 
CFLAGS := -Wall -Wextra -Wno-deprecated $(foreach incdir, $(INCLUDES), -I$(incdir))
LFLAGS := 

.PHONY: clean

all: 
	$(CC) $(CFLAGS) $(LFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm $(TARGET)
