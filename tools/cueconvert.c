/*
 * cueconvert.c -- convert between cue/toc formats
 *
 * Copyright (C) 2004 Svend Sorensen
 * For license terms, see the file COPYING in this distribution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cuefile.h"

char *progname;

void usage (int status)
{
	if (0 == status) {
		fprintf(stdout, "%s: usage: cueconvert [-h] [-i cue|toc] [-o cue|toc] [infile [outfile]]\n", progname);
		fputs("\
\n\
OPTIONS\n\
-h 			print usage\n\
-i cue|toc		set format of input file\n\
-o cue|toc		set format of output file\n\
", stdout);
	} else {
		fprintf(stderr, "%s: syntax error\n", progname);
		fprintf(stderr, "run `%s -h' for usage\n", progname);
	}

	exit (status);
}

int convert (char *iname, int iformat, char *oname, int oformat)
{
	Cd *cd = NULL;

	if (NULL == (cd = cf_parse(iname, &iformat))) {
		fprintf(stderr, "input file error\n");
		return -1;
	}

	if (UNKNOWN == oformat) {
		/* first use file suffix */
		if (UNKNOWN == (oformat = cf_format_from_suffix(oname))) {
			/* then use opposite of input format */
			switch(iformat) {
			case CUE:
					oformat = TOC;
					break;
			case TOC:
					oformat = CUE;
					break;
			}
		}
	}

	return cf_print(oname, &oformat, cd);
}

int main (int argc, char **argv)
{
	int iformat = UNKNOWN;
	int oformat = UNKNOWN;
	/* option variables */
	char c;
	/* getopt() variables */
	extern char *optarg;
	extern int optind;

	progname = *argv;

	while (-1 != (c = getopt(argc, argv, "hi:o:"))) {
		switch (c) {
		case 'h':
			usage(0);
			break;
		case 'i':
			if (0 == strcmp("cue", optarg))
				iformat = CUE;
			else if (0 == strcmp("toc", optarg))
				iformat = TOC;
			break;
		case 'o':
			if (0 == strcmp("cue", optarg))
				oformat = CUE;
			else if (0 == strcmp("toc", optarg))
				oformat = TOC;
			break;
		default:
			usage(1);
			break;
		}
	}
 
	if (optind == argc) {
		convert("-", iformat, "-", oformat);
	} else if (optind == argc - 1) {
		convert(argv[optind], iformat, "-", oformat);
	} else if (optind == argc - 2) {
		convert(argv[optind], iformat, argv[optind + 1], oformat);
	} else {
		usage(1);
	}

	return 0;
}
