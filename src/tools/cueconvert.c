/*
 * cueconvert.c -- convert between cue/toc formats
 *
 * Copyright (C) 2004, 2005, 2006 Svend Sorensen
 * For license terms, see the file COPYING in this distribution.
 */

#include <getopt.h>	/* getopt_long() */
#include <stdio.h>	/* fprintf(), printf(), snprintf(), stderr */
#include <stdlib.h>	/* exit() */
#include <string.h>	/* strcasecmp() */
#include "cuefile.h"

#if HAVE_CONFIG_H
#include "config.h"
#else
#define PACKAGE_STRING "cueconvert"
#endif

char *progname;

void usage (int status)
{
	if (0 == status) {
		printf("Usage: %s [option...] [infile [outfile]]\n", progname);
		printf("Convert file between the CUE and TOC formats.\n"
		       "\n"
		       "OPTIONS\n"
		       "-h, --help 			print usage\n"
		       "-i, --input-format cue|toc	set format of input file\n"
		       "-o, --output-format cue|toc	set format of output file\n"
		       "-V, --version			print version information\n");
	} else {
		fprintf(stderr, "Try `%s --help' for more information.\n", progname);
	}

	exit (status);
}

void version ()
{
	printf("%s\n", PACKAGE_STRING);

	exit(0);
}

int convert (char *iname, int iformat, char *oname, int oformat)
{
	Cd *cd = NULL;

	if (NULL == (cd = cf_parse(iname, &iformat))) {
		fprintf(stderr, "%s: error: unable to parse input file"
		        " `%s'\n", progname, iname);
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
	int ret = 0;		/* return value of convert() */

	/* option variables */
	int c;
	/* getopt_long() variables */
	extern char *optarg;
	extern int optind;

	static struct option longopts[] = {
		{"help", no_argument, NULL, 'h'},
		{"input-format", required_argument, NULL, 'i'},
		{"output-format", required_argument, NULL, 'o'},
		{"version", no_argument, NULL, 'V'},
		{NULL, 0, NULL, 0}
	};

	progname = argv[0];

	while (-1 != (c = getopt_long(argc, argv, "hi:o:V", longopts, NULL))) {
		switch (c) {
		case 'h':
			usage(0);
			break;
		case 'i':
			if (0 == strcmp("cue", optarg)) {
				iformat = CUE;
			} else if (0 == strcmp("toc", optarg)) {
				iformat = TOC;
			} else {
				fprintf(stderr, "%s: error: unknown input file"
				        " format `%s'\n", progname, optarg);
				usage(1);
			}
			break;
		case 'o':
			if (0 == strcmp("cue", optarg)) {
				oformat = CUE;
			} else if (0 == strcmp("toc", optarg)) {
				oformat = TOC;
			} else {
				fprintf(stderr, "%s: error: unknown output file"
				        " format `%s'\n", progname, optarg);
				usage(1);
			}
			break;
		case 'V':
			version();
			break;
		default:
			usage(1);
			break;
		}
	}

	/* What we do depends on the number of operands. */
	if (optind == argc) {
		/* No operands: report breakpoints of stdin. */
		ret = convert("-", iformat, "-", oformat);
	} else if (optind == argc - 1) {
		/* One operand: convert operand file to stdout. */
		ret = convert(argv[optind], iformat, "-", oformat);
	} else if (optind == argc - 2) {
		/* Two operands: convert input file to output file. */
		ret = convert(argv[optind], iformat, argv[optind + 1], oformat);
	} else {
		usage(1);
	}

	return ret;
}
