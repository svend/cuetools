libcuefile_sources = [
    'cd.c',
    'cdtext.c',
    'time.c',
    'cuefile.c',
    'cue_print.c',
    'toc_print.c',
]
libcuefile_sources += lgen.process(
    'cue_scan.l',
    'toc_scan.l',
)
libcuefile_sources += ygen.process(
    'cue_parse.y',
    'toc_parse.y',
)

libcuefile = static_library('cuefile',
    libcuefile_sources,
)

libcuefile_include_directories = include_directories('.')
