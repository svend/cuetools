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

#define UNSPECIFIED -1

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
		*c = 'd';
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
		*c = 'd';
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
}

/* print an escaped character
 * `c' is the character after the `/'
 * NOTE: this does not handle octal and hexidecimal escapes
 */
int print_esc (char *c)
{
	/* ?, ', " are handled by the default */
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

void print_info (Cd *cd)
{
	int i;		/* track */
	char *c;

	cd_printf(d_template, cd, 0);

	for (i = 1; i <= cd_get_ntrack(cd); i++) {
		cd_printf(t_template, cd, i);
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
