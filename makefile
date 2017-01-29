CC = gcc
CFLAGS = -O3 -Wall 
CFLAGS += -D_LARGEFILE_SOURCE
CFLAGS += -fno-exceptions
CFLAGS += -finline-functions
CFLAGS += -funroll-loops
CFLAGS += -D_FILE_OFFSET_BITS=64
 
# Source files
WRITE_BLOCKS_SRC=write_blocks_seq.c
WRITE_LINES_SRC=write_lines.c
READ_BLOCKS_SRC = read_blocks_seq.c
READ_RAM_SRC = read_ram_seq.c
READ_BLOCKS_RAND = read_blocks_rand.c
READ_RAM_RAND = read_ram_rand.c
 
# Binaries
all: write_blocks_seq write_lines read_blocks_seq read_ram_seq read_blocks_rand read_ram_rand
 
#sequential writing in blocks
write_blocks_seq: $(WRITE_BLOCKS_SRC)
	$(CC) $(CFLAGS) $^ -o $@ 
write_lines: $(WRITE_LINES_SRC)
	$(CC) $(CFLAGS) $^ -o $@ 
read_blocks_seq: $(READ_BLOCKS_SRC)
	$(CC) $(CFLAGS) $^ -o $@ 
read_ram_seq: $(READ_RAM_SRC)
	$(CC) $(CFLAGS) $^ -o $@ 
read_blocks_rand: $(READ_BLOCKS_RAND)
	$(CC) $(CFLAGS) $^ -o $@
read_ram_rand: $(READ_RAM_RAND)
	$(CC) $(CFLAGS) $^ -o $@ 
 

clean:  
	rm write_blocks_seq write_lines read_blocks_seq read_ram_seq read_blocks_rand read_ram_rand
  