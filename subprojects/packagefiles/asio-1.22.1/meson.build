project('asio',
    'cpp',
    version : '1.22.1',
    license : 'Boost'
)

asio_deps = []

# For windows builds, ASIO depends on ws2_32
if build_machine.system() == 'windows'
    compiler = meson.get_compiler('cpp')
    ws2_32_dep = compiler.find_library('ws2_32', required: true)
    asio_deps += ws2_32_dep
endif

asio_dep = declare_dependency(
    include_directories : include_directories('asio/include'),
    dependencies: asio_deps,
)
