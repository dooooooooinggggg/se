T1 = alice-bob
T2 = alice-to-bob

CC = g++
CFLAGS = -c -Wall -g -DDEBUG -O0
LDFLAGS = -Wall -lm -lpthread -O0

OBJ_T1 = alice-bob.o
OBJ_T2 = alice-to-bob.o


#
# Rules for make
#
all: $(T1) $(T2) 

$(T1): $(OBJ_T1) 
	$(CC) -o $@ $^ $(LDFLAGS)

$(T2): $(OBJ_T2) 
	$(CC) -o $@ $^ $(LDFLAGS)

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o  *.stackdump $(T1) $(T2) 
