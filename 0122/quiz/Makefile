T1 := aries

CC := g++
CFLAGS := -g -c -Wall -O0 -std=c++11
LDFLAGS := -lm -lpthread 

SRCS1 := main.cc 
OBJS1 := $(SRCS1:%.cc=%.o)

all: $(T1) 

$(T1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.exe *.stackdump $(T1) $(OBJS1) $(DEPS1)  
