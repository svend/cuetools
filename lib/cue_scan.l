%{
/*
 * cue_scan.l -- lexer for cue files
 *
 * Copyright (C) 2004 Svend Sorensen
 * For license terms, see the file COPYING in this distribution.
 */

#include <stdlib.h>
#include <string.h>
#include "cd.h"
#include "cue_parse.h"

int cue_lineno = 1;
%}

ws		[ \t\r]
nonws		[^ \t\r\n]

%option noyywrap

%s NAME

%%

\"(\\.|[^\\"])*\"	{
		cue_yylval.sval = strdup(yytext + 1);
		cue_yylval.sval[strlen(cue_yylval.sval) - 1] = '\0';
		BEGIN(INITIAL);
		return STRING;
		}

<NAME>{nonws}+	{
		cue_yylval.sval = strdup(yytext);
		BEGIN(INITIAL);
		return STRING;
		}

CATALOG		{ BEGIN(NAME); return CATALOG; }

FILE		{ BEGIN(NAME); return FFILE; }
BINARY		{ return BINARY; }
MOTOROLA	{ return MOTOROLA; }
AIFF		{ return AIFF; }
WAVE		{ return WAVE; }
MP3		{ return MP3; }

TRACK		{ return TRACK; }
AUDIO		{ cue_yylval.ival = MODE_AUDIO; return AUDIO; }
MODE1\/2048	{ cue_yylval.ival = MODE_MODE1; return MODE1_2048; }
MODE1\/2352	{ cue_yylval.ival = MODE_MODE1_RAW; return MODE1_2352; }
MODE2\/2336	{ cue_yylval.ival = MODE_MODE2; return MODE2_2336; }
MODE2\/2048	{ cue_yylval.ival = MODE_MODE2_FORM1; return MODE2_2048; }
MODE2\/2342	{ cue_yylval.ival = MODE_MODE2_FORM2; return MODE2_2342; }
MODE2\/2332	{ cue_yylval.ival = MODE_MODE2_FORM_MIX; return MODE2_2332; }
MODE2\/2352	{ cue_yylval.ival = MODE_MODE2_RAW; return MODE2_2352; }

FLAGS		{ return FLAGS; }
PRE		{ cue_yylval.ival = FLAG_PRE_EMPHASIS; return PRE; }
DCP		{ cue_yylval.ival = FLAG_COPY_PERMITTED; return DCP; }
4CH		{ cue_yylval.ival = FLAG_FOUR_CHANNEL; return FOUR_CH; }
SCMS		{ cue_yylval.ival = FLAG_SCMS; return SCMS; }

PREGAP		{ return PREGAP; }
INDEX		{ return INDEX; }
POSTGAP		{ return POSTGAP; }

TITLE		{ BEGIN(NAME); cue_yylval.ival = PTI_TITLE;  return TITLE; }
PERFORMER	{ BEGIN(NAME); cue_yylval.ival = PTI_PERFORMER;  return PERFORMER; }
SONGWRITER	{ BEGIN(NAME); cue_yylval.ival = PTI_SONGWRITER;  return SONGWRITER; }
COMPOSER	{ BEGIN(NAME); cue_yylval.ival = PTI_COMPOSER;  return COMPOSER; }
ARRANGER	{ BEGIN(NAME); cue_yylval.ival = PTI_ARRANGER;  return ARRANGER; }
MESSAGE		{ BEGIN(NAME); cue_yylval.ival = PTI_MESSAGE;  return MESSAGE; }
DISC_ID		{ BEGIN(NAME); cue_yylval.ival = PTI_DISC_ID;  return DISC_ID; }
GENRE		{ BEGIN(NAME); cue_yylval.ival = PTI_GENRE;  return GENRE; }
TOC_INFO1	{ BEGIN(NAME); cue_yylval.ival = PTI_TOC_INFO1;  return TOC_INFO1; }
TOC_INFO2	{ BEGIN(NAME); cue_yylval.ival = PTI_TOC_INFO2;  return TOC_INFO2; }
UPC_EAN		{ BEGIN(NAME); cue_yylval.ival = PTI_UPC_ISRC;  return UPC_EAN; }
ISRC/{ws}+\"	{ BEGIN(NAME); cue_yylval.ival = PTI_UPC_ISRC;  return ISRC; }
SIZE_INFO	{ BEGIN(NAME); cue_yylval.ival = PTI_SIZE_INFO;  return SIZE_INFO; }

ISRC		{ BEGIN(NAME); return TRACK_ISRC; }

^{ws}*REM.*\n	{ cue_lineno++; /* ignore comments */ }
{ws}+		{ /* ignore whitespace */ }

[[:digit:]]+	{ cue_yylval.ival = atoi(yytext); return NUMBER; }
:		{ return yytext[0]; }

^{ws}*\n	{ cue_lineno++; /* blank line */ }
\n		{ cue_lineno++; return '\n'; }
.		{ fprintf(stderr, "bad character '%c'\n", yytext[0]); }

%%
