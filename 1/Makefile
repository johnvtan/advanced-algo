TARGET := p1a
SRCS := p1a.cpp
CC := g++
INCLUDES := ../includes 
CFLAGS := -Wall -Wextra -Wno-deprecated $(foreach incdir, $(INCLUDES), -I$(incdir))
LFLAGS := 

.PHONY: clean

all: 
	$(CC) $(CFLAGS) $(LFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm $(TARGET)