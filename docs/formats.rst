============================================
Description of the Cuesheet/TOC File Formats
============================================

:Author: Svend Sorensen
:Contact: sorensen@users.berlios.de
:Date: $LastChangedDate$

.. contents::

The Cuesheet (cue) and TOC (toc) file formats both describe the layout of a CD.

Notes
=====

A Cuesheet or TOC file has an (optional) global section, followed by any number
of track sections.

Lines (with a few exceptions for the toc format) begin with a keyword, which
may be followed by whitespace separated fields.  Some keywords are only allowed
in the global section or a track section.

Format Notation
---------------

A string may be enclosed in single or double quotation marks.  If the string
contains whitespace, it must be enclosed in quotation marks.

``<field>``
	a required field
``[field]``
	an optional field
``...``
	preceding field may be repeated any number of times

Cuesheet Format
===============

The standard file suffix for a cuesheet file is ``.cue``.

Keywords
--------

CATALOG
~~~~~~~

Sets the catalog number of the CD.

:Section(s): global
:Format:
	``CATALOG <catalog_number>``

	``catalog_number``
		an integer consisting of exactly 13 digits.

FILE
~~~~

Sets a new input file.  ``FILE`` line precedes the beginning of the track
section.

:Section(s): track
:Format:
	``FILE <filename> [file_format]``

	``filename``
		a string.
	``file_format``
		one of: ``BINARY``, ``MOTOROLA``, ``AIFF``, ``WAVE``, or
		``MP3``.

FLAGS
~~~~~

Sets track flags.

:Section(s): track
:Format:
	``FLAGS [flag]...``

	``flag``
		one of: ``PRE``, ``DCP``, ``4CH``, or ``SCMS``.

INDEX
~~~~~

Sets a track index.

:Section(s): track
:Format:
	``INDEX <index_number> <index>``

	``index_number``
		an integer in the range 0-99.
	``index``
		time in MSF format.

ISRC
~~~~

Sets track ISRC number.

:Section(s): track
:Format:
	``ISRC <ISRC_number>``

	``ISRC_number``
		a string with the format CCOOOOYYSSSSS.

POSTGAP
~~~~~~~

Sets track postgap.

:Section(s): track
:Format:
	``POSTGAP <postgap>``

	``postgap``
		time in MSF format.

PREGAP
~~~~~~

Sets track pregap.

:Section(s): track
:Format:
	``PREGAP <pregap>``

	``pregap``
		time in MSF format.

REM
~~~

Begins a comment line.  All lines beginning with ``REM`` are ignored.

:Section(s): any
:Format:
	``REM [comments]``

TRACK
~~~~~

Starts a new track.

:Section(s): track
:Format:
	``TRACK <track_number> <track_mode>``

	``track_number``
		a positive integer in the range 1-99.
	``track_mode``
		one of: ``MODE1/2048``, ``MODE1/2352``, ``MODE2/2336``,
		``MODE2/2048``, ``MODE2/2324``, ``MODE2/2336``, or
		``MODE2/2352``.

(CD-TEXT keywords)
~~~~~~~~~~~~~~~~~~

Sets CD-TEXT data. The cue format does not have native support for binary
CD-TEXT.

:Section(s): any
:Format:
	``<CDTEXT_keyword> <CDTEXT_field>``

	``CDTEXT_keyword``
		one of: (see `CD-TEXT`_ section for a list of keywords).
	``CDTEXT_field``
		a string.

TOC File Format
===============

The standard file suffix for a TOC file is ``.toc``.

Certain keywords begin a block, which must be enclosed between ``{`` and ``}``
brackets.

Comments
--------

Comments begin with ``//`` and may be placed anywhere.  Because they can be
placed anywhere in a line, toc comments are treated as a special case (not as a
keyword).  Any text beginning with ``//`` is stripped from each line.

Keywords
--------

CATALOG
~~~~~~~

:Section(s): global
:Format: ``CATALOG "<catalog_number>"``

Sets the catalog number  of the CD.  ``catalog_number`` must be exactly 13
digits.

CD_DA
~~~~~

:Section(s): global
:Format: ``CD_DA``

Sets the CD format to audio.

CD_ROM
~~~~~~

:Section(s): global
:Format: ``CD_ROM``

Sets the CD format to data.

CD_ROM_XA
~~~~~~~~~

:Section(s): global
:Format: ``CD_ROM_XA``

Sets the CD format to mixed.

TRACK
~~~~~

:Section(s): track
:Format: ``TRACK <track_mode> [sub_channel_mode]``

NO
~~

:Section(s): track
:Format: ``NO <flag>``

Negates (clears) track flags.  Allowable ``flag`` entries  are ``COPY`` and
``PRE_EMPHASIS``.

COPY
~~~~

:Section(s): track
:Format: ``COPY``

Sets the copy permitted flag.

PRE_EMPHASIS
~~~~~~~~~~~~

:Section(s): track
:Format: ``PRE_EMPHASIS``

