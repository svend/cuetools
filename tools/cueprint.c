/* 
 * cueprint.c -- print cd information based on a template
 *
 * Copyright (C) 2004 Svend Sorensen
 * For license terms, see the file COPYING in this distribution.
 */

#include <stdio.h>
#include <stdlib.h>		/* exit() */
#include <string.h>		/* strcmp() */
#include <unistd.h>		/* getopt() */
#include <ctype.h>		/* isdigit() */
#include "cuefile.h"

/* default templates */
#define D_TEMPLATE "%P \"%T\" (%N tracks)\n"
#define T_TEMPLATE "%n: %p \"%t\"\n"

char *progname;
char *d_template = D_TEMPLATE;	/* disc template */
char *t_template = T_TEMPLATE;	/* track template */

void usage (int status)
{
	if (0 == status) {
		fprintf(stdout, "%s: usage: cueprint [-h] [-i cue|toc] [-d TEMPLATE] [-t TEMPLATE] [file...]\n", progname);
		fputs("\
\n\
OPTIONS\n\
-h 			print usage\n\
-i cue|toc		set format of file(s)\n\
-d TEMPLATE		set disc template (see TEMPLATE EXPANSION)\n\
-t TEMPLATE		set track template (see TEMPLATE EXPANSION)\n\
\n\
Template Expansion\n\
Disc:\n\
%A - album arranger\n\
%C - album composer\n\
%G - album genre\n\
%M - album message\n\
%N - number of tracks\n\
%P - album performer\n\
%S - album songwriter\n\
%T - album title\n\
%U - album UPC/EAN\n\
Track:\n\
%a - track arranger\n\
%c - track composer\n\
%g - track genre\n\
%i - track ISRC\n\
%m - track message\n\
%n - track number\n\
%p - track perfomer\n\
%t - track title\n\
%u - track ISRC (CD-TEXT)\n\
\n\
Any other %<character> is expanded to that character.  For example, to get a\n\
'%', use %%.\n\
", stdout);
		fprintf(stdout, "default disc template is:\n\"%s\"\n", D_TEMPLATE);
		fprintf(stdout, "default track template is:\n\"%s\"\n", T_TEMPLATE);
	} else {
		fprintf(stderr, "%s: syntax error\n", progname);
		fprintf(stderr, "run `%s -h' for usage\n", progname);
	}

	exit (status);
}

void disc_field (char *c, Cd *cd)
{
	Cdtext *cdtext = NULL;
	cdtext = cd_get_cdtext(cd);

	switch (*c) {
	case 'A':
		printf("%s", cdtext_get(PTI_ARRANGER, cdtext));
		break;
	case 'C':
		printf("%s", cdtext_get(PTI_COMPOSER, cdtext));
		break;
	case 'G':
		printf("%s", cdtext_get(PTI_GENRE, cdtext));
		break;
	case 'M':
		printf("%s", cdtext_get(PTI_MESSAGE, cdtext));
		break;
	case 'N':
		printf("%0*d", 2, cd_get_ntrack(cd));
		break;
	case 'P':
		printf("%s", cdtext_get(PTI_PERFORMER, cdtext));
		break;
	case 'R':
		printf("%s", cdtext_get(PTI_ARRANGER, cdtext));
		break;
	case 'S':
		printf("%s", cdtext_get(PTI_SONGWRITER, cdtext));
		break;
	case 'T':
		printf("%s", cdtext_get(PTI_TITLE, cdtext));
		break;
	case 'U':
		printf("%s", cdtext_get(PTI_UPC_ISRC, cdtext));
		break;
	default:
		putchar(*c);
		break;
	}
}

void track_field (char *c, Cd *cd, int trackno, int width)
{
	Track *track = NULL;
	Cdtext *cdtext = NULL;

	track = cd_get_track(cd, trackno);
	cdtext = track_get_cdtext(track);

	switch (*c) {
	case 'a':
		printf("%*s", width, cdtext_get(PTI_ARRANGER, cdtext));
		break;
	case 'c':
		printf("%*s", width, cdtext_get(PTI_COMPOSER, cdtext));
		break;
	case 'f':
		printf("%*s", width, track_get_filename(track));
		break;
	case 'g':
		printf("%*s", width, cdtext_get(PTI_GENRE, cdtext));
		break;
	case 'i':
		printf("%*s", width, track_get_isrc(track));
		break;
	case 'm':
		printf("%*s", width, cdtext_get(PTI_MESSAGE, cdtext));
		break;
	case 'n':
		printf("%0*d", width, trackno);
		break;
	case 'p':
		printf("%*s", width, cdtext_get(PTI_PERFORMER, cdtext));
		break;
	case 's':
		printf("%*s", width, cdtext_get(PTI_SONGWRITER, cdtext));
		break;
	case 't':
		printf("%*s", width, cdtext_get(PTI_TITLE, cdtext));
		break;
	case 'u':
		printf("%*s", width, cdtext_get(PTI_UPC_ISRC, cdtext));
		break;
	default:
		disc_field(c, cd);
		break;
	}
}

void print_info (Cd *cd) 
{
	int i;		/* track */
	char *c;

	/* field flags */
	int flag = 1;	/* flags remain */
	int zeropad = 0;

	/* field width */
	int width;

	for (c = d_template; '\0' != *c; c++) {
		if ('%' == *c) {
			c++;
			disc_field(c, cd);
		} else {
			putchar(*c);
		}
	}

	for (i = 1; i <= cd_get_ntrack(cd); i++) {
		for (c = t_template; '\0' != *c; c++) {
			if ('\\' == *c) {
				c++;

				switch (*c) {
				case 'n':
					putchar('\n');
					break;
				case 't':
					putchar('\t');
					break;
				default:
					putchar(*c);
					break;
				}
			} else if ('%' == *c) {
				c++;

				/* parse flags */
				do {
					switch (*c) {
					case '0':	/* zero-padding */
						c++;
						zeropad = 1;
						break;
					default:
						flag = 0;
					}
				} while (0 != flag);

				/* parse width */
				width = 0;
				while (0 != isdigit(*c)) {
					width = width * 10 + *c++ - '0';
				}

				track_field(c, cd, i, width);
			} else {
				putchar(*c);
			}
		}
	}
}

int info (char *name, int format)
{
	Cd *cd = NULL;

	if (NULL == (cd = cf_parse(name, &format))) {
		fprintf(stderr, "%s: input file error\n", name);
		return -1;
	}

	print_info(cd);

	return 0;
}

int main (int argc, char **argv)
{
	int format = UNKNOWN;
	/* getopt () variables */
	char c;
	extern char *optarg;
	extern int optind;

	progname = *argv;

	while (-1 != (c = getopt(argc, argv, "hi:d:t:"))) {
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
		case 'd':
			d_template = optarg;
			break;
		case 't':
			t_template = optarg;
			break;
		default:
			usage(1);
			break;
		}
	}

	if (optind == argc) {
		info("-", format);
	} else {
		for (; optind < argc; optind++)
			info(argv[optind], format);
	}

	return 0;
}
