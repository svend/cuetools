# Makefile

INSTALL= install
INSTALL_PROGRAM= $(INSTALL) -c
INSTALL_DATA= $(INSTALL) -m 644
GROFF= groff

prefix= /usr/local
mandir= $(prefix)/man
DESTDIR=

MANFILES= cuebreakpoints.man cueconvert.man cueprint.man
HTMLFILES= $(MANFILES:.man=.html)

html: $(HTMLFILES)

all:
	@true

install: all
	$(INSTALL) -d $(DESTDIR)$(mandir)/man1
	for i in $(MANFILES) ; do \
		$(INSTALL_DATA) $$i $(DESTDIR)$(mandir)/man1/`basename $$i .man`.1 ; \
	done

uninstall:
	-for i in $(MANFILES) ; do \
		rm -f $(DESTDIR)$(mandir)/man1/`basename $$i .man`.1 ; \
	done

clean:
	rm -f *.html

%.html: %.man
	${GROFF} -man -T html $< > $@
