%{
/*
 * toc_scan.l -- lexer for toc files
 *
 * Copyright (C) 2004 Svend Sorensen
 * For license terms, see the file COPYING in this distribution.
 */

#include <stdlib.h>
#include <string.h>
#include "cd.h"
#include "toc_parse.h"

int toc_lineno = 1;
%}

ws		[ \t\r]
nonws		[^ \t\r\n]

%option noyywrap

%s NAME

%%

\'([^\']|\\\')*\'	|
\"([^\"]|\\\")*\"	{
		toc_yylval.sval = strdup(yytext + 1);
		toc_yylval.sval[strlen(toc_yylval.sval) - 1] = '\0';
		BEGIN(INITIAL);
		return STRING;
		}

<NAME>{nonws}+	{
		toc_yylval.sval = strdup(yytext);
		BEGIN(INITIAL);
		return STRING;
		}

CATALOG		{ BEGIN(NAME); return CATALOG; }

CD_DA		{ toc_yylval.ival = MODE_CD_DA; return CD_DA; }
CD_ROM		{ toc_yylval.ival = MODE_CD_ROM; return CD_ROM; }
CD_ROM_XA	{ toc_yylval.ival = MODE_CD_ROM_XA; return CD_ROM_XA; }

TRACK		{ return TRACK; }
AUDIO		{ toc_yylval.ival = MODE_AUDIO; return AUDIO; }
MODE1		{ toc_yylval.ival = MODE_MODE1; return MODE1; }
MODE1_RAW	{ toc_yylval.ival = MODE_MODE1_RAW; return MODE1_RAW; }
MODE2		{ toc_yylval.ival = MODE_MODE2; return MODE2; }
MODE2_FORM1	{ toc_yylval.ival = MODE_MODE2_FORM1; return MODE2_FORM1; }
MODE2_FORM2	{ toc_yylval.ival = MODE_MODE2_FORM2; return MODE2_FORM2; }
MODE2_FORM_MIX	{ toc_yylval.ival = MODE_MODE2_FORM_MIX; return MODE2_FORM_MIX; }
MODE2_RAW	{ toc_yylval.ival = MODE_MODE2_RAW; return MODE2_RAW; }
RW		{ toc_yylval.ival = SUB_MODE_RW; return RW; }
RW_RAW		{ toc_yylval.ival = SUB_MODE_RW_RAW; return RW_RAW; }

NO		{ return NO; }
COPY		{ toc_yylval.ival = FLAG_PRE_EMPHASIS; return COPY; }
PRE_EMPHASIS	{ toc_yylval.ival = FLAG_COPY_PERMITTED; return PRE_EMPHASIS; }
FOUR_CHANNEL_AUDIO	{ toc_yylval.ival = FLAG_FOUR_CHANNEL; return FOUR_CHANNEL_AUDIO; }
TWO_CHANNEL_AUDIO	{ toc_yylval.ival = FLAG_FOUR_CHANNEL; return TWO_CHANNEL_AUDIO; }

		/* ISRC is with CD-TEXT items */

SILENCE		{ return SILENCE; }
ZERO		{ return ZERO; }
(AUDIO)?FILE	{ BEGIN(NAME); return AUDIOFILE; }
DATAFILE	{ BEGIN(NAME); return DATAFILE; }
FIFO		{ BEGIN(NAME); return FIFO; }
START		{ return START; }
PREGAP		{ return PREGAP; }
INDEX		{ return INDEX; }

CD_TEXT		{ return CD_TEXT; }
LANGUAGE_MAP	{ return LANGUAGE_MAP; }
LANGUAGE	{ return LANGUAGE; }

TITLE		{ BEGIN(NAME); toc_yylval.ival = PTI_TITLE;  return TITLE; }
PERFORMER	{ BEGIN(NAME); toc_yylval.ival = PTI_PERFORMER;  return PERFORMER; }
SONGWRITER	{ BEGIN(NAME); toc_yylval.ival = PTI_SONGWRITER;  return SONGWRITER; }
COMPOSER	{ BEGIN(NAME); toc_yylval.ival = PTI_COMPOSER;  return COMPOSER; }
ARRANGER	{ BEGIN(NAME); toc_yylval.ival = PTI_ARRANGER;  return ARRANGER; }
MESSAGE		{ BEGIN(NAME); toc_yylval.ival = PTI_MESSAGE;  return MESSAGE; }
DISC_ID		{ BEGIN(NAME); toc_yylval.ival = PTI_DISC_ID;  return DISC_ID; }
GENRE		{ BEGIN(NAME); toc_yylval.ival = PTI_GENRE;  return GENRE; }
TOC_INFO1	{ BEGIN(NAME); toc_yylval.ival = PTI_TOC_INFO1;  return TOC_INFO1; }
TOC_INFO2	{ BEGIN(NAME); toc_yylval.ival = PTI_TOC_INFO2;  return TOC_INFO2; }
UPC_EAN		{ BEGIN(NAME); toc_yylval.ival = PTI_UPC_ISRC;  return UPC_EAN; }
ISRC		{ BEGIN(NAME); toc_yylval.ival = PTI_UPC_ISRC;  return ISRC; }
SIZE_INFO	{ BEGIN(NAME); toc_yylval.ival = PTI_SIZE_INFO;  return SIZE_INFO; }

"//".*\n	{ toc_lineno++; /* ignore comments */ }
{ws}+		{ /* ignore whitespace */ }

[[:digit:]]+	{ toc_yylval.ival = atoi(yytext); return NUMBER; }
:|,|\{|\}	{ return yytext[0]; }

^{ws}*\n	{ toc_lineno++; /* blank line */ }
\n		{ toc_lineno++; return '\n'; }
.		{ fprintf(stderr, "bad character '%c'\n", yytext[0]); }

%%