Sets the pre-emphasis flag.

TWO_CHANNEL_AUDIO
~~~~~~~~~~~~~~~~~

:Section(s): track
:Format: ``TWO_CHANNEL_AUDIO``

Sets the two channel audio flag.

FOUR_CHANNEL_AUDIO
~~~~~~~~~~~~~~~~~~

:Section(s): track
:Format: ``FOUR_CHANNEL_AUDIO``

Sets the four channel audio flag.

ISRC
~~~~

:Section(s): track
:Format: ``ISRC "<ISRC_number>"``

Sets track ISRC number.  ``ISRC_number`` must be of the format CCOOOOYYSSSSS.

SILENCE
~~~~~~~

:Section(s): track

*TODO*

ZERO
~~~~

:Section(s): track

*TODO*

FILE
~~~~

:Section(s): track
:Format: ``FILE "<filename>" <start> [length]``

Add file to current track.  ``start`` and ``length`` must be in MSF format.

AUDIOFILE
~~~~~~~~~

:Section(s): track
:Format: ``AUDIOFILE "<filename>" <start> [length]``

Add file to current track.  ``start`` and ``length`` must be in MSF format.

DATAFILE
~~~~~~~~

:Section(s): track

*TODO*

FIFO
~~~~

:Section(s): track

*TODO*

START
~~~~~

:Section(s): track
:Format: ``START <start>``

*TODO*

PREGAP
~~~~~~

Section(s): track
Format: ``PREGAP <pregap>``

Sets track pregap.  ``pregap`` must be in MSF format.

INDEX
~~~~~

:Section(s): track
:Format: ``INDEX <index_number> <index_time>``

Sets a track index.  ``index_number`` must be a non-negative integer.
``index_time`` must be in MSF format.

CD_TEXT
~~~~~~~

:Section(s): any
:Format: ``CD_TEXT {}``

Begins a CD-TEXT block.

LANGUAGE_MAP
~~~~~~~~~~~~

:Section(s): global
:Subsection(s): CD-TEXT block
:Format: ``LANGUAGE_MAP { language_map... }``

Begins a language map block.  ``language_map`` is a mapping of an integer in
the range 0-255 to a country code.

LANGUAGE
~~~~~~~~

:Section(s): track
:Subsection(s): CD-TEXT block
:Format: ``LANGUAGE language_number { cd_text }``

Begins a language block.  ``language_number`` must be an integer in the range
0-255.  ``cd_text`` is a series of CD-TEXT keyword lines.

(CD-TEXT keywords)
~~~~~~~~~~~~~~~~~~

:Section(s): any
:Subsection(s): language block
:Format1: ``<CDTEXT_keyword> "<CDTEXT_field>"``
:Format2: ``<CDTEXT_keyword> { CDTEXT_binary_data }``

See `CD-TEXT`_ section for a list of CD-TEXT keywords.  Format1 is for
character data and Format2 is for binary data.  ``CDTEXT_binary_data`` is a
comma seperated list of integers in the range 0-255.

CD-TEXT
=======

The CD-TEXT specification set aside 16 CD-TEXT types.  Three are reserved and
have no corresponding keywords.  One is shared; the keyword depends on whether
it is for the disc (``UPC_EAN``) or a track (``ISRC``).  CD-TEXT entries are
either character or binary data.

Keywords
--------

ARRANGER
~~~~~~~~

:Description: Name(s) of the arranger(s)
:Section: any
:Format: character

COMPOSER
~~~~~~~~

:Description: Name(s) of the composer(s)
:Section: any
:Format: character

DISC_ID
~~~~~~~

:Description: Disc Identification information
:Section: any
:Format: binary

GENRE
~~~~~

:Description: Genre Identification and Genre information
:Section: any
:Format: binary

ISRC
~~~~~

:Description: ISRC Code of each track
:Section: track
:Format: character

MESSAGE
~~~~~~~

:Description: Message from the content provider and/or artist
:Section: any
:Format: character

PERFORMER
~~~~~~~~~

:Description: Name(s) of the performer(s)
:Section: any
:Format: character

SONGWRITER
~~~~~~~~~~

:Description: Name(s) of the songwriter(s)
:Section: any
:Format: character

TITLE
~~~~~

:Description: Title of album name or Track Titles
:Section: any
:Format: character

TOC_INFO
~~~~~~~~~

:Description: Table of Content information
:Section: any
:Format: binary

TOC_INFO2
~~~~~~~~~

:Description: Second Table of Content information
:Section: any
:Format: binary

UPC_EAN
~~~~~~~

:Description: UPC/EAN code of the album
:Section: disc
:Format: character

SIZE_INFO
~~~~~~~~~

:Description: Size information of the Block
:Section: any
:Format: binary

Definitions
===========

MSF format
	a representation of time in the form ``mm:ss:ff``.  ``mm`` is minutes,
	``ss`` is seconds, and ``ff`` is frames.
frame
	1/74 of a second.
