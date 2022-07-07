project(
    'deliverbot-navigation',
    'cpp', 'c',
    version : '0.1.0',
    default_options: [
        'cpp_std=c++17',
        'werror=false',
        'warning_level=0',
    ]
)

# Meson will find correct threading library (i.e. pthreads or W32)
thread_dep = dependency('threads')

cpp_compiler = meson.get_compiler('cpp')
bcrypt_dep = cpp_compiler.find_library('bcrypt', required : false)
dbghelp_dep = cpp_compiler.find_library('dbghelp', required : false)
dl_dep = cpp_compiler.find_library('dl', required: false)

# External dependencies
asio_dep = dependency('asio')
catch2_dep = dependency('catch2')
grpc_dep = dependency('gRPC', method: 'cmake', modules: ['gRPC::grpc++'])
proto_dep = dependency('protobuf', method: 'cmake', modules: ['protobuf::libprotobuf'])

subdir('src')
subdir('test')
