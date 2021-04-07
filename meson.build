project('cuetools',
    'c',
    version: '1.4.0',
    meson_version: '>=0.0.0',
    default_options: [
        'c_std=gnu17',
        'warning_level=3',
    ],
)

#######################################
# Flex / Bison generators

flex = find_program('flex')
bison = find_program('bison')

lgen = generator(flex,
    output : '@PLAINNAME@.c',
    arguments : [ '-o', '@OUTPUT@', '@INPUT@', ],
)

ygen = generator(bison,
    output : [ '@BASENAME@.c', '@BASENAME@.h' ],
    arguments : [
        '@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@',
        '--color=always', '-d',
    ],
)


subdir('doc')
subdir('src')
subdir('extras')
