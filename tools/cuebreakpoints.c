/*
 * cuebreakpoints.c -- print track break points
 *
 * Copyright (C) 2004 Svend Sorensen
 * For license terms, see the file COPYING in this distribution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cuefile.h"
#include "time.h"

char *progname;

void usage (int status)
{
	if (0 == status) {
		fprintf(stdout, "%s: usage: cuebreakpoints [-h] [-i cue|toc] [file...]\n", progname);
		fputs("\
\n\
OPTIONS\n\
-h 			print usage\n\
-i cue|toc		set format of file(s)\n\
", stdout);
	} else {
		fprintf(stderr, "%s: syntax error\n", progname);
		fprintf(stderr, "run `%s -h' for usage\n", progname);
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

	for (i = 1; i <= cd_get_ntrack(cd); i++) {
		track = cd_get_track(cd, i);
		/* don't print zero indexes */
		b = track_get_start(track) + track_get_index(track, 1) -  track_get_zero_pre(track);
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
	/* getopt() variables */
	extern char *optarg;
	extern int optind;

	progname = *argv;

	while (-1 != (c = getopt(argc, argv, "hi:"))) {	
		switch (c) {
		case 'h':
			usage(0);
			break;
		case 'i':
			if (0 == strcmp("cue", optarg))
				format = CUE;
			else if (0 == strcmp("toc", optarg))
				format = TOC;
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
