CC = g++
CFLAGS = -O3 -Wall 
FLAGS += -D_LARGEFILE_SOURCE
CFLAGS += -fno-exceptions
CFLAGS += -finline-functions
CFLAGS += -funroll-loops
FLAGS += -D_FILE_OFFSET_BITS=64
 
# Source files
WRITE_BLOCKS_SRC=write_blocks_seq.c
WRITE_LINES_SRC=write_lines.c
READ_BLOCKS_SRC = read_blocks_seq.c
READ_RAM_SRC = read_ram_seq.c

READ_BLOCKS_RAND = read_blocks_rand.c
READ_RAM_RAND = read_ram_rand.c

WRITE_BLOCKS_RAND = write_blocks_rand.c
WRITE_RAM_RAND = write_ram_rand.c
 
# Binaries
all: write_blocks_seq write_lines read_blocks_seq read_ram_seq read_blocks_rand read_ram_rand write_blocks_rand write_ram_rand

 
#sequential writing in blocks
write_blocks_seq: $(WRITE_BLOCKS_SRC)
	$(CC) $(CFLAGOPT) $^ -o $@ 
write_lines: $(WRITE_LINES_SRC)
	$(CC) $(CFLAGOPT) $^ -o $@ 
read_blocks_seq: $(READ_BLOCKS_SRC)
	$(CC) $(CFLAGOPT) $^ -o $@ 
read_ram_seq: $(READ_RAM_SRC)
	$(CC) $(CFLAGOPT) $^ -o $@ 
read_blocks_rand: $(READ_BLOCKS_RAND)
	$(CC) $(CFLAGOPT) $^ -o $@
read_ram_rand: $(READ_RAM_RAND)
	$(CC) $(CFLAGOPT) $^ -o $@ 
write_blocks_rand: $(WRITE_BLOCKS_RAND)
	$(CC) $(CFLAGOPT) $^ -o $@ 
write_ram_rand: $(WRITE_RAM_RAND)
	$(CC) $(CFLAGOPT) $^ -o $@ 

clean:  

	rm write_blocks_seq write_lines read_blocks_seq read_ram_seq read_blocks_rand read_ram_rand write_blocks_rand write_ram_rand

  