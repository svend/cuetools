# ReStructuredText Makefile

RST2HTML= rst2html.py
RST2LATEX= rst2latex.py
LATEX2PDF= pdflatex

RSTFILES= $(wildcard *.txt)
HTMLFILES= $(RSTFILES:.rst=.html)
LATEXFILES= $(RSTFILES:.rst=.tex)
PDFFILES= $(RSTFILES:.rst=.pdf)

all: html pdf

html: $(HTMLFILES)
latex: $(LATEXFILES)
pdf: $(PDFFILES)

%.html: %.txt
	$(RST2HTML) $< > $@

%.tex: %.txt
	$(RST2LATEX) $< > $@

%.pdf: %.tex
	$(LATEX2PDF) $<

clean:
	rm -f $(HTMLFILES) $(LATEXFILES) $(PDFFILES)
	rm -f *.aux *.log *.out
