CC = gcc
CFLAGS = -g
CFILES = tokenizer.c

all: tokenizer

tokenizer : $(CFILES)
	$(CC) $(CFLAGS) -o tokenizer $(CFILES)

clean:
	rm -rf *.o tokenizer