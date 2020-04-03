PREFIX=/usr/local
MANPREFIX=$(PREFIX)/man
BINDIR=$(DESTDIR)$(PREFIX)/bin
MANDIR=$(DESTDIR)$(MANPREFIX)/man1
CFLAGS=-std=c99 -Wall -Wextra

all: mpb

install: mpb
	rm -f $(BINDIR)/mpb
	mkdir -p $(BINDIR)
	cp mpb $(BINDIR)/mpb
	mkdir -p $(MANDIR)
	cp mpb.1 $(MANDIR)/mpb.1

uninstall:
	rm -f $(BINDIR)/mpb
	rm -f $(MANDIR)/mpb.1

clean:
	rm -f mpb
