CC=gcc
CFLAGS=-pthread `mysql_config --cflags --libs` -O
LDFLAGS+= -lpthread 
SOURCES=main.c network.c mcp.c thread.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mother

all:
	$(CC) main.c $(CFLAGS) -o $(EXECUTABLE)

# all: $(SOURCES) $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(LDFLAGS) main.o -o $@

# clean: 
# 	rm -rf *.o

# .c.o:
# 	$(CC) $(CFLAGS) $< -o $@