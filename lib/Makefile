YFLAGS+= -d
LEX=flex

TARGETS= libcuefile.a
OBJS= cd.o cdtext.o time.o cuefile.o
CUE_OBJS= cue_parse.o cue_scan.o cue_print.o
TOC_OBJS= toc_parse.o toc_scan.o toc_print.o

# There are multiple scanners/parsers
# so we need a prefix to aviod conflicts.
# (NOTE: these lines require GNU Make)

toc_scan.c: LFLAGS+= -Ptoc_yy
cue_scan.c: LFLAGS+= -Pcue_yy
toc_parse.c: YFLAGS+= -p toc_yy
cue_parse.c: YFLAGS+= -p cue_yy

all: $(TARGETS)

libcuefile.a: libcuefile.a($(OBJS) $(CUE_OBJS) $(TOC_OBJS))

%.c: %.y
	$(YACC) -o $@ $(YFLAGS) $<

install: all

uninstall:

clean:
	rm -f $(TARGETS)
	rm -f *.o *.a *.core *.output
	rm -f toc_scan.c toc_parse.c toc_parse.h
	rm -f cue_scan.c cue_parse.c cue_parse.h

# dependancies
# generated with `gcc -MM *.c'
# after building lex and yacc .c files

cd.o: cd.c cd.h cdtext.h
cdtext.o: cdtext.c cdtext.h
cuefile.o: cuefile.c cuefile.h cd.h cdtext.h cue.h toc.h
cue_parse.o: cue_parse.c cd.h cdtext.h time.h
cue_print.o: cue_print.c cd.h cdtext.h time.h
cue_scan.o: cue_scan.c cd.h cdtext.h cue_parse.h
time.o: time.c
toc_parse.o: toc_parse.c cd.h cdtext.h time.h
toc_print.o: toc_print.c cd.h cdtext.h time.h
toc_scan.o: toc_scan.c cd.h cdtext.h toc_parse.h
