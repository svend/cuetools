/*
 * cuebreakpoints.c -- print track break points
 *
 * Copyright (C) 2004 Svend Sorensen
 * For license terms, see the file COPYING in this distribution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "cuefile.h"
#include "time.h"

char *progname;

void usage (int status)
{
	if (0 == status) {
		fprintf(stdout, "%s: usage: cuebreakpoints [option...] [file...]\n", progname);
		fputs("\
\n\
OPTIONS\n\
-h, --help			print usage\n\
-i, --input-format cue|toc	set format of file(s)\n\
", stdout);
	} else {
		fprintf(stderr, "run `%s --help' for usage\n", progname);
	}

	exit (status);
}

void print_m_ss_ff (long frame)
{
	int m, s, f;

	time_frame_to_msf(frame, &m, &s, &f);
	printf ("%d:%02d.%02d\n", m, s, f);
}

void print_breaks (Cd *cd)
{
	int i;
	long b;
	Track *track;

	/* start on track 2 */
	for (i = 2; i <= cd_get_ntrack(cd); i++) {
		track = cd_get_track(cd, i);
		/* breakpoint is at index 1 */
		/* TODO: make option for breakpoint at index 0,
		 * and option for breakpoints a index 0 and 1
		 */
		b = track_get_start(track) + track_get_index(track, 1) -  track_get_zero_pre(track);
		/* don't print zero indexes */
		if (0 != b)
			print_m_ss_ff(b);
	}
}

int breaks (char *name, int format)
{
	Cd *cd = NULL;

	if (NULL == (cd = cf_parse(name, &format))) {
		fprintf(stderr, "%s: input file error\n", name);
		return -1;
	}

	print_breaks(cd);

	return 0;
}

int main (int argc, char **argv)
{
	int format = UNKNOWN;

	/* option variables */
	char c;
	/* getopt_long() variables */
	extern char *optarg;
	extern int optind;

	static struct option longopts[] = {
		{"help", no_argument, NULL, 'h'},
		{"input-format", required_argument, NULL, 'i'},
		{NULL, 0, NULL, 0}
	};

	progname = *argv;

	while (-1 != (c = getopt_long(argc, argv, "hi:", longopts, NULL))) {
		switch (c) {
		case 'h':
			usage(0);
			break;
		case 'i':
			if (0 == strcmp("cue", optarg))
				format = CUE;
			else if (0 == strcmp("toc", optarg))
				format = TOC;
			else
				fprintf(stderr, "%s: illegal format `%s'\n", progname, optarg);
				usage(1);
			break;
		default:
			usage(1);
			break;
		}
	}

	if (optind == argc) {
		breaks("-", format);
	} else {
		for (; optind < argc; optind++)
			breaks(argv[optind], format);
	}

	return 0;
}
