project(
    'navigation',
    'cpp', 'c',
    version : '0.0.1',
    default_options: [
        'cpp_std=c++17',
        'werror=true',
        'warning_level=3',
    ]
)

# Meson will find correct threading library (i.e. pthreads or W32)
thread_dep = dependency('threads')

# External dependencies
asio_dep = dependency('asio')
catch2_dep = dependency('catch2')

cmake = import('cmake')
grpc_options = cmake.subproject_options()
grpc_options.add_cmake_defines({
    'CMAKE_BUILD_TYPE': 'RELEASE',
    'gRPC_BUILD_GRPC_CPP_PLUGIN': 'ON',
    'gRPC_BUILD_GRPC_CSHARP_PLUGIN': 'OFF',
    'gRPC_BUILD_GRPC_NODE_PLUGIN': 'OFF',
    'gRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN': 'OFF',
    'gRPC_BUILD_GRPC_PHP_PLUGIN': 'OFF',
    'gRPC_BUILD_GRPC_PYTHON_PLUGIN': 'OFF',
    'gRPC_BUILD_GRPC_RUBY_PLUGIN': 'OFF',
    'gRPC_BUILD_TESTS': 'OFF',
    'gRPC_SSL_PROVIDER': 'package',
    'ABSL_PROPAGATE_CXX_STD': 'ON',
})

grpc_options.set_override_option('werror', 'false')
grpc_options.set_override_option('warning_level', '0')
#grpc = cmake.subproject('grpc', options: grpc_options)
#grpc_dep = grpc.dependency('grpc++')

grpc_dep = dependency('gRPC')

subdir('src')
subdir('test')
