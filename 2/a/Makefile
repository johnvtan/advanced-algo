TARGET := p2a
SRCS := $(TARGET).cpp
CC := g++
INCLUDES := ../../includes 
CFLAGS := -Wall -Wextra -Wno-deprecated $(foreach incdir, $(INCLUDES), -I$(incdir))
LFLAGS := 

.PHONY: clean

all: 
	$(CC) $(CFLAGS) $(LFLAGS) -o $(TARGET).out $(SRCS)

clean:
	rm $(TARGET).out
