all: write_blocks_seq.c write_blocks_seq.h
	gcc -g -Wall -o write_blocks_seq write_blocks_seq.c

clean: 
	$(RM) write_blocks_seq