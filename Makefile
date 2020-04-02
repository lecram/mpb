CFLAGS=-std=c99 -Wall -Wextra

all: mpb

.PHONE: clean
clean:
	$(RM) mpb
