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

#define D_TEMPLATE "\
Disc Information\n\
arranger:	%A\n\
composer:	%C\n\
genre:		%G\n\
message:	%M\n\
no. of tracks:	%N\n\
performer:	%P\n\
songwriter:	%S\n\
title:		%T\n\
UPC/EAN:	%U\n\
"

#define T_TEMPLATE "\
\n\
Track %n Information\n\
arranger:	%a\n\
composer:	%c\n\
genre:		%g\n\
ISRC:		%i\n\
message:	%m\n\
track number:	%n\n\
perfomer:	%p\n\
title:		%t\n\
ISRC (CD-TEXT):	%u\n\
"

char *progname;

void usage (int status)
{
	if (0 == status) {
		fprintf(stdout, "%s: usage: cueprint [-h] [-i cue|toc] [-n TRACKNUMBER] [-d TEMPLATE] [-t TEMPLATE] [file...]\n", progname);
		fputs("\
\n\
OPTIONS\n\
-h 			print usage\n\
-i cue|toc		set format of file(s)\n\
-n TRACKNUMBER		only print track information for track TRACKNUMBER\n\
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
		fprintf(stdout, "default disc template is:\n%s\n", D_TEMPLATE);
		fprintf(stdout, "default track template is:\n%s\n", T_TEMPLATE);
	} else {
		fprintf(stderr, "%s: syntax error\n", progname);
		fprintf(stderr, "run `%s -h' for usage\n", progname);
	}

	exit (status);
}

void disc_field (char *conv, int length, Cd *cd)
{
	char *c;	/* pointer to conversion character */

	Cdtext *cdtext = NULL;
	cdtext = cd_get_cdtext(cd);

	c = conv + length - 1;

	switch (*c) {
	case 'A':
		*c = 's';
		printf(conv, cdtext_get(PTI_ARRANGER, cdtext));
		break;
	case 'C':
		*c = 's';
		printf(conv, cdtext_get(PTI_COMPOSER, cdtext));
		break;
	case 'G':
		*c = 's';
		printf(conv, cdtext_get(PTI_GENRE, cdtext));
		break;
	case 'M':
		*c = 's';
		printf(conv, cdtext_get(PTI_MESSAGE, cdtext));
		break;
	case 'N':
		*c = 'd';	/* tracks is an integer */
		printf(conv, cd_get_ntrack(cd));
		break;
	case 'P':
		*c = 's';
		printf(conv, cdtext_get(PTI_PERFORMER, cdtext));
		break;
	case 'R':
		*c = 's';
		printf(conv, cdtext_get(PTI_ARRANGER, cdtext));
		break;
	case 'S':
		*c = 's';
		printf(conv, cdtext_get(PTI_SONGWRITER, cdtext));
		break;
	case 'T':
		*c = 's';
		printf(conv, cdtext_get(PTI_TITLE, cdtext));
		break;
	case 'U':
		*c = 's';
		printf(conv, cdtext_get(PTI_UPC_ISRC, cdtext));
		break;
	default:
		putchar(*c);
		break;
	}
}

void track_field (char *conv, int length, Cd *cd, int trackno)
{
	char *c;	/* pointer to conversion character */

	Track *track = NULL;
	Cdtext *cdtext = NULL;

	track = cd_get_track(cd, trackno);
	cdtext = track_get_cdtext(track);

	c = conv + length - 1;

	switch (*c) {
	case 'a':
		*c = 's';
		printf(conv, cdtext_get(PTI_ARRANGER, cdtext));
		break;
	case 'c':
		*c = 's';
		printf(conv, cdtext_get(PTI_COMPOSER, cdtext));
		break;
	case 'f':
		*c = 's';
		printf(conv, track_get_filename(track));
		break;
	case 'g':
		*c = 's';
		printf(conv, cdtext_get(PTI_GENRE, cdtext));
		break;
	case 'i':
		*c = 's';
		printf(conv, track_get_isrc(track));
		break;
	case 'm':
		*c = 's';
		printf(conv, cdtext_get(PTI_MESSAGE, cdtext));
		break;
	case 'n':
		*c = 'd';	/* track number is an integer */
		printf(conv, trackno);
		break;
	case 'p':
		*c = 's';
		printf(conv, cdtext_get(PTI_PERFORMER, cdtext));
		break;
	case 's':
		*c = 's';
		printf(conv, cdtext_get(PTI_SONGWRITER, cdtext));
		break;
	case 't':
		*c = 's';
		printf(conv, cdtext_get(PTI_TITLE, cdtext));
		break;
	case 'u':
		*c = 's';
		printf(conv, cdtext_get(PTI_UPC_ISRC, cdtext));
		break;
	default:
		disc_field(conv, length, cd);
		break;
	}
}

/* print a % conversion specification
 * %[flag(s)][width][.precision]<conversion-char>
 */
void print_conv (char *start, int length, Cd *cd, int trackno)
{
	char *conv;	/* copy of conversion specification */

	/* TODO: use strndup? */
	conv = malloc ((unsigned) (length + 1));
	strncpy(conv, start, length);
	conv[length] = '\0';

	/* conversion character */
	if (0 == trackno)
		disc_field(conv, length, cd);
	else
		track_field(conv, length, cd, trackno);

	free(conv);
}

/* print an single-character escape
 * `c' is the character after the `/'
 * NOTE: this does not handle octal and hexidecimal escapes
 *       except for \0
 */
void print_esc (char *c)
{
	switch (*c) {
		case 'a':
			putchar('\a');
			break;
		case 'b':
			putchar('\b');
			break;
		case 'f':
			putchar('\f');
			break;
		case 'n':
			putchar('\n');
			break;
		case 'r':
			putchar('\r');
			break;
		case 't':
			putchar('\t');
			break;
		case 'v':
			putchar('\v');
			break;
		case '0':
			putchar('\0');
			break;
		default:
			/* ?, ', " are handled by the default */
			putchar(*c);
			break;
	}
}

void cd_printf (char *format, Cd *cd, int trackno)
{
	char *c;	/* pointer into format */
	char *conv_start;
	int conv_length;

	for (c = format; '\0' != *c; c++) {
		switch (*c) {
		case '%':
			conv_start = c;
			conv_length = 1;
			c++;

			/* flags */
			while ( \
				'-' == *c \
				|| '+' == *c \
				|| ' ' == *c \
				|| '0' == *c \
				|| '#' == *c \
			) {
				conv_length++;
				c++;
			}

			/* field width */
			/* '*' not recognized */
			while (0 != isdigit(*c)) {
				conv_length++;
				c++;
			}
			
			/* precision */
			/* '*' not recognized */
			if ('.' == *c) {
				conv_length++;
				c++;

				while (0 != isdigit(*c)) {
					conv_length++;
					c++;
				}
			}

			/* length modifier (h, l, or L) */
			/* not recognized */

			/* conversion character */
			conv_length++;

			print_conv(conv_start, conv_length, cd, trackno);
			break;
		case '\\':
			c++;

			print_esc(c);
			break;
		default:
			putchar(*c);
			break;
		}
	}
}

int info (char *name, int format, int trackno, char *d_template, char *t_template)
{
	Cd *cd = NULL;
	int ntrack;

	if (NULL == (cd = cf_parse(name, &format))) {
		fprintf(stderr, "%s: input file error\n", name);
		return -1;
	}

	ntrack = cd_get_ntrack(cd);

	if (-1 == trackno) {
		cd_printf(d_template, cd, 0);

		for (trackno = 1; trackno <= ntrack; trackno++) {
			cd_printf(t_template, cd, trackno);
		}
	} else if (0 == trackno) {
		cd_printf(d_template, cd, trackno);
	} else if (0 < trackno && ntrack >= trackno) {
		cd_printf(t_template, cd, trackno);
	} else {
		fprintf(stderr, "%s: track number out of range\n", progname);
		return -1;
	}

	return 0;
}

int main (int argc, char **argv)
{
	int format = UNKNOWN;
	int trackno = -1;		/* track number (-1 = unspecified, 0 = disc info) */
	char *d_template = NULL;	/* disc template */
	char *t_template = NULL;	/* track template */
	/* getopt () variables */
	char c;
	extern char *optarg;
	extern int optind;

	progname = *argv;

	while (-1 != (c = getopt(argc, argv, "hi:n:d:t:"))) {
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
		/* TODO add track selection */
		case 'n':
			trackno = atoi(optarg);
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

	/* if no disc or track template is set, use the defaults for both */
	if (NULL == d_template && NULL == t_template) {
		d_template = D_TEMPLATE;
		t_template = T_TEMPLATE;
	} else {
		if (NULL == d_template)
			d_template = "";

		if (NULL == t_template)
			t_template = "";
	}

	if (optind == argc) {
		info("-", format, trackno, d_template, t_template);
	} else {
		for (; optind < argc; optind++)
			info(argv[optind], format, trackno, d_template, t_template);
	}

	return 0;
}
