SUBDIRS= lib tools man

all install uninstall:
	for dir in $(SUBDIRS) ; do \
		(cd $$dir && $(MAKE) $@) ; \
	done

# reStructuredText Makefile

RST2HTML= rst2html.py
RST2LATEX= rst2latex.py
LATEX2PDF= pdflatex

RSTFILES= $(wildcard *.txt)
HTMLFILES= $(RSTFILES:.txt=.html)
LATEXFILES= $(RSTFILES:.txt=.tex)
PDFFILES= $(RSTFILES:.txt=.pdf)

all: html pdf

html: $(HTMLFILES)
	cd man && $(MAKE) $@
latex: $(LATEXFILES)
pdf: $(PDFFILES)

%.html: %.txt
	$(RST2HTML) $< > $@

%.tex: %.txt
	$(RST2LATEX) $< > $@

%.pdf: %.tex
	$(LATEX2PDF) $<

clean:
	for dir in $(SUBDIRS) ; do \
		(cd $$dir && $(MAKE) $@) ; \
	done
	rm -f $(HTMLFILES) $(LATEXFILES) $(PDFFILES)
	rm -f *.aux *.log *.out
