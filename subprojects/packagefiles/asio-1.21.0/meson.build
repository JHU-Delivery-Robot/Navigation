project('asio',
    'cpp',
    version : '1.21.0',
    license : 'Boost'
)

# For windows builds, ASIO depends on ws2_32
if build_machine.system() == 'windows'
    compiler = meson.get_compiler('cpp')
    ws2_32_dep = compiler.find_library('ws2_32', required: true)
endif

asio_dep = declare_dependency(
    include_directories : include_directories('include'),
    dependencies: [ ws2_32_dep ],
)
