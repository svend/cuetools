SUBDIRS= lib tools man

all install uninstall clean:
	for dir in $(SUBDIRS) ; do \
		(cd $$dir && $(MAKE) $@) ; \
	done
