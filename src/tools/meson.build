tools = [
    'cuebreakpoints',
    'cueconvert',
    'cueprint',
]

foreach tool : tools
    executable(tool,
        tool + '.c',
        link_with: libcuefile,
        include_directories: libcuefile_include_directories,
    )
endforeach

install_data('cuetag.sh',
    install_dir: get_option('bindir'),
)
