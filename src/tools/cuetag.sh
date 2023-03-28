#!/bin/sh

# cuetag.sh - tag files based on cue/toc file information
# uses cueprint output
# usage: cuetag.sh <cuefile|tocfile> [file]...

CUEPRINT=cueprint
cue_file=''

# makes text lowercase ; simplifies file format check syntax
lowercase(){ echo "$*" | tr '[:upper:]' '[:lower:]';}

# print usage instructions
usage()
{
	echo 'usage: cuetag.sh <cuefile|tocfile> [file]...'
	echo
	echo 'cuetag.sh adds tags to files based on cue or toc information'
	echo
	echo 'Supported formats (format extension, format name, tagging utility):'
	echo 'ogg, Ogg Vorbis, vorbiscomment'
	echo 'flac, FLAC, metaflac'
	echo 'mp3, MP3, mp3info'
	echo 'txt, Vorbis Comment Text File, tee'
	echo
	echo 'cuetag.sh uses cueprint, which must be in your path'
}

# Vorbis Comments
# for FLAC and Ogg Vorbis files
vorbis()
{
	trackno="$1"; shift
	file="$1"; shift
	fields="$*"

	case "$(lowercase "$file")" in
		*.flac)
			# FLAC tagging
			#  --remove-all-tags overwrites existing comments
			VORBISTAG='metaflac --remove-all-tags --import-tags-from=-'
			;;
		*.ogg)
			# Ogg Vorbis tagging
			# -w overwrites existing comments
			# -a appends to existing comments
			VORBISTAG='vorbiscomment -w -c -'
			;;
		*.txt)
			# VC text file format
			# TODO: this also outputs to stdout
			VORBISTAG='tee'
			;;
	esac

	# space separated list of recommended standard field names
	# see http://www.xiph.org/ogg/vorbis/doc/v-comment.html
	# TRACKTOTAL is not in the Xiph recommendation, but is in common use

	[ -n "$fields" ] ||
	fields='TITLE VERSION ALBUM TRACKNUMBER TRACKTOTAL ARTIST PERFORMER COPYRIGHT LICENSE ORGANIZATION DESCRIPTION GENRE DATE LOCATION CONTACT ISRC'

	# fields' corresponding cueprint conversion characters
	# separate alternates with a space

	TITLE='%t'
	VERSION=''
	ALBUM='%T'
	TRACKNUMBER='%02n'
	TRACKTOTAL='%02N'
	ARTIST='%c %p'
	PERFORMER='%p'
	COPYRIGHT=''
	LICENSE=''
	ORGANIZATION=''
	DESCRIPTION='%m'
	GENRE='%g'
	DATE=''
	LOCATION=''
	CONTACT=''
	ISRC='%i %u'

	(for field in $fields; do
		case "$field" in
			(*=*) echo "$field";;
			(*)
				value=''
				for conv in $(eval echo \$"$field"); do
					value=$($CUEPRINT -n "$trackno" -t "$conv\n" "$cue_file")
					if [ -n "$value" ]; then
						echo "$field=$value"
						break
					fi
				done
				;;
		esac
	 done) | $VORBISTAG "$file"
}

id3()
{
	for mp3_cmd in mid3v2 id3v2; do
		type "$mp3_cmd" >/dev/null 2>&1 && MP3TAG="$mp3_cmd" && break
	done
	if [ -z "${MP3TAG}" ]; then
		echo "error: not found '(m)id3v2'."
		exit 1
	fi

	# space separated list of ID3 v1.1 tags
	# see http://id3lib.sourceforge.net/id3/id3v1.html

	fields='TITLE ALBUM ARTIST YEAR COMMENT GENRE TRACKNUMBER'

	# fields' corresponding cueprint conversion characters
	# separate alternates with a space

	TITLE='%t'
	ALBUM='%T'
	ARTIST='%p'
	YEAR=''
	COMMENT='%c'
	GENRE='%g'
	TRACKNUMBER='%n'

	for field in $fields; do
		case "$field" in
			*=*) value="${field#*=}";;
			*)
				value=''
				for conv in $(eval echo \$"$field"); do
					value=$($CUEPRINT -n "$1" -t "$conv\n" "$cue_file")

					if [ -n "$value" ]; then
						break
					fi
				done
				;;
		esac

		if [ -n "$value" ]; then
			case $field in
				TITLE)
					$MP3TAG -t "$value" "$2"
					;;
				ALBUM)
					$MP3TAG -A "$value" "$2"
					;;
				ARTIST)
					$MP3TAG -a "$value" "$2"
					;;
				YEAR)
					$MP3TAG -y "$value" "$2"
					;;
				COMMENT)
					$MP3TAG -c "$value" "$2"
					;;
				GENRE)
					$MP3TAG -g "$value" "$2"
					;;
				TRACKNUMBER)
					$MP3TAG -T "$value" "$2"
					;;
			esac
		fi
	done
}

main()
{
	if [ $# -lt 1 ]; then
		usage
		exit
	fi

	cue_file=$1
	shift

	ntrack=$(cueprint -d '%N' "$cue_file")
	trackno=1

	NUM_FILES=0 FIELDS=
	for arg in "$@"; do
		case "$arg" in
			*.*) NUM_FILES=$(( NUM_FILES + 1));;
			*) FIELDS="$FIELDS $arg";;
		esac
	done

	if [ "$NUM_FILES" -ne "$ntrack" ]; then
		echo 'warning: number of files does not match number of tracks'
	fi

	for file in "$@"; do
		case "$(lowercase "$file")" in
			*.flac|*.ogg)
				vorbis $trackno "$file" $FIELDS
				;;
			*.mp3)
				id3 $trackno "$file" $FIELDS
				;;
			*.txt)
				vorbis $trackno "$file"
				;;
			*.*)
				echo "$file: uknown file type"
				;;
		esac
		trackno=$((trackno + 1))
	done
}

main "$@"
