# Makefile

INSTALL= install
INSTALL_PROGRAM= $(INSTALL)
INSTALL_DATA= $(INSTALL) -m 644

prefix= /usr/local
mandir= $(prefix)/man
DESTDIR=

TARGETS= cuebreakpoints.1 cueconvert.1 cueprint.1

all:
	@true

install: all
	$(INSTALL) -d $(DESTDIR)$(mandir)/man1
	for i in $(TARGETS) ; do \
		$(INSTALL_DATA) $$i $(DESTDIR)$(mandir)/man1/$$i ; \
	done

uninstall:
	-for i in $(TARGETS) ; do \
		rm -f $(DESTDIR)$(mandir)/man1/$$i ; \
	done

clean:
	@true
