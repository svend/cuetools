===============
cuetools README
===============

:Author: Svend Sorensen
:Contact: sorensen@users.berlios.de
:Website: http://cuetools.berlios.de

Description
-----------

cuetools is a set of utilities for working with Cue Sheet (cue) and Table of
Contents (toc) files.

It includes:

cueconvert
	convert between the cue and toc formats

cuebreakpoints
	print the breakpoints from a cue or toc file

cueprint
	print disc and track infomation for a cue or toc file


Installation
------------

Building requires GNU Make, Lex, and Yacc.  GNU Bison works in Yacc mode
(``make YACC="bison -y"``).

Run ``make install`` to install cuetools into the default location of
/usr/local.  The Makefiles also support both the prefix and DESTDIR variables.
