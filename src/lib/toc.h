/*
 * toc.h -- toc function declarations
 *
 * Copyright (C) 2004, 2005 Svend Sorensen
 * For license terms, see the file COPYING in this distribution.
 */

Cd *toc_parse (FILE *fp);
void toc_print (FILE *fp, Cd *cd);
